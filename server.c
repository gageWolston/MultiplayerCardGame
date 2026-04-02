/*********************************************************************************************
 * server.c
 * Multiplayer Card game server that manages TCP connections for 2 players
 * Handles player setup, deck selection, turn-based gameplay, and card effects.
 *
 * Date: 12-8-2025
 * Class: CS3013
 * Professor: Dr. Zhao
 *********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "Cards.h"
#include "GameManager.h"
#include "Player.h"

#define PORT 8081
#define BUFFER_SIZE 2048
#define NUM_CLIENTS 2

typedef enum
{
    PHASE_SETUP,
    PHASE_MATCH
} Phase;

static Phase currentPhase = PHASE_SETUP;

// Active players
Player *currentPlayerPtr = NULL;
Player *opponentPlayerPtr = NULL;
int currentTurn = 0;     // 0 = player1, 1 = player2

//---------------- PROTOTYPES ----------------
int serverStartup(Player players[]);
void serverStart(Player players[]);
void beginMatch(Player players[]);
void handlePlayCommand(Player *player, Player *opponent, int cardIndex);
//--------------------------------------------


int main()
{
    Player players[NUM_CLIENTS];

    if (serverStartup(players) != 0)
    {
        printf("Server startup failed.\n");
        return 1;
    }

    serverStart(players);
    return 0;
}



/********************************************************************************************
 * MAIN SERVER LOOP (AFTER PLAYERS CONNECT)
 ********************************************************************************************/
void serverStart(Player players[])
{
    int running = 1;
    char buffer[BUFFER_SIZE];

    while (running)
    {
        for (int i = 0; i < NUM_CLIENTS; i++)
        {
            int bytes = recv(players[i].socket, buffer, BUFFER_SIZE - 1, MSG_DONTWAIT);

            if (bytes > 0)
            {
                buffer[bytes] = '\0';
                printf("Player %d sent: %s\n", players[i].playerNumber, buffer);

                switch (currentPhase)
                {
                    case PHASE_SETUP:
                    {
                        int allReady = PlayerSetup(players, NUM_CLIENTS, i, buffer);
                        if (allReady)
                        {
                            sendToPlayer(&players[0], "Both players READY. Starting match...\n");
                            sendToPlayer(&players[1], "Both players READY. Starting match...\n");

                            beginMatch(players);
                            currentPhase = PHASE_MATCH;
                        }
                        break;
                    }

                    case PHASE_MATCH:
                    {
                        // ONLY current turn’s player may act
                        if (i != currentTurn)
                        {
                            sendToPlayer(&players[i], "NOT YOUR TURN.\n");
                            break;
                        }
                        if(strncmp(buffer, "DECK", 4) == 0){
                                sendDeck(&players[i]);
                                break;
                        }
                        // Process commands
                        if (strncmp(buffer, "PLAY", 4) == 0)
                        {
                            int index = atoi(buffer + 5);
                            handlePlayCommand(&players[currentTurn], &players[1 - currentTurn], index);

                            // Switch turn
                            currentTurn = 1 - currentTurn;

                            // APPLY EFFECTS AT START OF NEW TURN
                            applyEffects(&players[currentTurn]);

                            sendActiveEffects(&players[currentTurn]);

                            sendToPlayer(&players[currentTurn], "\n***YOUR TURN***\n");
                            sendToPlayer(&players[1 - currentTurn], "\n***OPPONENT TURN***\n");
                            break;
                        }
                        else
                        {
                            sendToPlayer(&players[i], "Invalid command. Use: PLAY <cardIndex>\n");
                        }
                        break;

                    }
                }
            }
            else if (bytes == 0)
            {
                printf("Player %d disconnected.\n", players[i].playerNumber);
                running = 0;
                break;
            }
        }
    }
}
/********************************************************************************************
 * BEGIN MATCH: SEND DECKS + SET FIRST TURN
 ********************************************************************************************/
