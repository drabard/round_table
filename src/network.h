#pragma once

struct net {
  int socket;
};

void net_init(struct net*);
void net_process_gui(struct net*);
void net_update(struct net*);