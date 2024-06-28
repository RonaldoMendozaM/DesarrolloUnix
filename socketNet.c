#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define MAX_CONEXIONES 30
#define TAMANO_BUFFER 1024

void* handle_client(void* arg);
void sigint_handler(int sig);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
        return 1;
    }

    int puerto = atoi(argv[1]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Creación de socket fallida");
        return 1;
    }

    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(puerto);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Enlace fallido");
        return 1;
    }

    if (listen(sockfd, MAX_CONEXIONES) < 0) {
        perror("Escucha fallida");
        return 1;
    }

    signal(SIGINT, sigint_handler);

    while (1) {
        struct sockaddr_in cliaddr = {0};
        socklen_t clilen = sizeof(cliaddr);
        int connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
        if (connfd < 0) {
            perror("Aceptación fallida");
            continue;
        }

        pthread_t thread;
        int* connfd_ptr = malloc(sizeof(int));
        *connfd_ptr = connfd;
        pthread_create(&thread, NULL, handle_client, (void*)connfd_ptr);
    }

    return 0;
}

int sshdRunning() {
    FILE *fp;
    char buffer[1024];

    fp = popen("ps ax | grep sshd", "r");
    if (fp == NULL) {
        return -1;
    }

    while (fgets(buffer, 1024, fp)!= NULL) {
        if (strstr(buffer, "sshd")!= NULL) {
            pclose(fp);
            return 1; 
        }
    }

    pclose(fp);
    return 0; 
}

void* handle_client(void* arg) {
    int* connfd_ptr = (int*)arg;
    int connfd = *connfd_ptr;
    free(connfd_ptr);

    char buffer[TAMANO_BUFFER];

    while (1) {
        ssize_t bytes_read = read(connfd, buffer, TAMANO_BUFFER);
        if (bytes_read <= 0) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        printf("Mensaje recibido: %s\n", buffer);

        if (strcmp(buffer, "getInfo") == 0) {
            const char* respuesta = "Servicio de Mike, v0.1";
            write(connfd, respuesta, strlen(respuesta));
        }else if (strcmp(buffer, "getNumberOfPartitions") == 0) {
            FILE* fp = popen("lsblk -l | grep part | wc -l", "r");
            char respuesta[10];
            fgets(respuesta, sizeof(respuesta), fp);
            pclose(fp);
            write(connfd, respuesta, strlen(respuesta));
        }else if (strcmp(buffer, "getCurrentKernelVersion") == 0) {
            char respuesta[20];
            FILE* fp = popen("uname -r", "r");
            fgets(respuesta, sizeof(respuesta), fp);
            pclose(fp);
            write(connfd, respuesta, strlen(respuesta));
        }else if (strcmp(buffer, "sshdRunning") == 0) {
            int respuesta = sshdRunning();
            char respuesta_str[10];
            sprintf(respuesta_str, "%d", respuesta);
            write(connfd, respuesta_str, strlen(respuesta_str)); 
        }else {
            const char* respuesta = "Comando inválido";
            write(connfd, respuesta, strlen(respuesta));
        }
    }

    close(connfd);
    return NULL;
}

void sigint_handler(int sig) {
    printf("Se recibió SIGINT, saliendo...\n");
    exit(0);
}