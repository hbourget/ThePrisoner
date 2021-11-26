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
} PlayerGameSetting;

PlayerGameSetting *getGameSettings();

void calculateBalance(PlayerGameSetting *p1_gameSettings, PlayerGameSetting *p2_gameSettings);

#endif /* GAME.H */