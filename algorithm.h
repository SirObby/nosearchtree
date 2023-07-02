#if !defined(_H_ALGORITHM)
#define _H_ALGORITHM

#include <stdbool.h>
#include <stdint.h>

// internal end function, this is incase there is some sort of error case I find while coding.
void algo_failure(); 

typedef enum {
    WHITE,
    BLACK
} start_as;

typedef enum {
    PAWN,
    ROOK,
    BISHOP,
    KNIGHT,
    KING,
    QUEEN
} piece;

typedef struct {
    uint8_t piece : 4;  // 4 bits for the piece type (0-12)
    uint8_t color : 1;  // 1 bit for the color (0 = white, 1 = black)
} square;

typedef struct {
    square board[8][8];
    bool white_to_move;
    bool white_can_castle_kingside;
    bool white_can_castle_queenside;
    bool black_can_castle_kingside;
    bool black_can_castle_queenside;
    int en_passant_file;
    int en_passant_rank;
    int halfmove_clock;
    int fullmove_number;
} cur_game_state;

// WORSE THAN USELESS STRUCT?
/*typedef struct
{
    // set by input
    char *starting_position;
    char *end_position;
    piece p;
    // set by algo
    bool check;
    bool check_mate;
} move;*/

typedef struct
{
    char *FEN;
    start_as pieces;
    char *moves; // in format of "e2e4 e7e5"
    // automatically given to us by our magnificent lichess gameState event :)
    bool first_move;
    cur_game_state cgs;
} algorithm;

/**
 * @brief Create a algorithm object
 * 
 * @param a algorithm pointer
 */
void create_algorithm(algorithm* a);

/**
 * @brief Create a game
 * 
 * @param a algorithm pointer
 * @param FEN FEN string
 * @param p pieces algorithm plays at
 * @return int succesful?
 */
int begin_game(algorithm* a, char *FEN, start_as p);


int game_state(char* moves, algorithm* a);

/**
 * @brief The algorithm's think function. Called after it's the AI's turn. Can not be cancelled after start.
 * 
 * @param a Algorithm object
 * @return char* returns best move.
 */
char* think(algorithm* a);

#endif // _H_ALGORITHM
