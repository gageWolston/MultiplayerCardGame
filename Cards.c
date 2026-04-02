
/*
/       Name: Gage Wolston
/
/       What it does:
/
/               This file stores all card functions, as well as each deck's array with the structs for each card.
/               This is not my favorite way to fill the card database, as there's values some cards may not even use
/               I hate it. But, this is simple and there is not a need to make it harder than it needs to be.
/
/
/
/       Notes:
/               If you wish to add a new deck or function, don't forget to add it to Cards.h
/
/
/
*/

#include "Cards.h"
#include "GameManager.h"
#include "Player.h"
#include <stdio.h>

// ------------------------------------------------------------
//  CARD DATABASE (AVAILABLE TO ALL PLAYERS)
//  These decks must exist because Player.c loads cardDB[]
// ------------------------------------------------------------

// Basic default deck used before class selection
Card cardDB[DECK_SIZE] = {

    //name, cost, dmg, resource, health, turns
    {"Fireball", 2, 2, 0, 0, 2},
    {"Heal", 1, 0, 0, 2, 1},
    {"Mana Crystal", 0, 0, 2, 0, 1},
    {"Lightning Bolt", 3, 4, 0, 0, 1},
    {"Shield", 1, 0, 0, 2, 2},
    {"Strike", 1, 1, 0, 0, 1},
    {"Heavy Punch", 2, 2, 0, 0, 1},
    {"Greater Heal", 2, 0, 0, 4, 1}
};

// Mage deck
Card mageDeck[DECK_SIZE] = {
    //name,                cost, dmg, res,  heal, turns
    {"Fireball",                2, 2,   0,      0, 2},
    {"Inferno",                 3, 4,   0,      0, 0},
    {"Flame Shield",            1, 1,   0,      2, 0},
    {"Mana Surge",              0, 0,   3,      0, 2},
    {"Burn",                    2, 1,   0,      0, 3},
    {"Arcane Bolt",             2, 2,   0,      0, 0},
    {"Meteor",                  4, 6,   0,      0, 0},
    {"Flame Barrier",   3, 0,   0,      5, 0}
};

// Jack deck
Card jackDeck[DECK_SIZE] = {
    //name,                cost, dmg, res,  heal, turns
    {"Punch",                   1, 1,   0,      0, 0},
    {"Kick",                    2, 2,   0,      0, 0},
    {"Uppercut",                2, 2,   0,      0, 2},
    {"Taunt",                   0, 0,   2,      0, 0},
    {"Block",                   1, 0,   0,      2, 2},
    {"Rage",                    1, 0,   4,      0, 0},
    {"Haymaker",                3, 4,   0,      0, 0},
    {"Focus",                   0, 0,   2,      0, 2}
};

/*
    We NO LONGER use card->Play(card) function pointers.
    The server now uses: PlayCard(player, opponent, card)
    This module simply stores card data.
*/
