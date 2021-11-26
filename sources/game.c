#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../headers/game.h"
#include "../headers/config.h"

PlayerGameSetting *gameSettings;

PlayerGameSetting * init(ServerConfig cfgServer, int roomID){
    gameSettings = (PlayerGameSetting *)malloc(sizeof(PlayerGameSetting));
    gameSettings->action = SILENCE;
    gameSettings->balance = cfgServer.gameConfig.rooms[roomID].bank;
    gameSettings->bet = 0;
    gameSettings->currentR = 1;
    gameSettings->totalR = cfgServer.gameConfig.rooms[roomID].nbRounds;
    return gameSettings;
}

PlayerGameSetting *getGameSettings()
{ 
    return gameSettings; 
}

void playRound(PlayerGameSetting *p1_gameSettings, PlayerGameSetting *p2_gameSettings){
    if (p1_gameSettings->action == BETRAY) { /*Le joueur 1 trahi */
        if (p2_gameSettings->action == BETRAY) { /*Le joueur 2 trahi */
            p1_gameSettings->balance -= p1_gameSettings->bet;
            p2_gameSettings->balance -= p2_gameSettings->bet;
        }
        if(p2_gameSettings->action == COOP) { /*Le joueur 2 se tait */
            p1_gameSettings->balance += p1_gameSettings->bet;
            p2_gameSettings->balance -= p2_gameSettings->bet;
        }
    } else if (p1_gameSettings->action == COOP) { /*Le joueur 2 se tait */
        if (p2_gameSettings->action == BETRAY) { /*Le joueur 2 trahi */
            p1_gameSettings->balance -= p1_gameSettings->bet;
            p2_gameSettings->balance += p2_gameSettings->bet;
        }
        if(p2_gameSettings->action == COOP) { /*Le joueur 2 se tait */
            p1_gameSettings->balance -= (p1_gameSettings->bet / 2);
            p2_gameSettings->balance -= (p2_gameSettings->bet / 2);
        }
    }
}

bool nextRound(PlayerGameSetting *gameSettings)
{
    if(gameSettings->currentR <=  gameSettings->totalR)
    {
        
    }
}