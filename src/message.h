#include <sys/types.h> 
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>


#define BUFFER_SIZE 20

#define PATH_FORMAT "/tmp/client_%d_fifo"
#define FIFO_SERVEUR "/tmp/serveur_fifo"
#define TEMPLATE_CMD "echo '%s' | bc"

typedef struct requete_client_serveur {
    pid_t client_pid; //PID du Client
    char expression[BUFFER_SIZE -1]; //text de l expression a evaluer.
} requete_client_serveur_t;