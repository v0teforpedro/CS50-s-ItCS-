#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // Promping user to gives us text
    string text = get_string("Text: ");
    
    // setting base value for our integers
    int letters = 0;
    int words = 1;
    int sentences = 0;
    
    // making for-loop to calculate end values for our integers posted above
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
        
        if (isspace(text[i]))
        {
            words++;
        }
        
        if (text[i] == '!' || text[i] == '?' || text[i] == '.')
        {
            sentences++;
        }
    }
    
    // calculating index per given formula using our collected data above
    float L = (letters / (float) words) * 100;
    float S = (sentences / (float) words) * 100;
    int index = round(0.0588 * L - 0.296 * S - 15.8);
    
    // printing results per grade level
    if (index <= 0)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}