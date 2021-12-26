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
#include "../headers/interface.h"
#include "../../common/configurations.h"

GtkBuilder *builder = NULL;
GtkWidget *win;

int main(int argc, char** argv) {
    int status = 0;
    char msg[100];
    pthread_t thread;

    ClientConfig cfgClient = initClientCfg();
    int sockfd = open_connection(cfgClient);
    //Transfert ces deux paramètres au client
    setInterfaceParams(sockfd, cfgClient);

    showClientConfig(cfgClient);

    pthread_create(&thread, 0, threadProcess, &sockfd);
    pthread_detach(thread);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade/dilemme_prisonnier.glade");
    set_builder(builder);

    win = GTK_WIDGET(gtk_builder_get_object(builder, "app_win"));
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(win);
    gtk_main();

    return (EXIT_SUCCESS);
}

