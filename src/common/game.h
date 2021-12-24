#include <stdbool.h>
#include "config.h"
#ifndef GAME_H
#define GAME_H 

//START est l'action par défaut lors de l'initialisation du PlayerGameSettings
enum actions {START, COOP, BETRAY};

/**
* @brief Structure des informations personnels du client propre à sa room actuelle
*
* @param bet Pari (10, 25, 50, 100)
* @param balance Balance du client
* @param action Action du client dans le round (BETRAY / COOP)
* @param idClient ID du client
* @param responded True: le client à envoyer sa struct au serveur, False: n'a pas envoyer sa struct au serveur
*/
typedef struct {
    int bet;
    int balance;
    int idClient;
    enum actions action;
    bool responded;
} PlayerGameSettings;

/**
* @brief Structure des informations de jeu des clients propre à leur room actuelle
*
* @param currentRound Round courant
* @param totalRounds Nombre total de rounds
* @param p1 Structure PlayerGameSettings du premier joueur
* @param p2 Structure PlayerGameSettings du second joueur
*/
typedef struct {
    int currentRound;
    int totalRounds;
    PlayerGameSettings p1;
    PlayerGameSettings p2;
} GameData;

PlayerGameSettings initPlayerGameSettings(ServerConfig cfgServer, int roomID, int idClient);
GameData hydrateGameData(PlayerGameSettings cfgPlayer, GameData gameData, ServerConfig cfgServer, int i);
GameData playRound(GameData gameData);
int getWinner(GameData gameData);
bool isGameFinished(GameData gameData);
#endif /* GAME.H */