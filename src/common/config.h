#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include <stdbool.h>

#ifndef THEPRISONER_CONFIG_H
#define THEPRISONER_CONFIG_H
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

#endif /* THEPRISONER_CONFIG_H */