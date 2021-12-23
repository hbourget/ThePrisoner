#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <libconfig.h>
#include <gtk/gtk.h>

#include "../headers/clientcxnmanager.h"
#include "../../common/config.h"
#include "../../common/game.h"
#include "../headers/interface.h"

ClientConfig cfgClient;
int sockfd;
PlayerGameSettings cfgPlayer;
RoundChoice roundChoice;

void setCfgPlayer(PlayerGameSettings cfg){
    cfgPlayer = cfg;
}
PlayerGameSettings getCfgPlayer(){
    return cfgPlayer;
}
void on_window_main_destroy() {
    printf("Quitting..\n ");
    gtk_main_quit();
}
void on_connect_button_click(){
    cfgClient = getClientConfig();
    sockfd = getClientSockfd();
    write(sockfd, &cfgClient, sizeof(cfgClient));
}
void updateInterface(PlayerGameSettings cfg){

}
void on_validate_button_click () {
    printf("Mise choisi %d", roundChoice.bet);
    cfgPlayer.action = roundChoice.action;
    cfgPlayer.bet = roundChoice.bet;
    write(sockfd, &cfgPlayer, sizeof(cfgPlayer));
}
void on_leave_button_click(GtkWidget *widget){
    printf("Quitting..\n ");
    gtk_main_quit();
}
void on_toggled_cooperate(){
    roundChoice.action = COOP;
}
void on_toggled_betray(){
    roundChoice.action = BETRAY;
}
void on_toggled_10(){ 
    roundChoice.bet = 10;
}
void on_toggled_25(){ 
    roundChoice.bet = 25;
}
void on_toggled_50(){ 
    roundChoice.bet = 50;
}
void on_toggled_100(){ 
    roundChoice.bet = 100;
}
