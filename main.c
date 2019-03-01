/**
 * @file tictac7.c
 * @author mahboobeh shafiee
 * @date 21-01-2019
 * Version 1.0
 * @brief This is the main c file for the tic tac toe program
 * it contains all function definitions and globals as well as the main function
 * use this file with man.h to create the tic tac toe program.
 *
 * USE AT YOUR OWN RISK
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "main.h"


/**
 * Display the game board
 */
void displayboard() {

    printf( "\n           T!C-t@c-TOe Mahboobeh\n");
    printf ("           ~~~~~~~~~~\n");
    printf ("          [ %c | %c | %c ] \n",game_map[0], game_map[1], game_map[2]);
    printf ("          [ %c | %c | %c ] \n",game_map[3], game_map[4], game_map[5]);
    printf ("          [ %c | %c | %c ] \n\n",game_map[6], game_map[7], game_map[8]);
}

/**
 * This function takes the choice and matches it to the proper board box
 * 
 * @param field A integer that contains the field value
 * @param symbol A character that contains the field symbol
 * @return void
 */
void setboardwithchoice(int field, char symbol) {

    // Error handling - Out of range
    if(field < 0 || field > 8) {
        fprintf(stderr, "An error occurred. Argument out of range error.");
        exit(1);
    }

    // Error handling - Invalid symbol
    if(symbol != SYMBOL_1 && symbol != SYMBOL_2) {
        fprintf(stderr, "An error occurred. Invalid symbol error.");
        exit(1);
    }

    game_map[field] = symbol;

    return;
}
/**
 *display my scoreboard
 */
void displayscore()
{
    printf("                    SCORE\n");
    printf("                    ~~~~~\n");
    printf("%s DRAW %s " , player[PLAYER_1].name, player[PLAYER_2].name);
    printf("    %d   %d     %d   \n \n", player[PLAYER_1].score, draw, player[PLAYER_2].score);
}

/**
 * @brief called it hrun getmove because it is specifically for a 2 human player
 * game mode that allows for taking of turns and checking the board for a
 * winner after every turn then incrementing the turn variable
 *
 * 1 => call the displayboard function 
 * 2 => Starter play select one of the boxes 1 to 9 (boxchoice).
 * 3 => If select the duplicate box , it will give an error message and the loop will be repeated.
 * 4 => call the setboardwithchoice function with boxchoice argument.
 * 5 => Adds the totalmovecounteralm variable to one.
 * 6 => call hcheckforwin function
 * do 1 to 8 until istherewinner==0
 */
void hrungetmove()
{
    int mplayer = rand() % 2; // Random beginner
    int boxchoice = -1;
    int totalmovecounter = 0;
    int istherewinner = 0;

    player[mplayer].count_symbol = 0;

    displayboard();

    do {


        if(player[mplayer].human) {

            do{

                boxchoice = read_player_choice(mplayer);
            } while (boxchoice == -1);

            if (game_map[boxchoice] == SYMBOL_1 ||
                game_map[boxchoice] == SYMBOL_2) {

                printf("\nAlready used! Re-enter: ");
                continue;
            }
        } else {

            boxchoice = compstrategy(mplayer);
            printf("%s choice: %d\n", player[mplayer].name, boxchoice + 1);
        }

        setboardwithchoice(boxchoice, player[mplayer].symbol);

        // For average statistic
        player[mplayer].count_symbol ++;

        totalmovecounter++;
        mplayer = !mplayer;

        displayboard();

        istherewinner = hcheckforwin(!mplayer);

    } while (totalmovecounter < 9 && !istherewinner);

    displayboard();

    // Double check the winner
    if (istherewinner) {

        printf(  "%s is the WINNER with a %s win!\n\n", player[!mplayer].name, (istherewinner == 1 ? "horizontal" : (istherewinner == 2 ? " vertical" : "diagonal")));

        player[!mplayer].score++;

        // For statistic won/lost
        player[!mplayer].statistic.won++;
        player[mplayer].statistic.lost++;

    }
    else {

        draw++;

        // For statistic draw/draw
        player[!mplayer].statistic.draw++;
        player[mplayer].statistic.draw++;
    }

    // Update and write statistic

    if(player[mplayer].symbol == SYMBOL_1) {

        player[mplayer].statistic.count_x++;
        player[!mplayer].statistic.count_o++;
    }
    else {

        player[mplayer].statistic.count_o++;
        player[!mplayer].statistic.count_x++;
    }

    player[!mplayer].statistic.games++;
    player[mplayer].statistic.games++;

    // Average player one and two
    if (!player[mplayer].statistic.count_av_sign) {

        player[mplayer].statistic.count_av_sign = player[mplayer].count_symbol;
    }
    else {

        player[mplayer].statistic.count_av_sign = (player[mplayer].statistic.count_av_sign + player[mplayer].count_symbol) / 2;
    }

    if (!player[!mplayer].statistic.count_av_sign) {

        player[!mplayer].statistic.count_av_sign = player[!mplayer].count_symbol;
    }
    else {

        player[!mplayer].statistic.count_av_sign = (player[!mplayer].statistic.count_av_sign + player[!mplayer].count_symbol) / 2;
    }

    write_statistic(!mplayer); write_statistic(mplayer);

    // Write playfield
    write_playfield();

    // Display result
    displayscore();

    // Display statistic
    print_statistic(mplayer); print_statistic(!mplayer);


    hmenu();

    return;
}

