#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifndef THEPRISONER_SERVER_CONFIG_H
#define THEPRISONER_SERVER_CONFIG_H

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

ServerConfig initCfg();
void showConfig(ServerConfig cfgServer);
void showRooms(ServerConfig cfgServer);

#endif /* THEPRISONER_SERVER_CONFIG_H */
