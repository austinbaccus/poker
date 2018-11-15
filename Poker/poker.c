
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <ctype.h>
#include <math.h>
#include "poker.h"
#include "ui.h"

/// Returns: the index of the winner in the parameter "players". A return value of
/// 1 would mean that players[1] is the winner of the game.
int play(Player players[2], Card cards[])
{
	int wins_per_player[2] = { 0, 0 }; // an array to keep track of each player's number of wins.
	for (int round = 1; round <= 10; round++) // the rubric says to play 10 rounds per game.
	{
		wins_per_player[start_new_round(players, cards)]++; // start a new round of five-card-draw, and increment the resulting winning player's number of wins.
		players[0].wins = wins_per_player[0];
		players[1].wins = wins_per_player[1];
	}
	if (wins_per_player[0] > wins_per_player[1]) return 0; // player 2 wins.
	else if (wins_per_player[0] < wins_per_player[1]) return 1; // player 1 wins.
	else return -1; // tie.
}

int start_new_round(Player players[2], Card cards[])
{
	// an array that keeps track of which cards are on the table, or which cards are no longer in the deck.
	int dealt_card_indexes[52] = { 0 };

	// initializing all values within the array to a value that represents an undealt card.
	initialize_dealt_cards(dealt_card_indexes);

	// shuffling all cards
	shuffle(cards);

	#pragma region deal cards
	deal(cards, players, dealt_card_indexes);
	players[0].hand_score = evaluate_hand(players[0]);
	players[1].hand_score = evaluate_hand(players[1]);
	print_game_screen(players, false);
	#pragma endregion

	#pragma region switch out cards
	draw(cards, players, dealt_card_indexes);
	players[0].hand_score = evaluate_hand(players[0]);
	players[1].hand_score = evaluate_hand(players[1]);
	// switch out the a.i.'s cards
	#pragma endregion

	#pragma region determine round winner
	print_game_screen(players, true);
	int winner = determine_winner(players);
	switch (winner)
	{
	case (-1):
		printf("\n\ttie! split the pot.");
		break;
	case (0):
		printf("\n\tyou won that hand.");
		break;
	case (1):
		printf("\n\tyou lost that hand.");
		break;
	}
	//Sleep(2000);
	printf("\n\tpress 'enter' to continue: ");
	getchar();
	getchar();
	return winner;
	#pragma endregion
}

void initialize_dealt_cards(int * dealt_card_indexes)
{
	for (int i = 0; i < _NUMBER_OF_CARDS_TOTAL; i++)
	{
		dealt_card_indexes[i] = _UNDEALT_CARD_VALUE;
	}
}
void shuffle(Card cards[])
{
	// unshuffle 
	for (int i = 0; i < _NUMBER_OF_CARDS_TOTAL; i++)
	{
		cards[i].suit = -1;
		cards[i].face = -1;
	}

	// shuffle
	for (int i = 0; i < _NUMBER_OF_SUITS; i++)
	{
		for (int j = 0; j < _NUMBER_OF_CARDS_PER_SUIT; j++)
		{
			int index = -1;
			do
			{
				index = rand() % _NUMBER_OF_CARDS_TOTAL;
			} while (cards[index].face != -1);

			cards[index].suit = i;
			cards[index].face = j;
		}
	}
}

void deal(Card cards[], Player players[], int * dealt_card_indexes)
{
	for (int player_idx = 0; player_idx < 2; player_idx++)
	{
		for (int i = 0; i < _NUMBER_OF_DEALT_CARDS_PER_PLAYER; i++)
		{
			int undealt_card_value = _UNDEALT_CARD_VALUE;
			int card_index = -1;

			do
			{
				card_index = rand() % _NUMBER_OF_CARDS_TOTAL;
			} while (dealt_card_indexes[card_index] != undealt_card_value);
			// For whatever reason, C throws a compiler error when I use " do ... while (dDealt_cards_indexes[card_index] != _UNDEALT_CARD_VALUE)". 
			// I'm replacing _UNDEALT_CARD_VALUE with a local variable which equals _UNDEALT_CARD_VALUE.

			// updating array that keeps track of whcih cards are on the table, or which cards are no longer in the deck
			dealt_card_indexes[card_index] = _DEALT_CARD_VALUE;

			players[player_idx].cards_in_hand[i] = cards[card_index];
		}
	}
}
void draw(Card cards[], Player players[], int * dealt_card_indexes)
{
	printf("\n\tchoose which cards to replace\n\t");
	char c[256] = "";
	scanf(" %s", &c);
	replace_cards(c, cards, players[0].cards_in_hand, 0, dealt_card_indexes);
}
void replace_cards(char input[256], Card cards[], Player players[], int player_index, int * dealt_card_indexes)
{
	for (int i = 0; i < 256 && input[i] != '\0'; i++)
	{
		if (input[i] >= '1' && input[i] <= '5')
		{
			char c = input[i];
			int a = c - '0';
			int idx = a - 1;

			int undealt_card_value = _UNDEALT_CARD_VALUE;
			int card_index = -1;

			do
			{
				card_index = rand() % _NUMBER_OF_CARDS_TOTAL;
			} 
			while (dealt_card_indexes[card_index] != undealt_card_value);

			dealt_card_indexes[card_index] = _DEALT_CARD_VALUE;

			players[player_index].cards_in_hand[idx] = cards[card_index];
		}
	}
}

