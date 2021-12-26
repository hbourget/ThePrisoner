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

#include "../headers/client.h"
#include "../../common/configurations.h"
#include "../headers/interface.h"

ClientConfig cfgClient;
int sockfd;
PlayerGameSettings cfgPlayer;
GtkBuilder *build;

/**
 * @brief Set the builder object
 * 
 * @param builder 
 */
void set_builder(GtkBuilder *builder){
    build = builder;
}
void setCfgPlayer(PlayerGameSettings cfg){
    cfgPlayer = cfg;
}

void on_window_main_destroy() {
    printf("Quitting..\n ");
    gtk_main_quit();
}
void set_result(int result){
    GtkLabel *information = GTK_LABEL(gtk_builder_get_object(build, "information_label"));
    if(result == 1){
        gtk_label_set_text(information, "You have won !");
    } else if(result == 2){
        gtk_label_set_text(information, "You have lost !");
    }else{
        gtk_label_set_text(information, "Game is tied !");
    }
}

void set_balance(int balance){
    GtkLabel *balance_label = GTK_LABEL(gtk_builder_get_object(build, "balance_label"));
    char lab[64];
    sprintf(lab, "Initial balance: %d", balance);
    gtk_label_set_text(balance_label, lab);
}

//Bouton "Se connecter"
void on_connect_button_click(GtkWidget *widget)
{
    //Définition d'un pointeur sur le label de connexion
    GtkLabel *information = GTK_LABEL(gtk_builder_get_object(build, "information_label"));

    cfgClient = getClientConfig();
    sockfd = getClientSockfd();

    int id = cfgClient.idClient;
    char base[64];
    sprintf(base, "You are connected ! ID Client : %d", id);

    write(sockfd, &cfgClient, sizeof(cfgClient));

    //Desactivation du bouton
    gtk_widget_set_sensitive(widget, FALSE);
    
    // Mise à jour du label de status de connexion.
    gtk_label_set_text(information, base);
}
void on_validate_button_click () {
    cfgPlayer.responded = true;
    printf("Bet: %d\n", cfgPlayer.bet);
    printf("Action: %d (1=COOP, 2=BETRAY)\n", cfgPlayer.action);
    write(sockfd, &cfgPlayer, sizeof(cfgPlayer));
    cfgPlayer.responded = false;
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