/**
 * Reads the player choice from console
 *
 * @param player_number An integer that contains the player choice.
 * @return Returns an integer that contains the choosen number or -1 if invalid.
 */
int read_player_choice(int player_number) {

    // Error handling
    if (player_number >= MAX_PLAYER ) {

        fprintf(stderr, "An error occurred. Maximal player error.");
        exit(1);
    }

    printf( "%s which numbered box do you choose? [1-9] ", player[player_number].name );

    int number = getchar() - 48 - 1; while(getchar() != '\n') {}

    if (number < 0 || number > 8) {

        number = -1;
    }

    return number;
}

/**
 * Checks if player win the game
 * 
 * @param player_number An integer that contains the player number.
 * @return Returns an integer that contains the result.
 */
int hcheckforwin(int player_number) {

    // Error handling
    if (player_number >= MAX_PLAYER ) {

        fprintf(stderr, "An error occurred. Maximal player error.");
        exit(1);
    }

    /*
     * Row 1 => [X|X|X]/[0|0|0] => WON or Row 2 => [X|X|X]/[0|0|0] => WON or Row 3 => [X|X|X]/[0|0|0] => WON
     */
    int horizontal =
            ((game_map[0] == player[player_number].symbol) && (game_map[1] == player[player_number].symbol) && (game_map[2] == player[player_number].symbol)) ||
            ((game_map[3] == player[player_number].symbol) && (game_map[4] == player[player_number].symbol) && (game_map[5] == player[player_number].symbol)) ||
            ((game_map[6] == player[player_number].symbol) && (game_map[7] == player[player_number].symbol) && (game_map[8] == player[player_number].symbol));

    /*
     *             [X|/[O|                       |X|/|O|                       |X]/|O]
     * Column 1 => [X|/[O| => WON or Column 2 => |X|/|O| => WON or Column 3 => |X]/|O] => WON
     *             [X|/[O|                       |X|/|O|                       |X]/|O]
     */
    int vertical   =
            ((game_map[0] == player[player_number].symbol) && (game_map[3] == player[player_number].symbol) && (game_map[6] == player[player_number].symbol)) ||
            ((game_map[1] == player[player_number].symbol) && (game_map[4] == player[player_number].symbol) && (game_map[7] == player[player_number].symbol)) ||
            ((game_map[2] == player[player_number].symbol) && (game_map[5] == player[player_number].symbol) && (game_map[8] == player[player_number].symbol));

    /*
     *           [X|?|?]/[O|?|?]                     [?|?|X]/[?|?|O]
     * Diag 1 => [?|X|?]/[?|O|?] => WON or Diag 2 => [?|X|?]/[?|O|?] => WON
     *           [?|?|X]/[?|?|O]                     [X|?|?]/[O|?|?]
     */
    int diagonal   =
            ((game_map[0] == player[player_number].symbol) && (game_map[4] == player[player_number].symbol) && (game_map[8] == player[player_number].symbol)) ||
            ((game_map[2] == player[player_number].symbol) && (game_map[4] == player[player_number].symbol) && (game_map[6] == player[player_number].symbol));

    int result = 0;

    if (horizontal) {result = WIN_HORIZONTAL; }
    if (vertical) {result = WIN_VERTICAL; }
    if (diagonal) {result = WIN_DIAGONAL; }

    return result;
}

