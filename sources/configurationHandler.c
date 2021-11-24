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

    //Server
    if(MATCH("systemrules", "serverIP"))
    {
        pconfig->sys.serverIP = strdup(value);
    }
    else if(MATCH("systemrules", "serverPort"))
    {
        pconfig->sys.serverPort = atoi(value);
    }
    //Match 1
    else if(MATCH("match1", "nbRound"))
    {
        pconfig->m1.nbRound = atoi(value);
    }
    else if(MATCH("match1", "roundTime"))
    {
        pconfig->m1.roundTime = atoi(value);
    }
    else if(MATCH("match1", "bank"))
    {
        pconfig->m1.bank = atoi(value);
    }
    else if(MATCH("match1", "showNbRound"))
    {
        pconfig->m1.showNbRound = atoi(value);
    }
    else if(MATCH("match1", "coopMutuelle"))
    {
        pconfig->m1.coopMutuelle = atoi(value);
    }
    else if(MATCH("match1", "idClient1"))
    {
        pconfig->m1.idClient1 = strdup(value);
    }
    else if(MATCH("match1", "idClient2"))
    {
        pconfig->m1.idClient2 = strdup(value);
    }

    //Match 2
    else if(MATCH("match2", "nbRound"))
    {
        pconfig->m2.nbRound = atoi(value);
    }
    else if(MATCH("match2", "roundTime"))
    {
        pconfig->m2.roundTime = atoi(value);
    }
    else if(MATCH("match2", "bank"))
    {
        pconfig->m2.bank = atoi(value);
    }
    else if(MATCH("match2", "showNbRound"))
    {
        pconfig->m2.showNbRound = atoi(value);
    }
    else if(MATCH("match2", "coopMutuelle"))
    {
        pconfig->m2.coopMutuelle = atoi(value);
    }
    else if(MATCH("match2", "idClient1"))
    {
        pconfig->m2.idClient1 = strdup(value);
    }
    else if(MATCH("match2", "idClient2"))
    {
        pconfig->m2.idClient2 = strdup(value);
    }

    return 1;
}

configuration getServerConfig()
{
    configuration config;
    config.sys.serverIP = NULL;
    config.sys.serverPort = 0;

    if (ini_parse("../config.ini", handler, &config) < 0) 
    {
        printf("Une erreur est survenue lors de l'ouverture du fichier\n");
    }

    return config;
}

void showServerConfig()
{
    configuration config;
    config = getServerConfig();

    printf("\n --------------------------\nCONFIGURATION SERVEUR\n \n- Adresse IP: %s \n- Port: %d \n --------------------------\n", config.sys.serverIP, config.sys.serverPort);
}

void showMatch1Config()
{
    configuration config = getServerConfig();
    char *coop = (char*)malloc(sizeof(char));

    if(config.m1.coopMutuelle == 1)
    {
        coop = "50/50";
    }
    else
    {
        coop = "Proportionnel";
    }

    printf("\n --------------------------\nCONFIGURATION MATCH 1\n \n- Nombre de tours: %d \n- Temps par tour: %d \n- Banque: %d \n- Coopération mutuelle: %s \n- ID Client #1 : %s \n- ID Client #2 : %s \n--------------------------\n", config.m1.nbRound, config.m1.roundTime, config.m1.bank, coop, config.m1.idClient1, config.m1.idClient2);
}


void showMatch2Config()
{
    configuration config = getServerConfig();
    char *coop = (char*)malloc(sizeof(char));

    if(config.m2.coopMutuelle == 1)
    {
        coop = "50/50";
    }
    else
    {
        coop = "Proportionnel";
    }

     printf("\n --------------------------\nCONFIGURATION MATCH 2\n \n- Nombre de tours: %d \n- Temps par tour: %d \n- Banque: %d \n- Coopération mutuelle: %s \n- ID Client #1 : %s \n- ID Client #2 : %s \n--------------------------\n", config.m2.nbRound, config.m2.roundTime, config.m2.bank, coop, config.m2.idClient1, config.m2.idClient2);
}
