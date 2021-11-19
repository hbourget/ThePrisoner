//
// Created by student on 11/19/21.
//
#include <stdbool.h>

#ifndef THEPRISONER_SERVER_CONFIGURATIONHANDLER_H
#define THEPRISONER_SERVER_CONFIGURATIONHANDLER_H

typedef struct
{
    const char* name;
    int number;
} configuration;


static int handler(void* config, const char* section, const char* name, const char* value);

configuration getConfig();

#endif //THEPRISONER_SERVER_CONFIGURATIONHANDLER_H
