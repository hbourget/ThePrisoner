#ifndef GAME_H
#define GAME_H 

enum actions
{ 
    SILENCE, COOP, BETRAY 
};

typedef struct {
    int bet;
    int balance;
    int currentR;
    int totalR;
    enum actions action;
} PlayerGameSettings;

PlayerGameSettings *getGameSettings();

void calculateBalance(PlayerGameSettings *p1_gameSettings, PlayerGameSettings *p2_gameSettings);

bool nextround(PlayerGameSettings *p1_gameSettings, PlayerGameSettings *p2_gameSettings);

#endif /* GAME.H */