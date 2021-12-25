#include <stdbool.h>
#include "config.h"
#ifndef GAME_H
#define GAME_H 

//START est l'action par défaut lors de l'initialisation du PlayerGameSettings
enum actions {START, BETRAY, COOP};

/**
* @brief Structure des informations personnels du client propre à sa room actuelle
*
* @param bet Pari (10, 25, 50, 100)
* @param action Action du client dans le round (BETRAY / COOP)
* @param idClient ID du client
* @param responded True: le client à envoyer sa réponse au serveur, False: n'a pas envoyer sa réponse au serveur
*/
typedef struct {
    int bet;
    int idClient;
    enum actions action;
    bool responded;
} PlayerGameSettings;

/**
* @brief Structure des informations de jeu des clients propre à leur room actuelle
*
* @param currentRound Round courant
* @param totalRounds Nombre total de rounds
* @param bal_p1 Balance du premier joueur
* @param bal_p2 Balance du second joueur
* @param p1 Structure PlayerGameSettings du premier joueur
* @param p2 Structure PlayerGameSettings du second joueur
*/
typedef struct {
    int currentRound;
    int totalRounds;
    int bal_p1;
    int bal_p2;
    PlayerGameSettings p1;
    PlayerGameSettings p2;
} GameData;

PlayerGameSettings initPlayerGameSettings(ServerConfig cfgServer, int roomID, int idClient);
GameData firstHydrateGameData(PlayerGameSettings cfgPlayer, GameData gameData, ServerConfig cfgServer, int i);
GameData hydrateGameData(PlayerGameSettings cfgPlayer, GameData gameData, ServerConfig cfgServer, int i);
GameData playRound(GameData gameData);
int getWinner(GameData gameData);
bool isGameFinished(GameData gameData);
void writeResults(const char *RoomName, GameData gameData);
void writeSeparator();
#endif /* GAME.H */