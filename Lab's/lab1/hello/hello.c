#include <stdio.h>

#include <cs50.h>

int main(void)
{
    //asking user name
    string name = get_string("what is your name?\n");
    
    //greeting user
    printf("hello, %s\n", name);
}