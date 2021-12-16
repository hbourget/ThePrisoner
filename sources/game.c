#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../headers/game.h"
#include "../headers/config.h"

PlayerGameSettings initPlayerGameSettings(ServerConfig cfgServer, int roomID, int idClient) {
    PlayerGameSettings gameSettings;

    // gameSettings = (PlayerGameSettings)malloc(sizeof(PlayerGameSettings));
    gameSettings.action = SILENCE;
    gameSettings.balance = cfgServer.gameConfig.rooms[roomID].bank;
    gameSettings.bet = 0;
    gameSettings.currentR = 1;
    gameSettings.totalR = cfgServer.gameConfig.rooms[roomID].nbRounds;
    gameSettings.responded = false;
    gameSettings.idClient = idClient;
    return gameSettings;
}

GameData hydrateData(GameData gameData, PlayerGameSettings cfgPlayer)
{
    if(gameData.p1.idClient == cfgPlayer.idClient)
    {
        gameData.p1 = cfgPlayer;
    }
    else if (gameData.p2.idClient == cfgPlayer.idClient)
    {
        gameData.p2 = cfgPlayer;
    }

    return gameData;
}

GameData hydrateGameData(PlayerGameSettings cfgPlayer, GameData gameData, ServerConfig cfgServer, int i)
{

    if(cfgServer.gameConfig.rooms[i].idClient_1 == cfgPlayer.idClient)
    {
        gameData.p1 = cfgPlayer;
    }
    else
    {
        gameData.p2 = cfgPlayer;
    }
    return gameData;
}

void playRound(PlayerGameSettings p1_gameSettings, PlayerGameSettings p2_gameSettings)
{
    //Retourne vrai si currentR <= totalR pour les 2 gameSettings et ajoute 1 à currentR pour chaque gameSettings
    if(nextround(p1_gameSettings, p2_gameSettings))
    {
        if (p1_gameSettings.action == BETRAY)
        { /*Le joueur 1 trahi */
            if (p2_gameSettings.action == BETRAY)
            { /*Le joueur 2 trahi */
                p1_gameSettings.balance -= p1_gameSettings.bet;
                p2_gameSettings.balance -= p2_gameSettings.bet;
            }
            if(p2_gameSettings.action == COOP)
            { /*Le joueur 2 se tait */
                p1_gameSettings.balance += p1_gameSettings.bet;
                p2_gameSettings.balance -= p2_gameSettings.bet;
            }
        } 
        else if (p1_gameSettings.action == COOP)
        { /*Le joueur 2 se tait */
            if (p2_gameSettings.action == BETRAY)
            { /*Le joueur 2 trahi */
                p1_gameSettings.balance -= p1_gameSettings.bet;
                p2_gameSettings.balance += p2_gameSettings.bet;
            }
            if(p2_gameSettings.action == COOP)
            { /*Le joueur 2 se tait */
                p1_gameSettings.balance -= (p1_gameSettings.bet / 2);
                p2_gameSettings.balance -= (p2_gameSettings.bet / 2);
            }
        }
    }
}

bool nextround(PlayerGameSettings p1_gameSettings, PlayerGameSettings p2_gameSettings)
{
    bool ret = true;
    if(p1_gameSettings.currentR <=  p1_gameSettings.totalR && p2_gameSettings.currentR <=  p2_gameSettings.totalR)
    {
        p1_gameSettings.currentR += 1;
        p2_gameSettings.currentR += 1;
    }
    else
    {
        ret = false;
    }
    return ret;
}