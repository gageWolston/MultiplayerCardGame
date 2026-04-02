#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Cards.h"

#define DECK_SIZE 8
#define MAX_RESOURCES 10
#define MAX_HEALTH 10

typedef struct Player Player;

// Health functions now take Player*
int GetHealth(Player *p);
void SetHealth(Player *p, int setHealth);
void AddHealth(Player *p, int addHealth);
void RemoveHealth(Player *p, int removeHealth, Card *card);

// Resource functions now take Player*
int GetResources(Player *p);
void SetResources(Player *p, int resourceAmount);
void AddResources(Player *p, int resourceAmount);
void RemoveResources(Player *p, int resourceAmount);

// Cost checking
int CheckCost(Player *p, Card *card);
void RemoveCost(Player *p, Card *card);

// Card playing logic
void PlayCard(Player *user, Player *opponent, Card *card);

// Deck initialization
void RandomDeck(Player *p, int classType);

#endif
