#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include <stdbool.h>

#ifndef THEPRISONER_CONFIG_H
#define THEPRISONER_CONFIG_H

typedef struct {
    const char *name;
    int nbRounds;
    int bank;
    int idClient_1;
    int idClient_2;
} Room;

typedef struct {
    Room rooms[50];
    int nbRooms;
} GameConfig;

typedef struct {
    const char *serverIP;
    int serverPort;
    GameConfig gameConfig;
} ServerConfig;

typedef struct {
    char serverIP[15];
    int serverPort;
    int idClient;
} ClientConfig;

ServerConfig initServerCfg();
void showServerConfig(ServerConfig cfgServer);
void showRooms(ServerConfig cfgServer);

ClientConfig initClientCfg();
void showClientConfig(ClientConfig cfgClient);

#endif /* THEPRISONER_CONFIG_H */