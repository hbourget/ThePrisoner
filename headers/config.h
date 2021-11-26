#include <stdio.h>
#include <stdlib.h>
#ifndef THEPRISONER_SERVER_CONFIG_H
#define THEPRISONER_SERVER_CONFIG_H

typedef struct {
    const char *name;
    int nbRounds;
    int bank;
    const char *idClient_1;
    const char *idClient_2;
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

ServerConfig initCfg();
void showConfig(ServerConfig cfgServer);
void showRooms(ServerConfig cfgServer);
#endif /* THEPRISONER_SERVER_CONFIG_H */