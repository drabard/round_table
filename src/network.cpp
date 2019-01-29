#include "network.h"

#include <stdio.h>

#include <arpa/inet.h>
#include <imgui.h>

#include "sys/socket.h"

void net_init(struct net* net) {
  net->socket = -1;
  net->is_server = false;
}

void net_process_gui(struct net* net) {
  ImGui::Begin("Network debug");
  ImGui::Text("Socket: %d", net->socket);
  ImGui::Text("Server: %d", net->is_server);
  if (ImGui::Button("Run server")) {
    net->is_server = true;
    net->socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (net->socket <= 0) {
      fprintf(stderr, "Couldn't create socket.");
    }
  }
  ImGui::End();
}

void net_update(struct net* net) {
  if (net->socket > 0) {
    if (net->is_server) {
      struct sockaddr_in client_addr;
      client_addr.sin_family = AF_INET;
      client_addr.sin_port = htons(1234);
      inet_pton(AF_INET, "127.0.0.1", &client_addr.sin_addr);

      const char test[] = "test message\n";
      sendto(net->socket, test, sizeof(test), 0,
             (const struct sockaddr*)&client_addr, sizeof(client_addr));
    }
  }
}