#include <stdbool.h>
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
/**
* @brief Structure d'une room
*
* @param name Nom de la room
* @param nbRounds Nombre de manches
* @param bank Montant de la balance initiale
* @param idClient_1 ID du premier client attendu
* @param idClient_2 ID du second client attendu
*/
typedef struct {
    GameData gameData;
    const char *name;
    int nbRounds;
    int bank;
    int idClient_1;
    int idClient_2;
} Room;

/**
* @brief Structure de la configuration de la partie (Une partie peut avoir plusieurs rooms)
*
* @param rooms Liste de rooms
* @param nbRooms Nombre de rooms
*/
typedef struct {
    Room rooms[50];
    int nbRooms;
} GameConfig;

/**
* @brief Structure de la configuration du serveur
*
* @param serverIP Adresse IP du serveur
* @param serverPort Port du serveur
* @param gameConfig Configuration de la partie
*/
typedef struct {
    const char *serverIP;
    int serverPort;
    GameConfig gameConfig;
} ServerConfig;

/**
* @brief Structure de la configuration du client
*
* @param serverIP Adresse IP du serveur
* @param serverPort Port du serveur
* @param idClient ID du client
*/
typedef struct {
    char serverIP[15];
    int serverPort;
    int idClient;
} ClientConfig;

ServerConfig initServerCfg();
ClientConfig initClientCfg();
void showServerConfig(ServerConfig cfgServer);
void showRooms(ServerConfig cfgServer);
void showClientConfig(ClientConfig cfgClient);
PlayerGameSettings initPlayerGameSettings(ServerConfig cfgServer, int roomID, int idClient);
GameData firstHydrateGameData(PlayerGameSettings cfgPlayer, GameData gameData, ServerConfig cfgServer, int i);
GameData hydrateGameData(PlayerGameSettings cfgPlayer, GameData gameData, ServerConfig cfgServer, int i);
GameData playRound(GameData gameData);
int getWinner(GameData gameData);
bool isGameFinished(GameData gameData);
void writeResults(FILE *file, const char *RoomName, GameData gameData);
void writeHeader(FILE *file);
#endif /* GAME.H */