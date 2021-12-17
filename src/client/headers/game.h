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

// PlayerGameSettings getGameSettings();

#endif /* GAME_H */