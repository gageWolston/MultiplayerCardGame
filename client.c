/***************************************************************************
* client.c
* TCP client for a multiplayer card game with non-blocking I/O
* Connects to the server and handles asynchrounous send/receive of game
* commands.
*
* Date: 12-8-2025
* Class: CS3013
* Professor: Dr. Zhao
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8081                                       // Port number for server
#define BUFFER_SIZE 1024                                // Size of message buffer


// Function prototypes---------------------------
int clientStartup(const char *server_ip);               // Sets up client connection to serve r
void clientStart(int sock);                            // Main client loop for sending/receiv ing messages
//-----------------------------------------------



int main(int argc, char *argv[])
{
        if (argc != 2)                                                  // Expects one argume nt: server IP
        {
                printf("Usage: %s <server_ip>\n", argv[0]);
                return 1;
        }

        int sock = clientStartup(argv[1]);                              // Start client and c onnect to server

        if(sock < 0)                                                    // Connection failed
        {
                printf("Client Startup Failed.\n");
                return 1;
        }

        printf("Connected to server.\n");                               // Connection success ful
        clientStart(sock);

        close(sock);                                                    // Close socket when  finished
        return 0;
}

void clientStart(int sock)                                              // Main client loop f or sending/receiving messages
{
        char UserInput[BUFFER_SIZE];                                    // Buffer for user in put
        char buffer[BUFFER_SIZE];                                       // Buffer for server  messages

        // Set socket to non-blocking mode
        int flags = fcntl(sock, F_GETFL, 0);
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);

        // Set stdin to non-blocking mode
        int stdin_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, stdin_flags | O_NONBLOCK);

        int bytes = recv(sock, buffer, BUFFER_SIZE - 1, 0);            // Receive first messa ge from server
        if (bytes > 0)
        {
                buffer[bytes] = '\0';                                   // End message with n ull character
                printf("%s", buffer);                                   // Print first messag e from server
        }

        printf("> ");
        fflush(stdout);

        //Main message loop------------------------------
        while (1)
        {
                // Check for server messages
                bytes = recv(sock, buffer, BUFFER_SIZE - 1, 0);
                if (bytes > 0)
                {
                        buffer[bytes] = '\0';
                        printf("\n%s", buffer);
                        printf("> ");
                        fflush(stdout);
                }
                else if (bytes == 0)
                {
                        printf("\nServer closed the connection.\n");
                        break;
                }

                // Check for user input
                if (fgets(UserInput, BUFFER_SIZE, stdin) != NULL)
                {
                        if (send(sock, UserInput, strlen(UserInput), 0) < 0)
                        {
                                perror("Send failed");
                                break;
                        }
                }
                else if (feof(stdin))
                {
                        printf("\nEOF (CTRL+D) detected, terminating connection.\n");
                        break;
                }

                // Small sleep to prevent busy waiting
                usleep(10000);  // 10ms
        }//End main message loop--------------------------

        // Restore blocking mode before exit
        fcntl(STDIN_FILENO, F_SETFL, stdin_flags);
}

int clientStartup(const char *server_ip)                   // Sets up client connection to se rver
{

        int sock;
        struct sockaddr_in serv_addr;


        //Create socket
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                perror("Socket creation failed");
                return -1;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        //Convert server IP address from text to binary
        if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0)
        {
                perror("Invalid address or address not supported");
                return -1;
        }

        //Connect to server
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
                perror("Connection failed");
                return -1;
        }

        return sock;
}
