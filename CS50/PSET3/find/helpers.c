/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // If the array is empty... no need to search!
    if (n < 1)
    {
        return false;
    }
    //Initialize search start
    int nn = (n-1)/2; //Mid point index
    int ul = n-1; //The upper limit index
    int ll = 0; //Lower Limit index
    
    while (true)
    {
        /*
        printf("Upper Limit: %i\n", ul);
        printf("Lower Limit: %i\n", ll);
        printf("NN: %i\n", nn);
        */
        
        //If either of the end points or midpoint is the value... report true!
        if (values[nn] == value || values[ul] == value || values[ll] == value)
        {
            return true;
        }
        //Limits are right next to eachother and they were just checked above^
        if (ul-ll < 2) //Number is not in the list!
        {
            return false;
        }
        //Should the upper or lower limit be updated??
        if (values[nn] > value)
        {
            ul = nn;
        }
        else
        {
            ll = nn;
        }
        //Re-evaluate the midpoint based on the above determination
        nn = ((ul-ll)/2)+ll; 
        
    }
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm (Bubble Sort!)
    int change = 1; 
    int tempVal; 
    while (change > 0)
    {
        change = 0;
        for (int i = 0; i < n-1; i++)
        {
            if (values[i] > values[i+1])
            {
                tempVal = values[i+1];
                values[i+1] = values[i];
                values[i] = tempVal;
                change++;
            }
        }
    }
    return;
}