/**
 * Strategy for the computer when one of the player is computer 
 *
 * @param player_number An integer that contains the player number.
 * @return Returns an integer that contains the map position.
 */
int compstrategy(int player_number) {

    // Error handling
    if (player_number >= MAX_PLAYER ) {

        fprintf(stderr, "An error occurred. Maximal player error.");
        exit(1);
    }

    // Defense
    // Horizontal
    for(int i = 0; i < 3; i++) {

        // Row 1 - 3 [X|X|.] or [0|0|.] => [X|X|0] or [0|0|X]
        if(game_map[0 + 3 * i] == player[!player_number].symbol && game_map[1 + 3 * i] == player[!player_number].symbol &&
           game_map[2 + 3 * i] != SYMBOL_1 && game_map[2 + 3 * i] != SYMBOL_2) {

            return 2 + 3 * i;
        }

        // Row 1 - 3 [.|X|X] or [.|0|0] => [0|X|X] or [X|0|0]
        if(game_map[1 + 3 * i] == player[!player_number].symbol && game_map[2 + 3 * i] == player[!player_number].symbol &&
           game_map[0 + 3 * i] != SYMBOL_1 && game_map[0 + 3 * i] != SYMBOL_2) {

            return 0 + 3 * i;
        }

        // Row 1 - 3 [X|.|X] or [0|.|0] => [X|0|X] or [0|X|0]
        if(game_map[0 + 3 * i] == player[!player_number].symbol && game_map[2 + 3 * i] == player[!player_number].symbol &&
           game_map[1 + 3 * i] != SYMBOL_1 && game_map[1 + 3 * i] != SYMBOL_2) {

            return 1 + 3 * i;
        }
    }

    // Vertical
    for(int i = 0; i < 3; i++) {

        // Column 1 - 3 [X|    [0|    [X|    [0|
        //              [X| or [0| => [X| or [0|
        //              [.|    [.|    [0|    [X|
        if(game_map[0 + i] == player[!player_number].symbol && game_map[3 + i] == player[!player_number].symbol &&
           game_map[6 + i] != SYMBOL_1 && game_map[6 + i] != SYMBOL_2) {

            return 6 + i;
        }

        // Column 1 - 3 [.|    [.|    [0|    [X|
        //              [X| or [0| => [X| or [0|
        //              [X|    [0|    [X|    [0|
        if(game_map[3 + i] == player[!player_number].symbol && game_map[6 + i] == player[!player_number].symbol &&
           game_map[0 + i] != SYMBOL_1 && game_map[0 + i] != SYMBOL_2) {

            return 0 + i;
        }

        // Column 1 - 3 [X|    [0|    [X|    [0|
        //              [.| or [.| => [0| or [X|
        //              [X|    [0|    [X|    [0|
        if(game_map[0 + i] == player[!player_number].symbol && game_map[6 + i] == player[!player_number].symbol &&
           game_map[3 + i] != SYMBOL_1 && game_map[3 + i] != SYMBOL_2) {

            return 3 + i;
        }
    }

    // Diagonal

    // Col/Row 1 - 3 [X|?|?]    [0|?|?]    [X|?|?]    [0|?|?]
    //               [?|X|?] or [?|0|?] => [?|X|?] or [?|0|?]
    //               [?|?|.]    [?|?|.]    [?|?|0]    [?|?|X]
    if(game_map[0] == player[!player_number].symbol && game_map[4] == player[!player_number].symbol &&
       game_map[8] != SYMBOL_1 && game_map[8] != SYMBOL_2) {

        return 8;
    }

    // Col/Row 1 - 3 [.|?|?]    [.|?|?]    [0|?|?]    [X|?|?]
    //               [?|X|?] or [?|0|?] => [?|X|?] or [?|0|?]
    //               [?|?|X]    [?|?|0]    [?|?|X]    [?|?|0]
    if(game_map[4] == player[!player_number].symbol && game_map[8] == player[!player_number].symbol &&
       game_map[0] != SYMBOL_1 && game_map[0] != SYMBOL_2) {

        return 0;
    }

    // Col/Row 1 - 3 [X|?|?]    [0|?|?]    [X|?|?]    [0|?|?]
    //               [?|.|?] or [?|.|?] => [?|0|?] or [?|X|?]
    //               [?|?|X]    [?|?|0]    [?|?|X]    [?|?|0]
    if(game_map[0] == player[!player_number].symbol && game_map[8] == player[!player_number].symbol &&
       game_map[4] != SYMBOL_1 && game_map[4] != SYMBOL_2) {

        return 4;
    }


    // Col/Row 1 - 3 [?|?|X]    [?|?|0]    [?|?|X]    [?|?|0]
    //               [?|X|?] or [?|0|?] => [?|X|?] or [?|0|?]
    //               [.|?|?]    [.|?|?]    [0|?|?]    [X|?|?]
    if(game_map[2] == player[!player_number].symbol && game_map[4] == player[!player_number].symbol &&
       game_map[6] != SYMBOL_1 && game_map[6] != SYMBOL_2) {

        return 6;
    }

    // Col/Row 1 - 3 [?|?|.]    [?|?|.]    [?|?|0]    [?|?|X]
    //               [?|X|?] or [?|0|?] => [?|X|?] or [?|0|?]
    //               [X|?|?]    [0|?|?]    [X|?|?]    [0|?|?]
    if(game_map[4] == player[!player_number].symbol && game_map[6] == player[!player_number].symbol &&
       game_map[2] != SYMBOL_1 && game_map[2] != SYMBOL_2) {

        return 2;
    }

    // Col/Row 1 - 3 [?|?|X]    [?|?|0]    [?|?|X]    [?|?|0]
    //               [?|.|?] or [?|.|?] => [?|0|?] or [?|X|?]
    //               [X|?|?]    [0|?|?]    [X|?|?]    [0|?|?]
    if(game_map[2] == player[!player_number].symbol && game_map[6] == player[!player_number].symbol &&
       game_map[4] != SYMBOL_1 && game_map[4] != SYMBOL_2) {

        return 4;
    }

    // Attack
    // Horizontal
    for(int i = 0; i < 3; i++) {

        // Row 1 - 3 [X|X|.] or [0|0|.] => [X|X|X] or [0|0|0]
        if(game_map[0 + 3 * i] == player[player_number].symbol && game_map[1 + 3 * i] == player[player_number].symbol &&
           game_map[2 + 3 * i] != SYMBOL_1 && game_map[2 + 3 * i] != SYMBOL_2) {

            return 2 + 3 * i;
        }

        // Row 1 - 3 [.|X|X] or [.|0|0] => [X|X|X] or [0|0|0]
        if(game_map[1 + 3 * i] == player[player_number].symbol && game_map[2 + 3 * i] == player[player_number].symbol &&
           game_map[0 + 3 * i] != SYMBOL_1 && game_map[0 + 3 * i] != SYMBOL_2) {

            return 0 + 3 * i;
        }

        // Row 1 - 3 [X|.|X] or [0|.|0] => [X|X|X] or [0|0|0]
        if(game_map[0 + 3 * i] == player[player_number].symbol && game_map[2 + 3 * i] == player[player_number].symbol &&
           game_map[1 + 3 * i] != SYMBOL_1 && game_map[1 + 3 * i] != SYMBOL_2) {

            return 1 + 3 * i;
        }
    }

    // Vertical
    for(int i = 0; i < 3; i++) {

        // Column 1 - 3 [X|    [0|    [X|    [0|
        //              [X| or [0| => [X| or [0|
        //              [.|    [.|    [X|    [0|
        if(game_map[0 + i] == player[player_number].symbol && game_map[3 + i] == player[player_number].symbol &&
           game_map[6 + i] != SYMBOL_1 && game_map[6 + i] != SYMBOL_2) {

            return 6 + i;
        }

        // Column 1 - 3 [.|    [.|    [X|    [0|
        //              [X| or [0| => [X| or [0|
        //              [X|    [0|    [X|    [0|
        if(game_map[3 + i] == player[player_number].symbol && game_map[6 + i] == player[player_number].symbol &&
           game_map[0 + i] != SYMBOL_1 && game_map[0 + i] != SYMBOL_2) {

            return 0 + i;
        }

        // Column 1 - 3 [X|    [0|    [X|    [0|
        //              [.| or [.| => [X| or [0|
        //              [X|    [0|    [X|    [0|
        if(game_map[0 + i] == player[player_number].symbol && game_map[6 + i] == player[player_number].symbol &&
           game_map[3 + i] != SYMBOL_1 && game_map[3 + i] != SYMBOL_2) {

            return 3 + i;
        }
    }

    // Diagonal

    // Col/Row 1 - 3 [X|?|?]    [0|?|?]    [X|?|?]    [0|?|?]
    //               [?|X|?] or [?|0|?] => [?|X|?] or [?|0|?]
    //               [?|?|.]    [?|?|.]    [?|?|X]    [?|?|0]
    if(game_map[0] == player[player_number].symbol && game_map[4] == player[player_number].symbol &&
       game_map[8] != SYMBOL_1 && game_map[8] != SYMBOL_2) {

        return 8;
    }

    // Col/Row 1 - 3 [.|?|?]    [.|?|?]    [X|?|?]    [0|?|?]
    //               [?|X|?] or [?|0|?] => [?|X|?] or [?|0|?]
    //               [?|?|X]    [?|?|0]    [?|?|X]    [?|?|0]
    if(game_map[4] == player[player_number].symbol && game_map[8] == player[player_number].symbol &&
       game_map[0] != SYMBOL_1 && game_map[0] != SYMBOL_2) {

        return 0;
    }

    // Col/Row 1 - 3 [X|?|?]    [0|?|?]    [X|?|?]    [0|?|?]
    //               [?|.|?] or [?|.|?] => [?|X|?] or [?|0|?]
    //               [?|?|X]    [?|?|0]    [?|?|X]    [?|?|0]
    if(game_map[0] == player[player_number].symbol && game_map[8] == player[player_number].symbol &&
       game_map[4] != SYMBOL_1 && game_map[4] != SYMBOL_2) {

        return 4;
    }

    // Col/Row 1 - 3 [?|?|X]    [?|?|0]    [?|?|X]    [?|?|0]
    //               [?|X|?] or [?|0|?] => [?|X|?] or [?|0|?]
    //               [.|?|?]    [.|?|?]    [X|?|?]    [0|?|?]
    if(game_map[2] == player[player_number].symbol && game_map[4] == player[player_number].symbol &&
       game_map[6] != SYMBOL_1 && game_map[6] != SYMBOL_2) {

        return 6;
    }

    // Col/Row 1 - 3 [?|?|.]    [?|?|.]    [?|?|X]    [?|?|0]
    //               [?|X|?] or [?|0|?] => [?|X|?] or [?|0|?]
    //               [X|?|?]    [0|?|?]    [X|?|?]    [0|?|?]
    if(game_map[4] == player[player_number].symbol && game_map[6] == player[player_number].symbol &&
       game_map[2] != SYMBOL_1 && game_map[2] != SYMBOL_2) {

        return 2;
    }

    // Col/Row 1 - 3 [?|?|X]    [?|?|0]    [?|?|X]    [?|?|0]
    //               [?|.|?] or [?|.|?] => [?|X|?] or [?|0|?]
    //               [X|?|?]    [0|?|?]    [X|?|?]    [0|?|?]
    if(game_map[2] == player[player_number].symbol && game_map[6] == player[player_number].symbol &&
       game_map[4] != SYMBOL_1 && game_map[4] != SYMBOL_2) {

        return 4;
    }

    // Default: If no strategy then random position 
    int pos;

    do {

        pos = rand() % 9;

    }while(game_map[pos] == SYMBOL_1 || game_map[pos] == SYMBOL_2); // Check free position

    return pos;
}


