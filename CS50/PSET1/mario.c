#include <stdio.h>
#include <cs50.h>

int main (void)
{
    int height = 0;
    int swidth;
    int wwidth; 
    
    do
    {
        printf("Height: ");
        height = GetInt();
        //printf("%i\n", height);
    }
    while (height < 0 || height > 23);
    
    swidth = height-1;
    wwidth = 2; 
    for (int i=0; i < height; i++)
    {
        for (int j=0; j < swidth; j++)
        {
            printf(" ");
        }
        swidth = swidth - 1;
        
        for (int k = 0; k < wwidth; k++)
        {
            printf("#");
        }
        wwidth = wwidth + 1;
        printf("\n");
        
    }
    
}