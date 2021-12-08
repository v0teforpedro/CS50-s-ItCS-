#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, string argv[])
{

    int track = 0;
    int text;
    // checking that only 2nd argument counts
    if (argc == 2)
    {
        // for loop to check if only digits were typed
        for (int i = 0, n = strlen(argv[1]); i < n ; i++)
        {
            if (isdigit(argv[1][i]))
            {
                track++;
            }
        }
        if (track == strlen(argv[1]))
        {
            // converting string to an integer
            text = atoi(argv[1]);
                
            // promt to get plaintext from user and save it as string
            string plain = get_string("plaintext: ");
            
            // printing cyphertext using below conditions
            printf("ciphertext: ");
            
            // for loop to go through all chars in string, adding key to get cyphertext
            for (int x = 0, y = strlen(plain); x < y; x++)
            {
                // for lower case
                if (isalpha(plain[x]) && islower(plain[x]))
                {
                    printf("%c", (((plain[x] - 'a') + text) % 26) + 'a');
                }
                // for upper case
                else if (isalpha(plain[x]) && isupper(plain[x]))
                {
                    printf("%c", (((plain[x] - 'A') + text) % 26) + 'A');
                }
                // printing other symbols without change
                else
                {
                    printf("%c", plain[x]);
                }
            }
            // new line for style
            printf("\n");
        }
        else
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}