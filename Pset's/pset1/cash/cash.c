#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    float dollars;
    
    do
    {
        // promts user to give number o change
        dollars = get_float("What's your change?: ");   
    }
    // condition to keep number positive
    while (dollars <= 0);
    
    int cents = round(dollars * 100);
    int coins = 0;
    
    // deduction of cents to determine coins needed for change
    while (cents >= 25)
    {
        cents -= 25;
        coins++;    
    }
    while (cents >= 10)
    {
        cents -= 10;
        coins++;
    }
    while (cents >= 5)
    {
        cents -= 5;
        coins++;    
    }
    while (cents >= 1)
    {
        cents -= 1;
        coins++;
    }
    // printing the result
    printf("You will get %i coins\n", coins);
}