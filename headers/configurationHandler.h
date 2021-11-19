//
// Created by student on 11/19/21.
//
#include <stdbool.h>

#ifndef THEPRISONER_SERVER_CONFIGURATIONHANDLER_H
#define THEPRISONER_SERVER_CONFIGURATIONHANDLER_H

typedef struct
{
    const char* adresseIP;
    int port;
} systemrules;

typedef struct
{
    int nbRound;
    int roundTime;
    int bankA;
    int bankB;
    int showNbRound;
    int coopMutuelle;
} gamerules;

typedef struct
{
    systemrules sys;
    gamerules game;
} configuration;


static int handler(void* config, const char* section, const char* name, const char* value);

configuration getConfig();

#endif //THEPRISONER_SERVER_CONFIGURATIONHANDLER_H
