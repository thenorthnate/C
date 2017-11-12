#include <stdio.h>
#include <cs50.h>

int main (void)
{
    
    printf("How many minutes do you shower for?\n");
    int n = GetInt();
    int bottles = n*192/16;
    
    printf("Minutes: %i\n", n);
    printf("Bottles: %i\n", bottles);
    
    
}