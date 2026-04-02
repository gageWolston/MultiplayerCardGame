/*
 * Player.c
 * Patched for multiplayer server + new GameManager logic
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "Player.h"
#include "Cards.h"
#include "GameManager.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

// ------------------------------------------------------------
// Player Setup
// ------------------------------------------------------------
void initializePlayer(Player *player, int socket)
{
    player->socket = socket;
    player->health = MAX_HEALTH;
    player->resources = MAX_RESOURCES;
    player->playerNumber = 0;       // server assigns number AFTER

    player->isReady = 0;
    player->classType = 0;

    // Load default deck before class selection
    for (int i = 0; i < DECK_SIZE; i++)
        player->deck[i] = cardDB[i];

    memset(player->effects, 0, sizeof(player->effects));
}


// ------------------------------------------------------------
// Communication Helpers
// ------------------------------------------------------------
void sendToPlayer(Player *player, const char *message)
{
    send(player->socket, message, strlen(message), 0);
}


void sendPlayerState(Player *player)
{
    char buffer[BUFFER_SIZE];

    snprintf(buffer, BUFFER_SIZE,
             "\nSTATS| Health:[%d] |Resources: [%d]\n",
             player->health, player->resources);

    send(player->socket, buffer, strlen(buffer), 0);
}


void sendDeck(Player *player)
{
    char buffer[BUFFER_SIZE];
    char temp[256];

    strcpy(buffer, "DECK|\n");  // Start with newline after DECK|

    for (int i = 0; i < DECK_SIZE; i++)
    {
        snprintf(temp, sizeof(temp),
                 "[%d] %-15s | Cost:%d | Dmg:%d | Heal:%d | Res+:%d\n",  // Added newline here
                 i,
                 player->deck[i].name,
                 player->deck[i].cost,
                 player->deck[i].damage,
                 player->deck[i].giveHealth,
                 player->deck[i].giveResourceAmount,
                 player->deck[i].effectTurns);

        strcat(buffer, temp);
    }

    send(player->socket, buffer, strlen(buffer), 0);
}


// ------------------------------------------------------------
// Player Menu + Class Selection
// ------------------------------------------------------------
void PlayerMenu(Player *player)
{
    sendToPlayer(player,
        "Welcome to the Card Game!\n"
        "Choose your class:\n"
        "   1 - Mage\n"
        "   2 - Jack\n"
        "Type READY when finished.\n\n"
    );
}


int PlayerSetup(Player players[], int numPlayers, int playerIndex,
{
    Player *p = &players[playerIndex];

    char *nl = strchr(buffer, '\n');
    if (nl) *nl = '\0';

    // Also remove carriage return if present
    char *cr = strchr(buffer, '\r');
    if (cr) *cr = '\0';

    // CLASS SELECT
    if (strcmp(buffer, "1") == 0)
    {
        p->classType = 1;
        // Load Mage deck
        for (int i = 0; i < DECK_SIZE; i++)
            p->deck[i] = mageDeck[i];
        sendDeck(p);
        sendToPlayer(p, "Mage class selected.\n");
        return 0;
    }
    else if (strcmp(buffer, "2") == 0)
    {
        p->classType = 2;
        // Load Jack deck
        for (int i = 0; i < DECK_SIZE; i++)
            p->deck[i] = jackDeck[i];
        sendDeck(p);
        sendToPlayer(p, "Jack class selected.\n");
        return 0;
    }

    // READY
    if (strcmp(buffer, "READY") == 0)
    {
        if (p->classType == 0)
        {
            sendToPlayer(p, "Choose a class first.\n");
            return 0;
        }

        p->isReady = 1;
        sendToPlayer(p, "You are READY.\n");

        // Check if all players are ready
        for (int i = 0; i < numPlayers; i++)
        {
            if (!players[i].isReady)
                return 0;
        }

        // Everyone ready → return 1
        return 1;
    }

    sendToPlayer(p, "Invalid command.\n");
    return 0;
}


void applyEffects(Player *p)
{
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        if (p->effects[i].turnsRemaining > 0)
        {
            // Damage
            if (p->effects[i].damagePerTurn > 0)
            {
                p->health -= p->effects[i].damagePerTurn;
                if (p->health < 0) p->health = 0;
            }

            // Heal
            if (p->effects[i].healPerTurn > 0)
            {
                p->health += p->effects[i].healPerTurn;
                if (p->health > MAX_HEALTH)
                    p->health = MAX_HEALTH;
            }

            // Resource gain
            if (p->effects[i].resourcePerTurn > 0)
            {
                p->resources += p->effects[i].resourcePerTurn;
                if (p->resources > MAX_RESOURCES)
                    p->resources = MAX_RESOURCES;
            }

            // Countdown
            p->effects[i].turnsRemaining--;

            // Clear effect when done
            if (p->effects[i].turnsRemaining == 0)
                memset(&p->effects[i], 0, sizeof(StatusEffect));
        }
    }
}

void addEffect(Player *p, Card *card)
{
    if(card->effectTurns <= 0)
        return;

    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        if (p->effects[i].turnsRemaining == 0)
        {
            p->effects[i].damagePerTurn = card->damage;
            p->effects[i].healPerTurn = card->giveHealth;
            p->effects[i].resourcePerTurn = card->giveResourceAmoun
            p->effects[i].turnsRemaining = card->effectTurns;
            return;
        }
    }


   sendToPlayer(p, "\nEFFECT LIMIT REACHED! No more effects can be
}

void sendActiveEffects(Player *p)
{
    char buffer[BUFFER_SIZE];
    strcpy(buffer, "\nEFFECTS|\n");

    int found = 0;

    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        if (p->effects[i].turnsRemaining > 0)
        {
            found = 1;

            char line[128];
            snprintf(line, sizeof(line),
                     " - Dmg:[%d] Heal:[%d] Res+:[%d] (turns left:
                     p->effects[i].damagePerTurn,
                     p->effects[i].healPerTurn,
                     p->effects[i].resourcePerTurn,
                     p->effects[i].turnsRemaining);

            strcat(buffer, line);
        }
    }

    if (!found)
        strcat(buffer, " - No active effects.\n");

    sendToPlayer(p, buffer);
}