/**@brief This function is for the end of each round.
 * Allows the user to select an option
 * 1=> Play again
 * 2=> Go to the main menu
 * 3=> Get out of the game
 */
void hmenu()
{
    printf ( "           1.Play Again?\n");
    printf ( "           2.Main Menu\n");
    printf ( "           3.Quit\n");

    int menuchoice = getchar(); while(getchar() != '\n') {}

    switch (menuchoice)
    {
        case '1':
            hresetboard();
            hrungetmove();

            break;

        case '2':
            hresetboard();
            mmenu();
            break;

        case '3':
        default:
            exit(0);
            break;
    }
}

/**
 * The main console shows the game and allows the user to select an option
 *
 * Depending on the user's choice, it is required to run the functions.
 */
void mmenu() {

    printf ( "          Welcome to Mahboobeh's Console\n              T!C-t@c-TOe\n\n");
    printf ( "          Main Menu\n");
    printf ( "          ~~~~~~~~~\n");
    printf ( "          1. Players 1 vs Player 2\n");
    printf ( "          2. Player vs Computer\n");
    printf ( "          3. Computer vs Computer\n");
    printf ( "          4. How to Play T!C-t@c-TOe?\n");
    printf ( "          5. Quit\n\n");

    hresetboard();

    int menuchoice = getchar(); while(getchar() != '\n') {} // Clear buffer

    switch (menuchoice) {

        case '1':
            read_player_name_and_symbol(PLAYER_1);
            read_player_name_and_symbol(PLAYER_2);
            hrungetmove();

            break;

        case '2':
            read_player_name_and_symbol(PLAYER_1);
            setup_player(PLAYER_2, "Computer\n", (player[PLAYER_1].symbol == SYMBOL_1 ? SYMBOL_2 : SYMBOL_1), 0, FALSE);
            hrungetmove();

            break;

        case '3':
            setup_player(PLAYER_1, "Computer 1\n", SYMBOL_1, 0, FALSE);
            setup_player(PLAYER_2, "Computer 2\n", SYMBOL_2, 0, FALSE);
            hrungetmove();

            break;

        case '4':
            printf ( "TAKE TURNS, GET 3 OF YOUR SYMBOLS IN A ROW AND YOU WIN! SIMPLE!\n");
            mmenu();
            break;

        case '5':
        default:
            exit(0);
            break;
    }
}

