#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height, row, column, space;
    
    // Promt the user to give height
    do
    {
        height = get_int("What is the height of pyramid?: ");
    }
    // Integer input is restricted between 1 and 8
    while (height < 1 || height > 8);
    
    // Creating height
    for (row = 0; row < height; row++)
    {
        // Creating width
        for (space = 0; space < height - row - 1; space++)
        {
            // Putting spaces in pyramid first (if needed)
            printf(" ");
        }
        for (column = 0; column <= row; column++)
        {
            // Putting hashes in pyramid
            printf("#");
        }
        printf("\n");
    }
}