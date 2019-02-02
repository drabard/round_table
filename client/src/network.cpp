#include "network.h"

#include <stdio.h>

#include <arpa/inet.h>
#include <imgui.h>

#include "sys/socket.h"

void net_init(struct net* net) { net->socket = -1; }

void net_process_gui(struct net* net) {
  ImGui::Begin("Network debug");
  ImGui::Text("Socket: %d", net->socket);
  if (ImGui::Button("Init socket")) {
    net->socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (net->socket <= 0) {
      fprintf(stderr, "Couldn't create socket.");
    }
  }
  if (ImGui::Button("Send test message")) {
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(1234);
    inet_pton(AF_INET, "127.0.0.1", &client_addr.sin_addr);

    const char test[] = "test message\n";
    sendto(net->socket, test, sizeof(test), 0,
           (const struct sockaddr*)&client_addr, sizeof(client_addr));
  }
  ImGui::End();
}

void net_update(struct net* net) {}
