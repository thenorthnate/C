#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main (void)
{
    int coinCount = 0;
    float change; 
    do
    {
        printf("How much change you need?!\n");
        change = GetFloat();
    }
    while (change < 0);
    float tCents = change*100;
    int cents = (int) round(tCents); 
    
    while (cents > 0)
    {
        //printf("%i\n", cents); 
        if (cents >= 25)
        {
            coinCount++;
            cents = cents - 25;
        }
        else if (cents < 25 && cents >= 10)
        {
            coinCount++;
            cents = cents - 10; 
        }
        else if (cents < 10 && cents >= 5)
        {
            coinCount++;
            cents = cents - 5;
        }
        else
        {
            coinCount = coinCount + cents;
            cents = 0;
        }
        
    }
    
    printf("%i\n", coinCount);
    
}