/**
 * Reads the player name and symbol from console
 *
 * @param player_number An integer that contains the player number.
 * @return void
 */
void read_player_name_and_symbol(int player_number) {

    // Error handling
    if (player_number >= MAX_PLAYER ) {

        fprintf(stderr, "An error occurred. Maximal player error.");
        exit(1);
    }

    char buf[MAX_NAME_LENGTH + 1] = { '\0' };
    int chr = (player_number == 1 ? (player[!player_number].symbol == SYMBOL_1 ? '1' : '0')  : '0');

    printf ( "Player %d: What is your nickname? ", player_number + 1);
    // Reads string and error handling
    if (fgets(buf, MAX_NAME_LENGTH, stdin) == NULL) {

        fprintf(stderr, "An error occurred. Error while read nickname.");
        exit(1);
    }

    if (player_number < 1) {

        printf ( "Player %d: What is your symbol? [0...'%c' (default), 1...'%c'] ", player_number + 1, SYMBOL_1, SYMBOL_2);
        chr = getchar(); while(getchar() != '\n') {} // Clear buffer

    }
    setup_player(player_number, buf, (chr == '1' ? SYMBOL_2 : SYMBOL_1), 0, TRUE);

    return;
}

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
void setup_player(int player_number, char name[MAX_NAME_LENGTH], char symbol, unsigned int score, char human) {

    // Error handling
    if (player_number >= MAX_PLAYER ) {

        fprintf(stderr, "An error occurred. Maximal player error.");
        exit(1);
    }

    strncpy(player[player_number].name, name, strlen(name) - 1);    // Remove last '\n' from name
    player[player_number].symbol = symbol;
    player[player_number].score = score;
    player[player_number].human = human;

    // Check statistic file
    if( access( player[player_number].name, F_OK ) != -1 ) {

        read_statistic(player_number);
    }
    else {

        /* Init. statistic 'games' */
        player[player_number].statistic.games = 0;

        /* Init. statistic 'won' */
        player[player_number].statistic.won = 0;

        /* Init statistic 'lost' */
        player[player_number].statistic.lost = 0;

        /* Init. statistic 'draw' */
        player[player_number].statistic.draw = 0;

        /* Init statistic 'X' */
        player[player_number].statistic.count_x = 0;

        /* Init. statistic 'O' */
        player[player_number].statistic.count_o = 0;

        /* Init. statistic 'Average' */
        player[player_number].statistic.count_av_sign = 0;
    }

    return;
}

