#include "ui.h"

void print_game_screen(Player players[], bool reveal_every_players_cards)
{
	system("cls");

	(players[0].wins == 1) ? printf("\n\tplayer 1: 1 win") : printf("\n\tplayer 1: %d wins", players[0].wins);
	(players[1].wins == 1) ? printf("\n\tplayer 2: 1 win") : printf("\n\tplayer 2: %d wins", players[1].wins);

	if (reveal_every_players_cards) print_cards(players[1].cards_in_hand);
	else print_hidden_cards();
	printf("\n");
	print_cards(players[0].cards_in_hand);

	printf("\n\t");
	print_combo_string(players[0].hand_score);
	printf("\n");
}
void print_cards(Card player_cards[])
{
	// example: printing one card (the five of hearts) to the console
	// printf(" _____   ");  line 1
	// printf("|5   H|  ");  line 2, draw number, then symbol
	// printf("|     |  ");  line 3
	// printf("|     |  ");  line 4
	// printf("|H___5|  ");  line 5, draw symbol, then number

	// line 1
	printf("\n\t");
	for (int i = 0; i < _NUMBER_OF_DEALT_CARDS_PER_PLAYER; i++)
	{
		printf(" _____   ");
	}
	printf("\n");

	// line 2
	printf("\t");
	for (int i = 0; i < _NUMBER_OF_DEALT_CARDS_PER_PLAYER; i++)
	{
		if (player_cards[i].face == 9)
		{
			printf("|10  %c|  ", get_card_suit(player_cards[i].suit));
		}
		else
		{
			printf("|%c   %c|  ", get_card_face(player_cards[i].face), get_card_suit(player_cards[i].suit));
		}
	}
	printf("\n");

	// line 3
	printf("\t");
	for (int i = 0; i < _NUMBER_OF_DEALT_CARDS_PER_PLAYER; i++)
	{
		printf("|     |  ");
	}
	printf("\n");

	// line 4
	printf("\t");
	for (int i = 0; i < _NUMBER_OF_DEALT_CARDS_PER_PLAYER; i++)
	{
		printf("|     |  ");
	}
	printf("\n");

	// line 5
	printf("\t");
	for (int i = 0; i < _NUMBER_OF_DEALT_CARDS_PER_PLAYER; i++)
	{
		if (player_cards[i].face == 9) printf("|%c__10|  ", get_card_suit(player_cards[i].suit));
		else printf("|%c___%c|  ", get_card_suit(player_cards[i].suit), get_card_face(player_cards[i].face));
	}
	printf("\n");
}
void print_hidden_cards(void)
{
	// line 1
	printf("\n\t");
	for (int i = 0; i < _NUMBER_OF_DEALT_CARDS_PER_PLAYER; i++)
	{
		printf(" _____   ");
	}
	printf("\n");

	// line 2
	printf("\t");
	for (int i = 0; i < _NUMBER_OF_DEALT_CARDS_PER_PLAYER; i++)
	{
		printf("|\\   /|  ");
	}
	printf("\n");

	// line 3
	printf("\t");
	for (int i = 0; i < _NUMBER_OF_DEALT_CARDS_PER_PLAYER; i++)
	{
		printf("| \\ / |  ");
	}
	printf("\n");

	// line 4
	printf("\t");
	for (int i = 0; i < _NUMBER_OF_DEALT_CARDS_PER_PLAYER; i++)
	{
		printf("| / \\ |  ");
	}
	printf("\n");

	// line 5
	printf("\t");
	for (int i = 0; i < _NUMBER_OF_DEALT_CARDS_PER_PLAYER; i++)
	{
		printf("|/___\\|  ");
	}
	printf("\n");
}
char get_card_suit(int suit_idx)
{
	switch (suit_idx)
	{
	case(0):
		return 'H';
	case(1):
		return 'D';
	case(2):
		return 'C';
	case(3):
		return 'S';
	default:
		return '?';
	}
}
char get_card_face(int face_idx)
{
	switch (face_idx)
	{
	case(0):
		return 'A';
	case(1):
		return '2';
	case(2):
		return '3';
	case(3):
		return '4';
	case(4):
		return '5';
	case(5):
		return '6';
	case(6):
		return '7';
	case(7):
		return '8';
	case(8):
		return '9';
	case(9):
		return "10";
	case(10):
		return 'J';
	case(11):
		return 'Q';
	case(12):
		return 'K';
	default:
		return '?';
	}
}
void print_combo_string(int score)
{
	switch (score)
	{
	case(10):
		printf( "royal flush");
		break;
	case(9):
		printf( "straight flush");
		break;
	case(8):
		printf( "four of a kind");
		break;
	case(7):
		printf( "full house");
		break;
	case(6):
		printf( "flush");
		break;
	case(5):
		printf( "straight");
		break;
	case(4):
		printf( "three of a kind");
		break;
	case(3):
		printf( "two pair");
		break;
	case(2):
		printf( "one pair");
		break;
	case(1):
		printf( "high card");
		break;
	}
}