/*
/ Name: Gage Wolston (Rewritten version for multiplayer server integration)
/ Date: December 2025
/
/ What it does:
/  This file now contains ONLY the game logic and rules.
/  It no longer handles input (scanf) or turns directly.
/  The server chooses which player acts and when.
/  GameManager simply applies card effects using Player structs.
/
/  IMPORTANT:
/
/  - The original global variables (health, resources, cardDeck) are removed.
/  - All game logic now receives Player* so server controls state.
/  - This keeps the same structure as the original file but adapted for multiplayer.
/
*/

#include "GameManager.h"
#include "Cards.h"
#include "Player.h"

// ------------------------------------------------------------
// No more global health/resources/cardDeck/playerTurn
// Server now owns and manages Player objects
// ------------------------------------------------------------


// ---------------- HEALTH FUNCTIONS --------------------------

int GetHealth(Player *p)
{
    return p->health;
}

void SetHealth(Player *p, int setHealth)
{
    p->health = setHealth;
}

void AddHealth(Player *p, int addHealth)
{
    if (p->health <= 0)
        return;

    p->health += addHealth;
    if (p->health > MAX_HEALTH)
        p->health = MAX_HEALTH;
}

void RemoveHealth(Player *p, int removeHealth, Card *card)
{
    if (p->health <= 0)
        return;

    p->health -= removeHealth;

    if (p->health < 0)
        p->health = 0;
}


// ---------------- RESOURCE FUNCTIONS -------------------------

int GetResources(Player *p)
{
    return p->resources;
}

void SetResources(Player *p, int resourceAmount)
{
    p->resources = resourceAmount;
}

void AddResources(Player *p, int resourceAmount)
{
    p->resources += resourceAmount;

    if (p->resources > MAX_RESOURCES)
        p->resources = MAX_RESOURCES;
}

void RemoveResources(Player *p, int resourceAmount)
{
    p->resources -= resourceAmount;

    if (p->resources < 0)
        p->resources = 0;
}


// ---------------- CARD COST CHECK ----------------------------

int CheckCost(Player *p, Card *card)
{
    return (p->resources >= card->cost);
}

void RemoveCost(Player *p, Card *card)
{
    p->resources -= card->cost;
    if (p->resources < 0)
        p->resources = 0;
}


// ---------------- CARD APPLICATION LOGIC ---------------------

/*
    PlayCard() is called by server when a player sends "PLAY X"
    This function applies all effects of the chosen card.
*/
void PlayCard(Player *user, Player *opponent, Card *card)
{
    // Damage logic
    if (card->damage > 0)
    {
        opponent->health -= card->damage;
        if (opponent->health < 0)
            opponent->health = 0;
    }

    // Healing logic
    if (card->giveHealth > 0)
    {
        user->health += card->giveHealth;
        if (user->health > MAX_HEALTH)
            user->health = MAX_HEALTH;
    }

    // Resource-gain logic
    if (card->giveResourceAmount > 0)
    {
        user->resources += card->giveResourceAmount;
        if (user->resources > MAX_RESOURCES)
            user->resources = MAX_RESOURCES;
    }
}


// ---------------- DECK BUILDING (unchanged style) -------------

/*
    RandomDeck() kept for compatibility, but server now uses Player.deck[].
    We keep the function because other code may still call it.
*/
void RandomDeck(Player *p, int classType)
{
    // Copy correct deck based on class
    Card *sourceDeck = (classType == 1 ? mageDeck : jackDeck);

    for (int i = 0; i < DECK_SIZE; i++)
        p->deck[i] = sourceDeck[i];
}
