#include "message.h"

#include <sys/types.h> 
#include <unistd.h>

#include <sys/stat.h>
#include<stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int fifo_me;
requete_client_serveur_t* requete;

void creation_tube_nomme() {
    mkfifo(FIFO_SERVEUR ,0666);//read & write
}

void recevoir_requete(){
    fifo_me = open(FIFO_SERVEUR ,O_RDONLY);
    requete = malloc(sizeof(requete_client_serveur_t));
    read(fifo_me, requete, sizeof(requete_client_serveur_t));
}

int resultat(char* result) {
    int pipe_bc[2];
    int pipe_resultat[2];
    pipe(pipe_bc);
    pipe(pipe_resultat);
    
    pid_t pid_bc = fork();
    if(pid_bc==0){//fils
        dup2(pipe_bc[0],0);
        dup2(pipe_resultat[1],1);

        execlp("bc","bc",NULL);
    }
    else{//père
        write(pipe_bc[1], requete->expression, strlen(requete->expression));
        
        int n = read(pipe_resultat[0],result,128);
        result[n-1]= '\0';
        
        kill(pid_bc,9);
        waitpid(pid_bc,NULL,0);
        return n;
    }
}

void envoyer_resultat(){
    char* fifo_client_path = malloc(sizeof(char) * 128);
    sprintf(fifo_client_path, PATH_FORMAT, requete->client_pid);
    int fifo_client = open(fifo_client_path,O_WRONLY);

    char* result = malloc(sizeof(char) * 128);
    write(fifo_client,result,resultat(result));
    
    close(fifo_client);
    free(fifo_client_path);
    free(result);    
}

void terminer(){
    close(fifo_me);
    remove(FIFO_SERVEUR);
}

int main(int argc, char** argv) {

    creation_tube_nomme();

    while(1){
        recevoir_requete();
        envoyer_resultat();
        
        free(requete);
    }

    terminer();
}