/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 * 
 */
 
#define _XOPEN_SOURCE 500

//Include your .h files!
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(int board[][DIM_MAX], int d);
void draw(int board[][DIM_MAX], int d, int emptyXY[]);
bool move(int tile, int board[][DIM_MAX], int d, int emptyXY[]);
bool won(int d);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    int emptyXY[2];
    init(board, d);

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw(board, d, emptyXY);

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won(d))
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile, board, d, emptyXY))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(int board[][DIM_MAX], int d)
{
    int bNum = d*d-1; //initialize the board number
    if (d%2)
    {
        // True means 1: Odd dimensions and 1-2 don't have to be swapped!
        for (int i = 0; i<d; i++)
        {
            for (int j = 0; j<d; j++)
            {
                board[i][j] = bNum; //place the number of the board into its respective place
                bNum--; //decrement the number by one each time it is put in place
            }
        }
    }
    else
    {
        // False means 0: Even dimensions and 1-2 have to be swapped
        int i = 0;
        int j = 0;
        for (i = 0; i<d; i++)
        {
            for (j = 0; j<d; j++)
            {
                board[i][j] = bNum;
                bNum--; 
            }
        }
        //Swap the 1 and the 2!
        board[i-1][j-2] = 2;
        board[i-1][j-3] = 1;
        
    }
}

/**
 * Prints the board in its current state.
 */
void draw(int board[][DIM_MAX], int d, int emptyXY[])
{
    for (int i = 0; i<d; i++)
    {
        for (int j = 0; j<d; j++)
        {
            if (board[i][j] == 0)
            {
                printf(" _ "); //Print an underscore instead of a zero
                emptyXY[0] = i; //keep track of the x/y position of the 0!
                emptyXY[1] = j;
            }
            else
            {
                //otherwise just print the number that's there
                printf("%2i ", board[i][j]);
            }
        }
        //move to the next line when you are done printing the last line
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile, int board[][DIM_MAX], int d, int emptyXY[])
{
    for (int i = 0; i<d; i++)
        {
            for (int j = 0; j<d; j++) //Search through all the board tiles
            {
                if (board[i][j] == tile) //if the tile is found... do the following
                {
                    if (i == emptyXY[0] && j == emptyXY[1]+1)
                    {
                        //tile is in the same column as the empty, one row down
                        //swap tiles
                        board[i][j-1] = tile;
                        board[i][j] = 0;
                        return true;
                    }
                    else if (i == emptyXY[0] && j == emptyXY[1]-1)
                    {
                        //tile is in the same column as the empty, one row up
                        //swap tiles
                        board[i][j+1] = tile;
                        board[i][j] = 0;
                        return true;
                    }
                    else if (i == emptyXY[0]-1 && j == emptyXY[1])
                    {
                        //tile is in the same row as the empty, one column ahead
                        //swap tiles
                        board[i+1][j] = tile;
                        board[i][j] = 0;
                        return true;
                    }
                    else if (i == emptyXY[0]+1 && j == emptyXY[1])
                    {
                        //tile is in the same row as the empty, one column behind
                        //swap tiles
                        board[i-1][j] = tile;
                        board[i][j] = 0;
                        return true;
                    }
                }
            }
        }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(int d)
{
    int boardValue = 0;
    int i;
    int j;
    for (i = 0; i<d; i++)
        {
            for (j = 0; j<d; j++)
            {
                if (board[i][j] == 0)
                {
                    //do nothing
                }
                else if (board[i][j] < boardValue)
                {
                    //NO value can be less than the one before it
                    return false;
                }
                boardValue = board[i][j];
            }
        }
        if (board[d-1][d-1] == 0)
        {
            //Empty tile needs to be in the last place to win
            return true;
        }
    return false;
}