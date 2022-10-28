#include "message.h"

#include <sys/types.h> 
#include <unistd.h>

#include <sys/stat.h>
#include<stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


pid_t pid ;
char pathname[80];
int fifo_serveur_fd;
int fifo_me_fd;

void creation_tube_nomme() {
    pid = getpid(); 
    sprintf(pathname,PATH_FORMAT,pid);
    mkfifo(pathname, 0666); //read & write
}

void envoyer_expression(){
    fifo_serveur_fd = open(FIFO_SERVEUR, O_WRONLY);

    requete_client_serveur_t* requete = malloc(sizeof(requete_client_serveur_t));
    read(0,requete->expression,BUFFER_SIZE-1);

    requete->client_pid = pid;
    
    
    write(fifo_serveur_fd, requete, sizeof(requete_client_serveur_t));

    close(fifo_serveur_fd);
}

void recevoir_resultat(){
    fifo_me_fd = open(pathname, O_RDONLY);

    char resultat[128];
    read(fifo_me_fd, resultat, 128);
    printf("--> %s\n", resultat);
};

void terminer(){
    remove( pathname );
    close(fifo_serveur_fd);
    close(fifo_me_fd);
}



int main(int argc, char** argv) {
    creation_tube_nomme();
    envoyer_expression();
    recevoir_resultat();
    terminer();

    return 0;
}