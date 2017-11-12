/**
 * generate.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Generates pseudorandom numbers in [0,LIMIT), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// constant
#define LIMIT 65536

int main(int argc, string argv[])
{
    // This if statement ensures that you've entered either 2 or 3 command line arguments
    // If you haven't, the printf executes and the program returns 1
    if (argc != 2 && argc != 3)
    {
        printf("Usage: generate n [s]\n");
        return 1;
    }

    // This line assignes a value to the variable "n"
    // The atoi function converts the command argument string into an integer
    int n = atoi(argv[1]);

    // If there are 3 command line arguments, then use the third cmd line arg as your seed!
    // Otherwise, use the current time as your "random" seed
    if (argc == 3)
    {
        srand48((long int) atoi(argv[2]));
    }
    else
    {
        srand48((long int) time(NULL));
    }

    // drand48 produces a random number between 0 and 1... the limit was set as a
    // constant for this code above in the #DEFINE statement
    // The loop prints the random number multiplied by the limit as an integer 
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}