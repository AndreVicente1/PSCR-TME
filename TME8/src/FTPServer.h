#pragma once


#include <thread>
#include <unistd.h>
#include <cstring> 
#include "ServerSocket.h"
#include "ConnectionHandler.h"



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

void handle_client(int client_socket, char* server_directory) {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Lecture de la commande envoyée par le client
    bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0) return;

    buffer[bytes_read] = '\0';

    if (strncmp(buffer, "LIST", 4) == 0) {
        DIR *dir;
        struct dirent *entry;
        dir = opendir(server_directory);
        if (dir == NULL) {
            perror("opendir");
            return;
        }

        while ((entry = readdir(dir)) != NULL) {
            send(client_socket, entry->d_name, strlen(entry->d_name), 0);
            send(client_socket, "\n", 1, 0);
        }

        closedir(dir);
    }
    // ... [Début de la fonction handle_client]

    if (strncmp(buffer, "UPLOAD", 6) == 0) {
        char file_name[BUFFER_SIZE];
        int file_size;
        
        // Recevoir le nom du fichier et sa taille
        recv(client_socket, file_name, BUFFER_SIZE, 0);
        recv(client_socket, &file_size, sizeof(file_size), 0);
        file_size = ntohl(file_size); // Conversion en format host

        char file_path[1024];
        sprintf(file_path, "%s/%s", server_directory, file_name);

        FILE *file = fopen(file_path, "wb");
        if (file == NULL) {
            perror("fopen");
            return;
        }

        char file_buffer[BUFFER_SIZE];
        int bytes_received, total_bytes_received = 0;

        while (total_bytes_received < file_size) {
            bytes_received = recv(client_socket, file_buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) {
                break;
            }
            fwrite(file_buffer, 1, bytes_received, file);
            total_bytes_received += bytes_received;
        }

        fclose(file);
    }
    // ... [Suite de la fonction handle_client]

    if (strncmp(buffer, "DOWNLOAD", 8) == 0) {
        char file_name[BUFFER_SIZE];
        recv(client_socket, file_name, BUFFER_SIZE, 0);

        char file_path[1024];
        sprintf(file_path, "%s/%s", server_directory, file_name);

        FILE *file = fopen(file_path, "rb");
        if (file == NULL) {
            perror("fopen");
            return;
        }

        fseek(file, 0, SEEK_END);
        int file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Envoyer la taille du fichier au client
        int converted_file_size = htonl(file_size);
        send(client_socket, &converted_file_size, sizeof(converted_file_size), 0);

        char file_buffer[BUFFER_SIZE];
        int bytes_read;

        while ((bytes_read = fread(file_buffer, 1, BUFFER_SIZE, file)) > 0) {
            send(client_socket, file_buffer, bytes_read, 0);
        }

        fclose(file);
    }

// ... [Fin de la fonction handle_client]


}


int main (int argc, char ** argv){

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <server_directory>\n", argv[0]);
        exit(1);
    }


    int port = atoi(argv[1]);
    char* server_directory = argv[2];

    //socket
    int client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);


    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    //bind

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_socket);
        exit(1);
    }
    
    //listen
    if (listen(server_socket, 10) < 0) {
        perror("listen");
        close(server_socket);
        exit(1);
    }

    //accept

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }

        handle_client(client_socket, server_directory);
        close(client_socket);
    }
    close(server_socket);
    return 0;
}