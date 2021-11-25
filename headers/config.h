#include <stdio.h>
#include <stdlib.h>
#ifndef THEPRISONER_SERVER_CONFIG_H
#define THEPRISONER_SERVER_CONFIG_H

typedef struct {
    const char *name;
    int nbRounds;
    int bank;
    int idClient_1;
    int idClient_2;
    int index;
} Box;

typedef struct {
    Box boxes[50];
    int nbBoxes;
} GameConfig;

typedef struct {
    const char *serverIP;
    int serverPort;
    GameConfig gameConfig;
} ServerConfig;

ServerConfig initCfg();
void showConfig(ServerConfig cfgServer);
void showBoxes(ServerConfig cfgServer);

#endif /* THEPRISONER_SERVER_CONFIG_H */