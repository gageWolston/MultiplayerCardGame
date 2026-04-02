/*
,/ Name: Gage Wolston
/ Date: November 18th, 2025
/
/ What it does:
/   Header file for card definitions.
/   Cards now only store data — no function pointers or card-specific logic.
/   All gameplay effects are handled by GameManager + Server.
*/

#ifndef CARDS_H
#define CARDS_H

#define DECK_SIZE 8

#include <stdio.h>

// Base card structure
typedef struct Card {
    char *name;               // Card name
    int cost;                 // Resource cost
    int damage;               // Damage dealt
    int giveResourceAmount;   // Resources gained
    int giveHealth;           // Healing amount
    int effectTurns;         //         <----------NEW CODE GAGE
} Card;

// Global card arrays (defined in Cards.c)
extern Card cardDB[];
extern Card mageDeck[];
extern Card jackDeck[];

#endif