/**
 * Write statistic to file
 *
 * @param player_number An integer that contains the player number. Use PLAYER_1 or PLAYER_2
 * @return void
 */
void write_statistic(int player_number) {

    // Error handling
    if (player_number >= MAX_PLAYER ) {

        fprintf(stderr, "An error occurred. Maximal player error.");
        exit(1);
    }

    FILE *fp = fopen( player[player_number].name, "w");

    if (fp == NULL) {

        fprintf(stderr, "An error occurred. Error while opening or creating file!\n");
        exit(1);
    }

    /* write statistic 'games' */
    fprintf(fp, "games: %d\n", player[player_number].statistic.games);
    /* write statistic 'won' */
    fprintf(fp, "won: %d\n", player[player_number].statistic.won);
    /* write statistic 'lost' */
    fprintf(fp, "lost: %d\n", player[player_number].statistic.lost);
    /* write statistic 'draw' */
    fprintf(fp, "draw: %d\n", player[player_number].statistic.draw);
    /* write statistic 'X' */
    fprintf(fp, "X: %d\n", player[player_number].statistic.count_x);
    /* write statistic 'O' */
    fprintf(fp, "O: %d\n", player[player_number].statistic.count_o);
    /* write statistic 'Average' */
    fprintf(fp, "Average: %d\n", player[player_number].statistic.count_av_sign);

    fclose(fp);

    return;
}

