#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include "../headers/config.h"

ServerConfig initCfg()
{
  config_t cfg;

  config_init(&cfg);

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&cfg, "../config.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
  }

  const char *serverIP;
  int serverPort = 0;
  config_lookup_string(&cfg, "serverIP", &serverIP);
  config_lookup_int(&cfg, "serverPort", &serverPort);

  ServerConfig cfgServer;
  cfgServer.serverIP = serverIP;
  cfgServer.serverPort = serverPort;

  config_setting_t *boxesSettings = config_lookup(&cfg, "boxes");
  int nbBoxes = config_setting_length(boxesSettings);
    
  cfgServer.gameConfig.nbBoxes = nbBoxes;
  
  for(int i = 0; i < nbBoxes; ++i)
  {
    Box theBox;
    config_setting_t *box = config_setting_get_elem(boxesSettings, i);

    const char *name;
    int bank, nbRounds, idClient_1, idClient_2;

    config_setting_lookup_string(box, "name", &name) ;
    config_setting_lookup_int(box, "bank", &bank);
    config_setting_lookup_int(box, "nbRounds", &nbRounds);
    config_setting_lookup_int(box, "idClient_1", &idClient_1);
    config_setting_lookup_int(box, "idClient_2", &idClient_2);

    theBox.name = name;
    theBox.bank = bank;
    theBox.nbRounds = nbRounds;
    theBox.idClient_1 = idClient_1;
    theBox.idClient_2 = idClient_2;
    theBox.index = i;

    cfgServer.gameConfig.boxes[i] = theBox;
  }
  return cfgServer;
}


void showConfig(ServerConfig cfgServer)
{
    printf("\n\033[0;36m--------------------------\n\033[1;36m SERVER CONFIGURATION\n \n\033[1;37m - IP Address: \033[0;36m%s \n\033[1;37m - Port: \033[0;36m%d \n\033[0;36m--------------------------\n", cfgServer.serverIP, cfgServer.serverPort);
}

void showBoxes(ServerConfig cfgServer)
{
    printf("\n\033[0;36m--------------------------\n\033[1;36m BOXES CONFIGURATION\n");
    for(int i = 0; i < cfgServer.gameConfig.nbBoxes; ++i)
    {
      printf("\n \033[1;37m- Box name: \033[0;36m%s  \033[1;37mBank: \033[0;36m%d  \033[1;37mNumber of rounds: \033[0;36m%d  \033[1;37mClient #1 ID: \033[0;36m%d  \033[1;37mClient #2 ID: \033[0;36m%d\n", cfgServer.gameConfig.boxes[i].name, cfgServer.gameConfig.boxes[i].bank, cfgServer.gameConfig.boxes[i].nbRounds, cfgServer.gameConfig.boxes[i].idClient_1, cfgServer.gameConfig.boxes[i].idClient_2);
    }
    printf("\n\033[0;36m--------------------------\n");
}