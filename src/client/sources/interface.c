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
GtkBuilder *build;

void setCfgPlayer(PlayerGameSettings cfg){
    cfgPlayer = cfg;
}

void on_window_main_destroy() {
    printf("Quitting..\n ");
    gtk_main_quit();
}
/**
 * @brief Set the builder object
 * 
 * @param builder 
 */
void set_builder(GtkBuilder *builder){
    build = builder;
}


/**
 * @brief Ecouteurs d'évènemment sur le bouton de connection.
 * 
 * @param widget 
 */
void on_connect_button_click(GtkWidget *widget)
{
    //Définition d'un pointeur sur le label de connexion
    GtkLabel *status = GTK_LABEL(gtk_builder_get_object(build, "status_label_connection"));
    GtkLabel *statement_label = GTK_LABEL(gtk_builder_get_object(build, "statement_label"));
    GtkLabel *id_client = GTK_LABEL(gtk_builder_get_object(build, "client_id_label"));

    cfgClient = getClientConfig();
    sockfd = getClientSockfd();

    int id = cfgClient.idClient;
    char base[20];
    sprintf(base, "ID Client : %d", id);

    write(sockfd, &cfgClient, sizeof(cfgClient));

    //Desactivation du bouton
    gtk_widget_set_sensitive(widget, FALSE);
    
    // Mise à jour du label de status de connexion.
    gtk_label_set_text(status, "You are connected !");
    gtk_label_set_text(statement_label, "It's up to you!");
    gtk_label_set_text(id_client, base);
}
/**
 * @brief Désactive les checkbuttons de l'interface
 * 
 */
void disable_check_button(){
    // Définition des pointeurs sur les CheckButton.
    GtkCheckButton *betray = GTK_CHECK_BUTTON(gtk_builder_get_object(build, "betra"));
    GtkCheckButton *cooperate = GTK_CHECK_BUTTON(gtk_builder_get_object(build, "ooperate"));

    GtkCheckButton *ten = GTK_CHECK_BUTTON(gtk_builder_get_object(build, "checkbox10"));
    GtkCheckButton *twenty_five = GTK_CHECK_BUTTON(gtk_builder_get_object(build, "checkbox25"));
    GtkCheckButton *fifty = GTK_CHECK_BUTTON(gtk_builder_get_object(build, "checkbox50"));
    GtkCheckButton *one_hundred = GTK_CHECK_BUTTON(gtk_builder_get_object(build, "checkbox100"));

    // Désactivation des boutons.
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(betray), false);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cooperate), false);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ten), false);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(twenty_five), false);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(fifty), false);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(one_hundred), false);
}
/**
 * @brief Permet d'envoyer ces choix au serveur. 
 * 
 */
void on_validate_button_click () {
    GtkLabel *statement_label = GTK_LABEL(gtk_builder_get_object(build, "statement_label"));

    cfgPlayer.responded = true;
    printf("Bet: %d\n", cfgPlayer.bet);
    printf("Action: %d (1=COOP, 2=BETRAY)\n", cfgPlayer.action);
    write(sockfd, &cfgPlayer, sizeof(cfgPlayer));
    cfgPlayer.responded = false;

    gtk_label_set_text(statement_label, "You have played !");
    disable_check_button();
}
void on_leave_button_click(GtkWidget *widget){
    printf("Closing of the application !\n");
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
