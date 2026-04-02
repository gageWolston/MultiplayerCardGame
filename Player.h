/***************************************************************************
* Player.h
*
*
* Author: Tyler Meredith
* Date: 12-8-2025
* Class: CS3013
* Professor: Dr. Zhao
***************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include "Cards.h"
#include "GameManager.h"

/*-------------------------------------------------
PLAYER STRUCTURE
      socket: network connection for player
      health: current health of player
   resources: current resource points (mana)
        deck: Deck loaded from cardBD
PlayerNumber: 1 or 2 depending on connection order
-------------------------------------------------*/
#define MAX_EFFECTS 10

typedef struct StatusEffect{

        int damagePerTurn;
        int healPerTurn;
        int resourcePerTurn;
        int turnsRemaining;

} StatusEffect;

typedef struct Player
{
    int socket;
    int health;
    int resources;
    Card deck[DECK_SIZE];
    int playerNumber;

    int isReady;
    int classType;

    StatusEffect effects[MAX_EFFECTS];
} Player;
//-------------------------------------------------

void initializePlayer(Player *player, int socket);                                  // Initializes player structure (health, resources, deck, etc.)
void sendToPlayer(Player *player, const char *message);                             // Sends a message to a specific player
void sendPlayerState(Player *player);                                               // Sends current game-state (health/resources) to a player
void sendDeck(Player *player);                                                      // Sends player's deck to them

void PlayerMenu(Player *player);                                                    // Displays player menu options
int PlayerSetup(Player players[], int numPlayers, int playerIndex, char *buffer);   // Handles player setup commands


//turn based effects
void applyEffects(Player *p);                      // Applies all active effects at start of turn
void addEffect(Player *p, Card *card);             // Adds a new multi-turn effect from a card
void sendActiveEffects(Player *p);                 // Sends list of current effects to player


#endif