/// Returns: an arbitrary value based on how strong a hand is. 
/// Also determines and assigns the player's high card value (on a scale from 0-12), just in case we need to determine a winner of each player has the same hand-rank
/// Return values: 
///		10 - royal flush
///      9 - straight flush
///		 8 - 4 of a kind
///      7 - full house
///		 6 - flush
///      5 - straight
///		 4 - 3 of a kind
///      3 - two pair
///		 2 - one pair
///      1 - high card
int evaluate_hand(Player player)
{
	player.high_card = -1;
	int instances_of_each_suit[4] = { 0,0,0,0 };
	int instances_of_each_face[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int number_of_pairs = 0;

	for (int i = 0; i < _NUMBER_OF_DEALT_CARDS_PER_PLAYER; i++)
	{
		instances_of_each_suit[player.cards_in_hand[i].suit]++;
		instances_of_each_face[player.cards_in_hand[i].face]++;
	}

	// has flush?
	bool is_flush = false;
	if (instances_of_each_suit[0] == 5 ||
		instances_of_each_suit[1] == 5 ||
		instances_of_each_suit[2] == 5 ||
		instances_of_each_suit[3] == 5)
	{
		is_flush = true;
	}

	// has straight?
	bool is_straight = false;
	for (int i = 1; i < _NUMBER_OF_CARDS_PER_SUIT - 5; i++)
	{
		if (instances_of_each_face[i + 0] == 1 &&
			instances_of_each_face[i + 1] == 1 &&
			instances_of_each_face[i + 2] == 1 &&
			instances_of_each_face[i + 3] == 1 &&
			instances_of_each_face[i + 4] == 1)
		{
			is_straight = true;
			if (player.high_card == -1)
			{
				player.high_card = (i == 0) ? 0 : i + 4;
			}
			break;
		}
	}

	// is royal?
	bool is_royal = false;
	if (instances_of_each_face[0]  == 1 &&
		instances_of_each_face[10] == 1 &&
		instances_of_each_face[11] == 1 &&
		instances_of_each_face[12] == 1 &&
		instances_of_each_face[13] == 1)
	{
		is_royal = true;
		if (player.high_card == -1) player.high_card = 0; // high card is an ace
	}

	// has n of a kind?
	bool has_four_of_a_kind = false;
	bool has_three_of_a_kind = false;
	bool has_two_of_a_kind = false;
	for (int i = _NUMBER_OF_CARDS_PER_SUIT - 1; i >= 0; i--)
	{
		if (instances_of_each_face[i] == 4)
		{
			has_four_of_a_kind = true;
			if (player.high_card == -1) player.high_card = i;
		}
		if (instances_of_each_face[i] == 3)
		{
			has_three_of_a_kind = true;
			if (player.high_card == -1) player.high_card = i;
		}
		if (instances_of_each_face[i] == 2)
		{
			has_two_of_a_kind = true;
			number_of_pairs = number_of_pairs + 1;
			if (player.high_card == -1) player.high_card = i;
		}
	}

	// what's the high card?
	if (player.high_card == -1) // if the high card is not already determined
	{
		for (int i = _NUMBER_OF_CARDS_PER_SUIT - 1; i >= 0; i--)
		{
			if (instances_of_each_face != 0)
			{
				player.high_card = i;
				break;
			}
		}
	}

	//	10 - royal flush
	if (is_royal && is_flush)
	{
		return 10;
	}
	//  9 - straight flush
	if (is_flush && is_straight)
	{
		return 9;
	}
	//	8 - 4 of a kind
	if (has_four_of_a_kind)
	{
		return 8;
	}
	//  7 - full house
	if (has_three_of_a_kind && has_two_of_a_kind)
	{
		return 7;
	}
	//	6 - flush
	if (is_flush)
	{
		return 6;
	}
	//  5 - straight
	if (is_straight)
	{
		return 5;
	}
	//	4 - 3 of a kind
	if (has_three_of_a_kind)
	{
		return 4;
	}
	//  3 - two pair
	if (number_of_pairs == 2)
	{
		return 3;
	}
	//	2 - one pair
	if (number_of_pairs == 1)
	{
		return 2;
	}
	//  1 - high card
	return  1;
	
}
void sort_hand(Player player)
{
	// sort by suit, then by face
}
int determine_winner(Player players[])
{
	// update each player's hand score value. 
	players[0].hand_score = evaluate_hand(players[0]);
	players[1].hand_score = evaluate_hand(players[1]);

	// if both players have the same combo...
	if (players[0].hand_score == players[1].hand_score)
	{
		for (int i = 0; i < _NUMBER_OF_DEALT_CARDS_PER_PLAYER; i++)
		{
			if (players[0].cards_in_hand[i].face == 0) players[0].high_card == 15; // high ace
			if (players[1].cards_in_hand[i].face == 0) players[1].high_card == 15; // high ace

			if (players[0].cards_in_hand[i].face > players[0].high_card) players[0].high_card = players[0].cards_in_hand[i].face;
			if (players[1].cards_in_hand[i].face > players[1].high_card) players[1].high_card = players[1].cards_in_hand[i].face;
		}

		if (players[0].high_card == players[1].high_card)
		{
			return -1; // tie! 
		}

		return (players[0].high_card > players[1].high_card) ? 0 : 1;
	}
	// return the index of the player with the best hand
	return (players[0].hand_score > players[1].hand_score) ? 0 : 1;
}