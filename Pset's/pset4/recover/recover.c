#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
 
const int BLOCK_SIZE = 512;

typedef uint8_t BYTE;

 
int main(int argc, char *argv[])
{
    // checking validity of user input
    if (argc != 2)
    {
        printf("Usage: ./recover image \n");
        return 1;
    }
        
    // checking validity of file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }
    
    BYTE buffer[BLOCK_SIZE];
    size_t bytes_read;
    FILE *current_file;
    
    bool first_jpeg = false;
    bool found_jpeg = false;
    
    int current_filenumber = 0;
    char current_filename[100];
    
    while (true)
    {
        // read through whole card
        bytes_read = fread(buffer, sizeof(BYTE), BLOCK_SIZE, file);

        // we break out of the loop when we reach end
        if (bytes_read == 0)
        {
            break;
        }
        
        // if start of new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            found_jpeg = true;
            
            // if its a first jpeg, we mark it
            if (!first_jpeg)
            {
                first_jpeg = true;
            }
            
            // otherwise we close current file
            else
            {
                fclose(current_file);
            }
            
            // create new jpeg file
            sprintf(current_filename, "%03i.jpg", current_filenumber);
            
            // open it for writing
            current_file = fopen(current_filename, "w");
            
            // write to new jpeg file
            fwrite(buffer, sizeof(BYTE), bytes_read, current_file);
            
            current_filenumber++;
        }
        
        else
        {
            // proceed writing to jpeg if it was already found
            if (found_jpeg)
            {
                fwrite(buffer, sizeof(BYTE), bytes_read, current_file);
            }
        }
    }
    fclose(file);
    fclose(current_file);
    return 0;
}