/**
 * Read statistic from file
 *
 * @param player_number An integer that contains the player number. Use PLAYER_1 or PLAYER_2
 * @return void
 */
void read_statistic(int player_number) {

    // Error handling
    if (player_number >= MAX_PLAYER ) {

        fprintf(stderr, "An error occurred. Maximal player error.");
        exit(1);
    }

    FILE *fp = fopen( player[player_number].name, "r");

    if (fp == NULL) {

        fprintf(stderr, "An error occurred. Error while opening or creating file!\n");
        exit(1);
    }

    char txt[10] = { '\0'};
    int val = 0;

    switch(fscanf(fp, "%s %d", txt, &val)) {

        case 2:
            /* write statistic 'games' */
            player[player_number].statistic.games = val;
            break;

        case EOF:
            return;

        default:
            fprintf(stderr, "An error occurred. Error while reading file!\n");
            exit(1);
    }

    switch(fscanf(fp, "%s %d", txt, &val)) {

        case 2:
            /* write statistic 'won' */
            player[player_number].statistic.won = val;
            break;

        case EOF:
            return;

        default:
            fprintf(stderr, "An error occurred. Error while reading file!\n");
            exit(1);
    }

    switch(fscanf(fp, "%s %d", txt, &val)) {

        case 2:
            /* write statistic 'lost' */
            player[player_number].statistic.lost = val;
            break;

        case EOF:
            return;

        default:
            fprintf(stderr, "An error occurred. Error while reading file!\n");
            exit(1);
    }

    switch(fscanf(fp, "%s %d", txt, &val)) {

        case 2:
            /* write statistic 'draw' */
            player[player_number].statistic.draw = val;
            break;

        case EOF:
            return;

        default:
            fprintf(stderr, "An error occurred. Error while reading file!\n");
            exit(1);
    }

    switch(fscanf(fp, "%s %d", txt, &val)) {

        case 2:
            /* write statistic 'X' */
            player[player_number].statistic.count_x = val;
            break;

        case EOF:
            return;

        default:
            fprintf(stderr, "An error occurred. Error while reading file!\n");
            exit(1);
    }

    switch(fscanf(fp, "%s %d", txt, &val)) {

        case 2:
            /* write statistic 'O' */
            player[player_number].statistic.count_o = val;
            break;

        case EOF:
            return;

        default:
            fprintf(stderr, "An error occurred. Error while reading file!\n");
            exit(1);
    }

    switch(fscanf(fp, "%s %d", txt, &val)) {

        case 2:
            /* write statistic 'Average' */
            player[player_number].statistic.count_av_sign = val;
            break;

        case EOF:
            return;

        default:
            fprintf(stderr, "An error occurred. Error while reading file!\n");
            exit(1);
    }

    return;
}

