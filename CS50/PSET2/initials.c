#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void)
{
    string name;
    //printf("Enter your name: ");
    name = GetString();
    int sCheck = 1;
    for (int i = 0, n = strlen(name); i < n; i++)
    {
        if (sCheck == 1)
        {
            if (name[i] > 96 && name[i] < 123)
            {
                printf("%c", name[i]-32);
                sCheck = 0;
            }
            else
            {
                printf("%c", name[i]);
                sCheck = 0;
            }
        }
        if ((int) name[i] == 32)
        {
            sCheck = 1; 
        }
    }
    printf("\n");

}