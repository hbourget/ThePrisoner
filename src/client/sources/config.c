#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include <string.h>
#include "../../common/configurations.h"

/**
* @brief Initialisation de la configuration du client
*/
ClientConfig initClientCfg()
{
  config_t cfg;
  config_init(&cfg);

  // Lit le fichier, si il y a une erreur, affichage + destruction.
  if(! config_read_file(&cfg, "cfg/client.config"))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
  }
 
  const char *serverIP;
  int serverPort = 0;
  int idClient = 0;

  config_lookup_string(&cfg, "serverIP", &serverIP);
  config_lookup_int(&cfg, "serverPort", &serverPort);
  config_lookup_int(&cfg, "idClient", &idClient);

  ClientConfig cfgClient;
  strcpy(cfgClient.serverIP, serverIP);
  cfgClient.serverPort = serverPort;
  cfgClient.idClient = idClient;

  return cfgClient;
}

/**
* @brief Affichage console de la configuration du client
*/
void showClientConfig(ClientConfig cfgClient)
{
    printf("\n\033[0;36m--------------------------\n\033[1;36m CLIENT CONFIGURATION\n \n\033[1;37m - Server IP Address: \033[0;36m%s \n\033[1;37m - Server Port: \033[0;36m%d \n\033[1;37m - Client ID: \033[0;36m%d\n\033[0;36m--------------------------\033[0m\n", cfgClient.serverIP, cfgClient.serverPort, cfgClient.idClient);
}