#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>

int main(int argc, string argv[])
{
    if (argc != 2) //If you didn't use my code correctly...
    {
        printf("Your inputs are wrong!!\n");
        return 1; 
    }
    string key = argv[1]; //Given key
    string plainText; //The plain text entered by the user
    int keyLen = strlen(key); //Length of the key
    int count = 0; //keeps track of which letter in the key
    int tempK; // Temporary K to be re-assigned each loop itteration
    int k; // This is the letter in the key
    for (int i = 0; i < keyLen; i++)
    {
        if (key[i]<65 || (key[i] > 90 && key[i] < 97) || key[i] > 122)
        {
            printf("You entered an invalid key!\n"); //Because it wasn't just letters!
            return 1; 
        }
    }
    plainText = GetString(); //Get the plaintext entry
    for (int i = 0, n = strlen(plainText); i < n; i++) //increment through all the plaintext letters
    {
        tempK = key[count%keyLen]; //create the temporary key
        if (tempK > 96 && tempK < 123)
        {
            k = tempK - 97; //If it's lowercase, send it down by 97
        }
        else if (tempK > 64 && tempK < 91)
        {
            k = tempK - 65; //If it's upppercase send it down by 65
        }
        if (plainText[i] > 96 && plainText[i] < 123) //maybe the input is lowercase? 
        {
            int item = ((((int) plainText[i]-97)+k)%26)+97; 
            printf("%c", (char) item);
            count ++; //Increment the count
        }
        else if (plainText[i] > 64 && plainText[i] < 91) //maybe the input is uppercase? 
        {
            //bring numbers down to start at 0, convert, bring back to ascii level!
            int item = ((((int) plainText[i]-65)+k)%26)+65; 
            printf("%c", (char) item);
            count ++; //increment the count
        }
        else //maybe the input isn't a letter?!!!
        {
            printf("%c", plainText[i]);
            //We don't want to increment the count here! Keep the key where it was!
        }
    }
    printf("\n"); //Finish off with a new line!
    return 0; //Yay!
}