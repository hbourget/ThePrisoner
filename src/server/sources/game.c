#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../common/game.h"

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
    gameSettings.bet = 0;
    gameSettings.idClient = idClient;
    gameSettings.responded = false;
    return gameSettings;
}

/**
* @brief Instanciation du PlayerGameSettings pour le client x appartenant à la room x à la connexion
*
* @param cfgPlayer Configuration du PlayerGameSettings
* @param gameData Structure des informations de jeu des clients propre à leur room actuelle
* @param cfgServer Configuration du serveur
* @param roomID ID de la room
*/
GameData firstHydrateGameData(PlayerGameSettings cfgPlayer, GameData gameData, ServerConfig cfgServer, int roomID)
{
    if(cfgServer.gameConfig.rooms[roomID].idClient_1 == cfgPlayer.idClient)
    {
        gameData.p1 = cfgPlayer;
    }
    else
    {
        gameData.p2 = cfgPlayer;
    }
    gameData.currentRound = 0;
    gameData.totalRounds = cfgServer.gameConfig.rooms[roomID].nbRounds;
    gameData.bal_p1 = cfgServer.gameConfig.rooms[roomID].bank;
    gameData.bal_p2 = cfgServer.gameConfig.rooms[roomID].bank;
    return gameData;
}

/**
* @brief Instanciation du PlayerGameSettings pour le client x appartenant à la room x lors du jeu
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

    gameData.totalRounds = cfgServer.gameConfig.rooms[roomID].nbRounds;
    return gameData;
}

/**
* @brief Algorithme de traitement des données envoyées par les clients
*
* @param gameData Structure des informations de jeu des clients propre à leur room actuelle
*/
GameData playRound(GameData gameData)
{
    if (gameData.p1.action == BETRAY)
    { /*Le joueur 1 trahi */
        if (gameData.p2.action == BETRAY)
        { /*Le joueur 2 trahi */
            gameData.bal_p1 -= gameData.p1.bet;
            gameData.bal_p2 -= gameData.p2.bet;
        }
        if(gameData.p2.action == COOP)
        { /*Le joueur 2 se tait */
            gameData.bal_p1 += gameData.p1.bet;
            gameData.bal_p2 -= gameData.p2.bet;
        }
    } 
    else if (gameData.p1.action == COOP)
    { /*Le joueur 1 se tait */
        if (gameData.p2.action == BETRAY)
        { /*Le joueur 2 trahi */
            gameData.bal_p1 -= gameData.p1.bet;
            gameData.bal_p2 += gameData.p2.bet;
        }
        if(gameData.p2.action == COOP)
        { /*Le joueur 2 se tait */
            gameData.bal_p1 -= (gameData.p1.bet / 2);
            gameData.bal_p2 -= (gameData.p2.bet / 2);
        }
    }
    gameData.p1.responded = false;
    gameData.p2.responded = false;
    gameData.currentRound += 1;
    return gameData;
}

int getWinner(GameData gameData)
{
    int idWinner;
    if(gameData.bal_p1 > gameData.bal_p2)
    {
        idWinner = gameData.p1.idClient;
    }
    else if (gameData.bal_p1 < gameData.bal_p2)
    {
        idWinner = gameData.p2.idClient;
    }
    else if (gameData.bal_p1 == gameData.bal_p2)
    {
        idWinner = 0;
    }
    return idWinner;
}

bool isGameFinished(GameData gameData)
{
    bool ret = false;
    if(gameData.totalRounds == gameData.currentRound)
    {
        ret = true;
    }
    return ret;
}
void writeHeader(FILE *file)
{
    file = fopen("results.csv", "a+");
    fprintf(file,"Nom de Room, Round, ID Client, Mise, Choix, Balance actuel\n");
    fclose(file);
}
void writeResults(FILE *file, const char *RoomName, GameData gameData){
    
    file = fopen("results.csv", "a+");

    if(gameData.p1.action = 1){
        fprintf(file, "%s, %d, %d, %d, BETRAY, %d\n", RoomName, gameData.currentRound, gameData.p1.idClient, gameData.p1.bet, gameData.bal_p1);
    } else if(gameData.p1.action = 2){
        fprintf(file, "%s, %d, %d, %d, COOP, %d\n", RoomName, gameData.currentRound, gameData.p1.idClient, gameData.p1.bet, gameData.bal_p1);
    }else{
        fprintf(file, "%s, %d, %d, %d, START, %d\n", RoomName, gameData.currentRound, gameData.p1.idClient, gameData.p1.bet, gameData.bal_p1);
    }

    if(gameData.p2.action = 1){
        fprintf(file, "%s, %d, %d, %d, BETRAY, %d\n", RoomName, gameData.currentRound, gameData.p2.idClient, gameData.p2.bet, gameData.bal_p2);
    } else if(gameData.p2.action = 2){
        fprintf(file, "%s, %d, %d, %d, COOP, %d\n", RoomName, gameData.currentRound, gameData.p2.idClient, gameData.p2.bet, gameData.bal_p2);
    }else{
        fprintf(file, "%s, %d, %d, %d, START, %d\n", RoomName, gameData.currentRound, gameData.p2.idClient, gameData.p2.bet, gameData.bal_p2);
    }
    fclose(file);
}