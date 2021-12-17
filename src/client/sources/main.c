#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <libconfig.h>

#include "../headers/clientcxnmanager.h"
#include "../headers/config.h"

int main(int argc, char** argv) {
    int sockfd;
    int status = 0;
    char msg[100];
    pthread_t thread;
    ClientConfig cfgClient;

    cfgClient = initCfg();
    
    showConfig(cfgClient);

    sockfd = open_connection(cfgClient);

    //Creation d'un pthread de lecture
    pthread_create(&thread, 0, threadProcess, &sockfd);
    //write(connection->sock,"Main APP Still running",15);
    pthread_detach(thread);
    do {
        fgets(msg, 100, stdin);
        //printf("sending : %s\n", msg);
        status = write(sockfd, msg, strlen(msg));
        //memset(msg,'\0',100);
    } while (status != -1);

    return (EXIT_SUCCESS);
}

