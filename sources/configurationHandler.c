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

void showConfig()
{
    configuration config;
    config = getConfig();
    char *showNb = (char*)malloc(sizeof(char));
    char *coop = (char*)malloc(sizeof(char));

    if(config.game.showNbRound == 1)
    {
        showNb = "Oui";
    }
    else
    {
        showNb = "Non";
    }

    if(config.game.coopMutuelle == 1)
    {
        coop = "50/50";
    }
    else
    {
        coop = "Proportionnel";
    }

    printf("\n --------------------------\nCONFIGURATION SERVEUR\n \n- Adresse IP: %s \n- Port: %d \n- Nombre de tours: %d \n- Temps par tour: %d \n- Solde joueur A: %d \n- Solde joueur B: %d \n- Montrer le nombre de tours: %s \n- Coopération mutuelle: %s \n--------------------------\n", config.sys.adresseIP, config.sys.port, config.game.nbRound, config.game.roundTime, config.game.bankA, config.game.bankB, showNb, coop);
}