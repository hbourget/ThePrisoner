#include <stdbool.h>
#include "config.h"
#ifndef GAME_H
#define GAME_H 

enum actions { SILENCE, COOP, BETRAY };

typedef struct {
    int bet;
    int balance;
    int currentR;
    int totalR;
    enum actions action;
    bool responded;
    int idClient;
} PlayerGameSettings;

typedef struct {
    PlayerGameSettings p1;
    PlayerGameSettings p2;
} GameData;

PlayerGameSettings initPlayerGameSettings(ServerConfig cfgServer, int roomID, int idClient);

bool nextround(PlayerGameSettings p1_gameSettings, PlayerGameSettings p2_gameSettings);

void playRound(PlayerGameSettings p1_gameSettings, PlayerGameSettings p2_gameSettings);

GameData hydrateGameData(PlayerGameSettings cfgPlayer, GameData gameData, ServerConfig cfgServer, int i);

GameData hydrateData(GameData gameData, PlayerGameSettings cfgPlayer);
#endif /* GAME.H */