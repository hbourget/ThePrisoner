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
#include "../../common/game.h"
#include "../headers/interface.h"

ClientConfig cfgClient;
int sockfd;
PlayerGameSettings cfgPlayer;

void setCfgPlayer(PlayerGameSettings cfg){
    cfgPlayer = cfg;
}

void on_window_main_destroy() {
    printf("Quitting..\n ");
    gtk_main_quit();
}

//Bouton "Se connecter"
void on_connect_button_click(GtkWidget *widget)
{
    cfgClient = getClientConfig();
    sockfd = getClientSockfd();
    write(sockfd, &cfgClient, sizeof(cfgClient));

    //Desactivation du bouton
    gtk_widget_set_sensitive(widget, FALSE);
}

void updateInterface(PlayerGameSettings cfg){

}
void on_validate_button_click () {
    cfgPlayer.responded = true;
    printf("Bet: %d\n", cfgPlayer.bet);
    printf("Action: %d (1=COOP, 2=BETRAY)\n", cfgPlayer.action);
    write(sockfd, &cfgPlayer, sizeof(cfgPlayer));
}
void on_leave_button_click(GtkWidget *widget){
    printf("Quitting..\n ");
    gtk_main_quit();
}
void on_toggled_cooperate(){
    cfgPlayer.action = COOP;
}
void on_toggled_betray(){
    cfgPlayer.action = BETRAY;
}
void on_toggled_10(){ 
    cfgPlayer.bet = 10;
}
void on_toggled_25(){ 
    cfgPlayer.bet = 25;
}
void on_toggled_50(){ 
    cfgPlayer.bet = 50;
}
void on_toggled_100(){ 
    cfgPlayer.bet = 100;
}
