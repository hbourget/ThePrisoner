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
#include "../headers/interface.h"

void on_window_main_destroy() {
    printf("Quitting..\n ");
    gtk_main_quit();
}
void on_connect_button_click(){
    printf("Interface : %d-%d\n", cfgClient.idClient, sockfd);
    write(sockfd, &cfgClient, sizeof(cfgClient));
}
void on_validate_button_click(){
    printf("sending..");
    gtk_main_quit();
}
void on_leave_button_click(){
    printf("Quitting..\n ");
    gtk_main_quit();
}
