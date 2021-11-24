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
} systemrules;

typedef struct
{
    int nbRound;
    int roundTime;
    int bank;
    int showNbRound;
    int coopMutuelle;
    const char* idClient1;
    const char* idClient2;
} match1;

typedef struct
{
    int nbRound;
    int roundTime;
    int bank;
    int showNbRound;
    int coopMutuelle;
    const char* idClient1;
    const char* idClient2;
} match2;

typedef struct
{
    systemrules sys;
    match1 m1;
    match2 m2;
} configuration;


static int handler(void* config, const char* section, const char* name, const char* value);

configuration getServerConfig();

void showServerConfig();

#endif //THEPRISONER_SERVER_CONFIGURATIONHANDLER_H
