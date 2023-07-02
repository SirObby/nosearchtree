#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <stdbool.h>
#include <time.h>

#include "algorithm.h"

void algo_failure()
{
    exit(-1);
}

/*

    algorithm

*/

int piece_from_char(char c);

int parse_fen(cur_game_state* state, const char* fen) {
    // split the FEN string into its individual components
    char* fields[6];
    char* token = strtok(fen, " ");
    for (int i = 0; i < 6; i++) {
        if (token == NULL) {
            return -1;  // invalid FEN string
        }
        fields[i] = token;
        token = strtok(NULL, " ");
    }
    
    // parse the piece placement field
    char* placement = fields[0];
    int rank = 7;
    int file = 0;
    for (int i = 0; i < strlen(placement); i++) {
        char c = placement[i];
        if (c == '/') {
            rank--;
            file = 0;
        } else if (c >= '1' && c <= '8') {
            file += c - '0';
        } else {
            int piece = piece_from_char(c);
            if (piece == -1) {
                return -1;  // invalid FEN string
            }
            state->board[rank][file].piece = piece;
            state->board[rank][file].color = (c >= 'a' && c <= 'z') ? 1 : 0;
            file++;
        }
    }
    
    // parse the active color field
    char* active_color = fields[1];
    state->white_to_move = (strcmp(active_color, "w") == 0);
    
    // parse the castling availability field
    char* castling = fields[2];
    state->white_can_castle_kingside = (strchr(castling, 'K') != NULL);
    state->white_can_castle_queenside = (strchr(castling, 'Q') != NULL);
    state->black_can_castle_kingside = (strchr(castling, 'k') != NULL);
    state->black_can_castle_queenside = (strchr(castling, 'q') != NULL);
    
    // parse the en passant target square field
    char* en_passant = fields[3];
    if (strcmp(en_passant, "-") == 0) {
        state->en_passant_file = -1;
        state->en_passant_rank = -1;
    } else {
        state->en_passant_file = en_passant[0] - 'a';
        state->en_passant_rank = en_passant[1] - '1';
    }
    
    // parse the halfmove clock field
    char* halfmove_clock = fields[4];
    state->halfmove_clock = atoi(halfmove_clock);
    
    // parse the fullmove number field
    char* fullmove_number = fields[5];
    state->fullmove_number = atoi(fullmove_number);
    
    return 0;
}

int piece_from_char(char c) {
    switch (c) {
        case 'P': return 1;
        case 'N': return 2;
        case 'B': return 3;
        case 'R': return 4;
        case 'Q': return 5;
        case 'K': return 6;
        case 'p': return 7;
        case 'n': return 8;
        case 'b': return 9;
        case 'r': return 10;
        case 'q': return 11;
        case 'k': return 12;
        default: return -1;  // invalid piece character
    }
}

void create_algorithm(algorithm* a) {
    //a->FEN = "";
    //a->pieces = WHITE;
}

int begin_game(algorithm* a, char *FEN, start_as p) {

    a->FEN = FEN;
    a->pieces = p;
    if(p == WHITE) a->first_move = true; else a->first_move = false;
    
    if(parse_fen(&a->cgs, a->FEN) != 0) algo_failure();


    return 0;
}

int square_from_string(char* str) {
    int file = str[0] - 'a';
    int rank = str[1] - '1';
    return rank * 8 + file;
}

int game_state(char* moves, algorithm* a) {
    char* move = strtok(moves, " ");
    int from = square_from_string(move);
    int to = square_from_string(move + 2);
    int piece = a->cgs.board[from / 8][from % 8].piece;
    int color = a->cgs.board[from / 8][from % 8].color;
    a->cgs.board[from / 8][from % 8].piece = 0;
    a->cgs.board[from / 8][from % 8].color = 0;
    a->cgs.board[to / 8][to % 8].piece = piece;
    a->cgs.board[to / 8][to % 8].color = color;
    return 0; // successful
}

bool is_move_legal(square source, square dest, algorithm* a) {

    // simple algorithm to check whether or not a move the algorithm is going to make is legal or not.
    
    

}

char* think(algorithm* a) {
    
    // NOPE THE ALGORITHM DOES NOT THINK, THE ALGORITHM CONSISTS OF IF STATEMENTS HAHAHA.
    printf("%d %d\n", a->pieces, a->first_move);
    
    /*
    list of prorities, highest = most important.

    3, checkmate,
    2, check,
    1, capture,
    0, push,

    */

    

    int priorities = 3;

    if(a->pieces == WHITE && a->first_move) return "e2e4";

    if(a->pieces == WHITE) {

    }

    if(a->pieces == BLACK) {
        
    }


    return "(null)";
}