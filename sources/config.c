#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include "../headers/config.h"

ServerConfig initCfg()
{
  config_t cfg;

  config_init(&cfg);

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&cfg, "../server.config"))
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

  config_setting_t *roomsSettings = config_lookup(&cfg, "rooms");
  int nbRooms = config_setting_length(roomsSettings);
    
  cfgServer.gameConfig.nbRooms = nbRooms;
  
  for(int i = 0; i < nbRooms; ++i)
  {
    Room theRoom;
    config_setting_t *room = config_setting_get_elem(roomsSettings, i);

    const char *name, *idClient_1, *idClient_2;
    int bank, nbRounds;

    config_setting_lookup_string(room, "name", &name) ;
    config_setting_lookup_int(room, "bank", &bank);
    config_setting_lookup_int(room, "nbRounds", &nbRounds);
    config_setting_lookup_string(room, "idClient_1", &idClient_1);
    config_setting_lookup_string(room, "idClient_2", &idClient_2);

    theRoom.name = name;
    theRoom.bank = bank;
    theRoom.nbRounds = nbRounds;
    theRoom.idClient_1 = idClient_1;
    theRoom.idClient_2 = idClient_2;
    cfgServer.gameConfig.rooms[i] = theRoom;
  }
  return cfgServer;
}


void showConfig(ServerConfig cfgServer)
{
    printf("\n\033[0;36m--------------------------\n\033[1;36m SERVER CONFIGURATION\n \n\033[1;37m - IP Address: \033[0;36m%s \n\033[1;37m - Port: \033[0;36m%d \n\033[0;36m--------------------------\n", cfgServer.serverIP, cfgServer.serverPort);
}

void showRooms(ServerConfig cfgServer)
{
    printf("\n\033[0;36m--------------------------\n\033[1;36m ROOMS CONFIGURATION\n");
    for(int i = 0; i < cfgServer.gameConfig.nbRooms; ++i)
    {
      printf("\n \033[1;37m- Room name: \033[0;36m%s  \033[1;37mBank: \033[0;36m%d  \033[1;37mNumber of rounds: \033[0;36m%d  \033[1;37mClient #1 ID: \033[0;36m%s  \033[1;37mClient #2 ID: \033[0;36m%s\n", cfgServer.gameConfig.rooms[i].name, cfgServer.gameConfig.rooms[i].bank, cfgServer.gameConfig.rooms[i].nbRounds, cfgServer.gameConfig.rooms[i].idClient_1, cfgServer.gameConfig.rooms[i].idClient_2);
    }
    printf("\n\033[0;36m--------------------------\n");
}