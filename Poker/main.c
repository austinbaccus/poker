#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "poker.h"

bool play_again();
void print_game_over_message(int winner);

int main(void)
{
	/* initialize suit array */
	const char *suit[4] = { "Hearts", "Diamonds", "Clubs", "Spades" };

	/* initialize face array */
	const char *face[13] = { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight",
		"Nine", "Ten", "Jack", "Queen", "King" };


	// initializing an array that represents all of the cards in a full deck of cards.
	Card cards[52] = { {0,0} };

	// initializing array of players
	Player players[2] = 
	{ 
		{ {0,0}, 0, 0, false }, 
		{ {0,0}, 0, 0, true } 
	};

	srand((unsigned)time(NULL)); /* seed random-number generator */
	
	do
	{
		int winner = play(players, cards);
		print_game_over_message(winner);
	} while (play_again());

	return 0;
}

bool play_again()
{
	printf("\n\twould you like to play again? enter 'y' if yes, 'n' if no.\n\t");
	char answer = 'n';
	scanf(" %c", &answer);
	return true;
}
void print_game_over_message(int winner)
{
	switch (winner)
	{
	case (-1):
		printf("\n\n\ttie!");
		break;
	case (0):
		printf("\n\n\tyou won!");
		break;
	case (1):
		printf("\n\n\tyou lost!");
		break;
	}
}