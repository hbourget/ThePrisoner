#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include <stdbool.h>

#ifndef THEPRISONER_CLIENT_CONFIG_H
#define THEPRISONER_CLIENT_CONFIG_H

typedef struct {
    char serverIP[15];
    int serverPort;
    int idClient;
} ClientConfig;

ClientConfig initCfg();
void showConfig(ClientConfig cfgClient);

#endif /* THEPRISONER_CLIENT_CONFIG_H */