#include "network.h"

#include <stdio.h>

#include <arpa/inet.h>
#include <imgui.h>
#include <unistd.h>

#include "sys/socket.h"

#define SERVER_STR_SIZE 128

bool net_init(struct net* net) {
  net->socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (net->socket <= 0) {
    fprintf(stderr, "ERROR: Couldn't create socket.\n");
    return false;
  }
  return true;
}

void net_process_gui(struct net* net) {
  ImGui::Begin("Connection");

  static char server_str[SERVER_STR_SIZE];
  static int port;
  struct sockaddr_in* server_addr = &net->server_addr;

  ImGui::InputText("server", server_str, SERVER_STR_SIZE);
  ImGui::InputInt("port", &port);
  if (ImGui::Button("Connect")) {
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    if (inet_pton(AF_INET, server_str, &server_addr->sin_addr) <= 0) {
      ImGui::OpenPopup("connerrpopup");
    }
  }

  if (ImGui::BeginPopup("connerrpopup")) {
    ImGui::Text("Invalid server address and/or port.");
    ImGui::EndPopup();
  }

  if (ImGui::Button("Disconnect")) {
    close(net->socket);
    net->socket = -1;
  }

  if (ImGui::Button("Send test message")) {
    const char test[] = "test message\n";
    sendto(net->socket, test, sizeof(test), 0,
           (const struct sockaddr*)server_addr, sizeof(*server_addr));
  }

  ImGui::End();
}

void net_update(struct net* net) {}