/**
 * Print statistic to screen
 *
 * @param player_number An integer that contains the player number. Use PLAYER_1 or PLAYER_2
 * @return void
 */
void print_statistic(int player_number) {

    // Error handling
    if (player_number >= MAX_PLAYER ) {

        fprintf(stderr, "An error occurred. Maximal player error.");
        exit(1);
    }

    printf("Statistic %s\n", player[player_number].name);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    /* write statistic 'games' */
    printf("Games: %d\n", player[player_number].statistic.games);
    /* write statistic 'won' */
    printf("Won: %d\n", player[player_number].statistic.won);
    /* write statistic 'lost' */
    printf("Lost: %d\n", player[player_number].statistic.lost);
    /* write statistic 'draw' */
    printf("Draw: %d\n", player[player_number].statistic.draw);
    /* write statistic 'X' */
    printf("X: %d\n", player[player_number].statistic.count_x);
    /* write statistic 'O' */
    printf("O: %d\n", player[player_number].statistic.count_o);
    /* write statistic 'Average' */
    printf("Average: %d\n\n", player[player_number].statistic.count_av_sign);

    return;
}

/**
 * Write play field to file
 *
 * @return void
 */
void write_playfield() {

    FILE *fp = fopen( "playfield", "a");

    if (fp == NULL) {

        fprintf(stderr, "An error occurred. Error while opening or creating file!\n");
        exit(1);
    }

    fprintf(fp, "\n           T!C-t@c-TOe Mahboobeh\n");
    fprintf(fp, "           ~~~~~~~~~~\n");
    fprintf(fp, "          [ %c | %c | %c ] \n",game_map[0], game_map[1], game_map[2]);
    fprintf(fp, "          [ %c | %c | %c ] \n",game_map[3], game_map[4], game_map[5]);
    fprintf(fp, "          [ %c | %c | %c ] \n\n",game_map[6], game_map[7], game_map[8]);

    fprintf(fp, "Player 1: %s\n", player[PLAYER_1].name);
    fprintf(fp, "Player 2: %s\n\n", player[PLAYER_2].name);

    fclose(fp);

    return;
}

/**
 * Resets the matrix 9*9 of the game.
 */
void hresetboard() {

    // Setup game map from 1 to 9
    for(int i = 0; i < 9; i++) {
        game_map[i] = 49 + i;
    }
}

/**
 * Start function
 */
int main(int argc, char* argv[]) {

    // dummy 
    if(argc || argv) {}

    // Init randomizer
    srand(time(NULL));

    mmenu();
}


