#include <stdio.h>

#include <stdlib.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

static struct addrinfo* get_local_addresses(const char* port_str) {
  struct addrinfo* result;

  struct addrinfo hints = (struct addrinfo){};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  int status = getaddrinfo(0, port_str, &hints, &result);
  if (status != 0) {
    fprintf(stderr, "ERROR: getaddrinfo failed: %s\n", gai_strerror(status));
    exit(EXIT_FAILURE);
  }

  return result;
}

static void print_address(struct sockaddr* addr, socklen_t addr_len) {
  const uint32_t host_str_len = 64;
  const uint32_t serv_str_len = 64;
  char host_str[host_str_len] = {};
  char serv_str[serv_str_len] = {};

  int status = getnameinfo(addr, addr_len, host_str, host_str_len, serv_str,
                           serv_str_len, 0);
  if (status != 0) {
    fprintf(stderr, "ERROR: getnameinfo failed.\n");
    exit(EXIT_FAILURE);
  }
  printf("%s:%s\n", host_str, serv_str);
}

struct server {
  int socket;
};

bool server_init(struct server* srv, struct addrinfo* local_addresses) {
  if (local_addresses == 0)
    return false;

  srv->socket = socket(AF_INET, SOCK_DGRAM, 0);

  if (srv->socket <= 0) {
    fprintf(stderr, "ERROR: Couldn't initialize socket.\n");
    return false;
  }

  int status =
      bind(srv->socket, local_addresses->ai_addr, local_addresses->ai_addrlen);
  if (status != 0) {
    fprintf(stderr, "ERROR: Couldn't bind socket.\n");
    return false;
  }

  return true;
}

void server_update(struct server* srv) {
  const uint32_t recv_buf_size = 128;
  char recv_buf[recv_buf_size];

  struct sockaddr src_addr;
  socklen_t src_addr_len = sizeof(src_addr);

  uint32_t nbytes = recvfrom(srv->socket, recv_buf, recv_buf_size, 0, &src_addr,
                             &src_addr_len);
  if (nbytes == -1)
    return;

  printf("From: ");
  print_address(&src_addr, src_addr_len);
  printf("[%d]: %s\n", nbytes, recv_buf);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct addrinfo* local_addresses = get_local_addresses(argv[1]);
  if (local_addresses == 0) {
    fprintf(stderr, "ERROR: No local addresses found.\n");
    exit(EXIT_FAILURE);
  }

  for (struct addrinfo* c_addr = local_addresses; c_addr != 0;
       c_addr = c_addr->ai_next) {
    print_address(c_addr->ai_addr, c_addr->ai_addrlen);
  }

  struct server server;
  if (!server_init(&server, local_addresses)) {
    fprintf(stderr, "ERROR: Failed to initialize server.\n");
    exit(EXIT_FAILURE);
  }

  for (;;) {
    server_update(&server);
  }

  freeaddrinfo(local_addresses);

  return 0;
}