#pragma once

#include <arpa/inet.h>

struct net {
  int socket;
  struct sockaddr_in server_addr;
};

bool net_init(struct net*);
void net_process_gui(struct net*);
void net_update(struct net*);