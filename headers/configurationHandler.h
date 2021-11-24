//
// Created by student on 11/19/21.
//
#include <stdbool.h>

#ifndef THEPRISONER_SERVER_CONFIGURATIONHANDLER_H
#define THEPRISONER_SERVER_CONFIGURATIONHANDLER_H

typedef struct
{
    const char* serverIP;
    int serverPort;
} SystemRules;

typedef struct
{
    int nbRound;
    int roundTime;
    int bank;
    int showNbRound;
    int coopMutuelle;
    const char* idClient1;
    const char* idClient2;
} RoomOne;

typedef struct
{
    int nbRound;
    int roundTime;
    int bank;
    int showNbRound;
    int coopMutuelle;
    const char* idClient1;
    const char* idClient2;
} RoomTwo;

typedef struct
{
    SystemRules sys;
    RoomOne m1;
    RoomTwo m2;
} Configuration;


static int handler(void* config, const char* section, const char* name, const char* value);

Configuration getServerConfig();

void showServerConfig(Configuration config);

void showMatch1Config(Configuration config);

void showMatch2Config(Configuration config);

#endif //THEPRISONER_SERVER_CONFIGURATIONHANDLER_H
