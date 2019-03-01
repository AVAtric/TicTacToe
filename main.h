/** @brief man.h
 *
 *  Created on: Jan 18, 2019
 *  @Author Mahboobeh Shafiee
 */

#ifndef MAN_H_
#define MAN_H_
/**@brief global variables
 *char p1[20], p2[20]                                                string containers for player names
 *int p1score, p2score, drawscore, menuchoice, boxchoice             int containers for scores and choices
 *char p1symbol = 'X', p2symbol = 'O'                                X and O symbols for players
 *char array[9] = { '1','2','3','4','5','6','7','8','9' }            the tictactoe boards that holds X or O
 *int turn = 1                                                       initialized player1's turn to start game
 *_Bool istherewinner                                                flag that confirms is there a winner currently
 *                                                                   and is used in my checkwinner functions
 *int compmove                                                       marker for comp to choose moves
 *int totalmovecounter                                               container for total moves for draw
 *int compchoice                                                     int container for comp choice
 *int dchoice
 */
 
    #define SYMBOL_1 'X'
    #define SYMBOL_2 'O'

    #define TRUE  1
    #define FALSE 0
    
    #define MAX_PLAYER 2
    #define MAX_NAME_LENGTH 20
    
    #define PLAYER_1 0
    #define PLAYER_2 1
    
    #define WIN_HORIZONTAL 1
    #define WIN_VERTICAL   2
    #define WIN_DIAGONAL   3
    
    typedef struct {
        
        unsigned int games;
        
        unsigned int won;
        unsigned int lost;
        unsigned int draw;
        
        unsigned int count_x;
        unsigned int count_o;
        
        unsigned int count_av_sign;
        
    } player_stat;
    
    typedef struct {
        
        char name[MAX_NAME_LENGTH]; 
        unsigned int score;
                
        char symbol;
        char human;
        
        player_stat statistic;
        unsigned int count_symbol;
        
    } player_dat;
    
    /*
     * Init first and second player
     */
    player_dat player[2];
   
    /*
     * Init. 3x3 game matrix
     */
    char game_map[] = { '1','2','3','4','5','6','7','8','9' };
    
    /*
     * Count draws
     */
    int draw = 0;

    /**
    * Checks if player win the game
    * 
    * @param player_number An integer that contains the player number.
    * @return Returns an integer that contains the result.
    */
    static int hcheckforwin(int player_number);

    /** 
     * @brief hresetboard() is a function to reset the game to beginning
     */
     static void hresetboard(void);

    /**
     * @brief displayboard() is a function to display the board
     */
    static void displayboard(void);

    /**
     * @brief displayscore() is a function to display the score board
     */
    static void displayscore(void);

    /**
     * This function takes the choice and matches it to the proper board box
     * 
     * @param field A integer that contains the field value
     * @param symbol A character that contains the field symbol
     * @return void
     */
     static void setboardwithchoice(int field, char symbol);

    /**
     * @brief hrungetmove() is specifically for a 2 human player
     * game mode that allows for taking of turns and checking the board for a
     * winner after every turn then incrementing the turn variable
     */
    static void hrungetmove(void);

    /**
     * Strategy for the computer when one of the player is computer 
     *
     * @param player_number An integer that contains the player number.
     * @return Returns an integer that contains the map position.
     */
    static int compstrategy(int player_number);
    
    /**
     * @brief This function is for the end of each round.
     * Allows the user to select an option
     */
    static void hmenu(void);
    
    /**
     * @brief mmenu() is a function to the main console shows the game and allows the user to select an option
     * Depending on the user's choice, it is required to run the functions.
     */
    static void mmenu(void);
    
    /**
     * Set up the player properties
     *
     * @param player_number An integer that contains the player number. Use PLAYER_1 or PLAYER_2
     * @param name An character array that contains the player name.
     * @param symbol An character that contains the player symbol.
     * @param score An unsigned integer that contains the player score.
     * @param human An character (boolean) that contains true if human; Otherwise false.
     * @return void
     */
    static void setup_player(int player_number, char name[MAX_NAME_LENGTH], char symbol, unsigned int score, char human);
    
    /**
     * Reads the player name and symbol from console
     *
     * @param player_number An integer that contains the player number.
     * @return void
     */
    static void read_player_name_and_symbol(int player_number);
    
    /**
     * Reads the player choice from console
     *
     * @param player_number An integer that contains the player choice.
     * @return Returns an integer that contains the choosen number or -1 if invalid.
     */
    static int read_player_choice(int player_number);

    /**
     * Write statistic to file
     *
     * @param player_number An integer that contains the player number. Use PLAYER_1 or PLAYER_2
     * @return void
     */
    static void write_statistic(int player_number);
    
    /**
     * Read statistic from file
     *
     * @param player_number An integer that contains the player number. Use PLAYER_1 or PLAYER_2
     * @return void
     */
    static void read_statistic(int player_number);
    
    /**
     * Print statistic to screen
     *
     * @param player_number An integer that contains the player number. Use PLAYER_1 or PLAYER_2
     * @return void
     */
    static void print_statistic(int player_number);
    
    /**
     * Write play field to file
     *
     * @return void
     */
    static void write_playfield(void);
    
#endif /* MAN_H_ */
