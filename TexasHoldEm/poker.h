#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define _CRT_SECURE_NO_WARNINGS

#define _NUMBER_OF_SUITS 4
#define _NUMBER_OF_CARDS_TOTAL 52
#define _NUMBER_OF_CARDS_PER_SUIT 13
#define _NUMBER_OF_DEALT_CARDS_PER_PLAYER 5

#define _UNDEALT_CARD_VALUE -1;
#define _DEALT_CARD_VALUE 1;

typedef struct card
{
	int suit;
	int face;
} Card;

typedef struct player
{
	Card cards_in_hand[5];
	int high_card;
	int hand_score;
	bool is_computer;
	int wins;
} Player;

int play(Player players[2], Card cards[]);
int start_new_round(Player players[2], Card cards[]);
void initialize_dealt_cards(int * dealt_card_indexes);
void shuffle(Card cards[]);
void deal(Card cards[], Player players[], int * dealt_card_indexes);
void draw(Card cards[], Player players[], int * dealt_card_indexes);
void replace_cards(char input[256], Card cards[], Player players[], int player_index, int * dealt_card_indexes);
int evaluate_hand(Player player); 
void sort_hand(Player player);
int determine_winner(Player players[]);