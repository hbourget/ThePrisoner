#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../common/game.h"
#include "../../common/config.h"

/**
* @brief Instanciation du PlayerGameSettings pour le client x appartenant à la room x
*
* @param cfgServer Configuration du serveur
* @param roomID ID de la room
* @param idClient ID du client
*/
PlayerGameSettings initPlayerGameSettings(ServerConfig cfgServer, int roomID, int idClient) {
    PlayerGameSettings gameSettings;

    gameSettings.action = START;
    gameSettings.balance = cfgServer.gameConfig.rooms[roomID].bank;
    gameSettings.bet = 0;
    gameSettings.idClient = idClient;
    return gameSettings;
}

/**
* @brief Instanciation du PlayerGameSettings pour le client x appartenant à la room x
*
* @param cfgPlayer Configuration du PlayerGameSettings
* @param gameData Structure des informations de jeu des clients propre à leur room actuelle
* @param cfgServer Configuration du serveur
* @param roomID ID de la room
*/
GameData hydrateGameData(PlayerGameSettings cfgPlayer, GameData gameData, ServerConfig cfgServer, int roomID)
{
    if(cfgServer.gameConfig.rooms[roomID].idClient_1 == cfgPlayer.idClient)
    {
        gameData.p1 = cfgPlayer;
    }
    else
    {
        gameData.p2 = cfgPlayer;
    }

    gameData.currentRound = 1;
    gameData.totalRounds = cfgServer.gameConfig.rooms[roomID].nbRounds;
    return gameData;
}

/**
* @brief Algorithme de traitement des données envoyées par les clients
*
* @param gameData Structure des informations de jeu des clients propre à leur room actuelle
*/
void playRound(GameData gameData)
{
    if (gameData.p1.action == BETRAY)
    { /*Le joueur 1 trahi */
        if (gameData.p2.action == BETRAY)
        { /*Le joueur 2 trahi */
            gameData.p1.balance -= gameData.p1.bet;
            gameData.p2.balance -= gameData.p2.bet;
        }
        if(gameData.p2.action == COOP)
        { /*Le joueur 2 se tait */
            gameData.p1.balance += gameData.p1.bet;
            gameData.p2.balance -= gameData.p2.bet;
        }
    } 
    else if (gameData.p1.action == COOP)
    { /*Le joueur 2 se tait */
        if (gameData.p2.action == BETRAY)
        { /*Le joueur 2 trahi */
            gameData.p1.balance -= gameData.p1.bet;
            gameData.p2.balance += gameData.p2.bet;
        }
        if(gameData.p2.action == COOP)
        { /*Le joueur 2 se tait */
            gameData.p1.balance -= (gameData.p1.bet / 2);
            gameData.p2.balance -= (gameData.p2.bet / 2);
        }
    }
    gameData.currentRound += 1;
}