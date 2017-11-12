#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>

int main(int argc, string argv[]) //Includes the command line arguments!
{
    if (argc != 2) //If you don't have exactly 2 arguments... ERRRROORR!
    {
        printf("Your cmd line args are wrong!\n");
        return 1; 
    }
    int k = atoi(argv[1]);
    string plainText = GetString();
    for (int i = 0, n = strlen(plainText); i < n; i++)
    {
        if (plainText[i] > 96 && plainText[i] < 123) //maybe the input is lowercase? 
        {
            int item = ((((int) plainText[i]-97)+k)%26)+97; 
            printf("%c", (char) item);
        }
        else if (plainText[i] > 64 && plainText[i] < 91) //maybe the input is uppercase? 
        {
            //bring numbers down to start at 0, convert, bring back to ascii level!
            int item = ((((int) plainText[i]-65)+k)%26)+65; 
            printf("%c", (char) item);
        }
        else //maybe the input isn't a letter?!!!
        {
            printf("%c", plainText[i]);
        }
    }
    printf("\n"); //Finish off with a new line!
    return 0; //If you get here... yay! 
}