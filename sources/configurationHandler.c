#include <string.h>
#include <stdlib.h>
#include "../headers/ini.h"
#include "../headers/configurationHandler.h"

static int handler(void* config, const char* section, const char* name, const char* value)
{
    // config instance for filling in the values.
    configuration* pconfig = (configuration*)config;

    // define a macro for checking Sections and keys under the sections.
    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

    // fill the values in config struct for Section 1.
    if(MATCH("systemrules", "adresseIP"))
    {
        pconfig->sys.adresseIP = strdup(value);
    }
    else if(MATCH("systemrules", "port"))
    {
        pconfig->sys.port = atoi(value);
    }
    else if(MATCH("gamerules", "nbRound"))
    {
        pconfig->game.nbRound = atoi(value);
    }
    else if(MATCH("gamerules", "roundTime"))
    {
        pconfig->game.roundTime = atoi(value);
    }
    else if(MATCH("gamerules", "bankA"))
    {
        pconfig->game.bankA = atoi(value);
    }
    else if(MATCH("gamerules", "bankB"))
    {
        pconfig->game.bankB = atoi(value);
    }
    else if(MATCH("gamerules", "showNbRound"))
    {
        pconfig->game.showNbRound = atoi(value);
    }
    else if(MATCH("gamerules", "coopMutuelle"))
    {
        pconfig->game.coopMutuelle = atoi(value);
    }

    return 1;
}

configuration getConfig()
{
    configuration config;
    config.sys.adresseIP = NULL;
    config.sys.port = 20;
    config.game.nbRound = 0;
    config.game.roundTime = 0;
    config.game.bankA = 0;
    config.game.bankB = 0;
    config.game.showNbRound = 0;
    config.game.coopMutuelle = 0;

    if (ini_parse("../config.ini", handler, &config) < 0) 
    {
        printf("Une erreur est survenue lors de l'ouverture du fichier\n");
    }

    return config;
}