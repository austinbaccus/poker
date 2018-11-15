#pragma once

#include "poker.h"
#include <time.h>

void print_game_screen(Player players[], bool reveal_every_players_cards);
void print_cards(Card player_cards[]);
void print_hidden_cards(void);
char get_card_suit(int suit_idx);
char get_card_face(int face_idx);
char print_combo_string(int score);