void beginMatch(Player players[])
{
    currentTurn = 0;               // Player 1 starts
    currentPlayerPtr = &players[0];
    opponentPlayerPtr = &players[1];

    sendDeck(&players[0]);
    sendDeck(&players[1]);

    sendPlayerState(&players[0]);
    sendPlayerState(&players[1]);

    sendToPlayer(&players[0], "YOUR TURN\n");
    sendToPlayer(&players[1], "OPPONENT TURN\n");
}



/********************************************************************************************
 * CARD PLAY HANDLER
 ********************************************************************************************/
void handlePlayCommand(Player *player, Player *opponent, int cardIndex)
{
    if (cardIndex < 0 || cardIndex >= DECK_SIZE)
    {
        sendToPlayer(player, "Invalid card index.\n");
        return;
    }

    Card *card = &player->deck[cardIndex];

    // Check cost
    if (player->resources < card->cost)
    {
        sendToPlayer(player, "Not enough resources!\n");
        return;
    }

    // Pay cost
    player->resources -= card->cost;

    // If multi-turn effect -> addStatusEffect
        if (card->effectTurns > 0)
        {
            // Damage-over-time applies to opponent
            if (card->damage > 0){
                char msg[128];

                addEffect(opponent, card);//adding the overtime effect this turn

                snprintf(msg, sizeof(msg), "%s is doing [%d] damage this turn...\n", card->name, card->damage);
                sendToPlayer(opponent, msg);//telling the oponent what did what damage

            }

            // Healing-over-time applies to self
            if (card->giveHealth > 0){
                char msg[128];

                addEffect(player, card);

                snprintf(msg, sizeof(msg), "%s is giving [%d] health this turn...\n", card->name, card->giveHealth);
                sendToPlayer(opponent, msg);//telling the oponent what did what damage

            }

            if(card->giveResourceAmount > 0){
                char msg[128];

                addEffect(player, card);//adding overtime effect this turn

                snprintf(msg, sizeof(msg), "%s is giving [%d] resources this turn...\n", card->name, card->giveResourceAmount);
                sendToPlayer(opponent, msg);//telling the oponent what did what damage


            }
        }
        else
        {
            // Instant damage
            if (card->damage > 0)
            {
                char msg[128];

                opponent->health -= card->damage;//do damage

                snprintf(msg, sizeof(msg), "%s did [%d] damage!\n", card->name, card->damage);
                sendToPlayer(opponent, msg);//telling the oponent what did what damage

                if (opponent->health < 0) opponent->health = 0;
            }

            // Instant heal
            if (card->giveHealth > 0)
            {
                char msg[128];

                player->health += card->giveHealth;//give healing

                snprintf(msg, sizeof(msg), "%s did [%d] healing!\n", card->name, card->giveHealth);
                sendToPlayer(player, msg); //telling the player what did how much health

                if (player->health > MAX_HEALTH) player->health = MAX_HEALTH;
            }

            // Instant resource gain
            if (card->giveResourceAmount > 0)
            {
                char msg[128];

                player->resources += card->giveResourceAmount;//give resources

                snprintf(msg, sizeof(msg), "%s gave [%d] resources!\n", card->name, card->giveResourceAmount);
                sendToPlayer(player, msg); //telling the player how many resources what card gave them

                if (player->resources > MAX_RESOURCES)
                    player->resources = MAX_RESOURCES;
            }
        }

    // Send updated states
    sendPlayerState(player);
    sendPlayerState(opponent);

    // Check victory
    if (opponent->health <= 0)
    {
        sendToPlayer(player, "YOU WIN!\n");
        sendToPlayer(opponent, "YOU LOSE!\n");
    }
}

/********************************************************************************************
 * SERVER STARTUP — ACCEPT 2 PLAYERS
 ********************************************************************************************/
int serverStartup(Player players[])
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, NUM_CLIENTS) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server running on port %d. Waiting for players...\n", PORT);

    // Accept both players
    for (int i = 0; i < NUM_CLIENTS; i++)
    {
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

        if (new_socket < 0)
        {
            perror("Accept failed");
            continue;
        }

        printf("Player %d connected.\n", i + 1);
        players[i].playerNumber = i + 1;               // Assign player number
        initializePlayer(&players[i], new_socket);
        PlayerMenu(&players[i]);
    }

    close(server_fd);
    return 0;
}
