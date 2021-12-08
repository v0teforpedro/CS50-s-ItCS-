#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // iteration though rows and columns
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel_color = image[i][j];
            
            // rounding up colors
            int average = round((pixel_color.rgbtGreen + pixel_color.rgbtRed + pixel_color.rgbtBlue) / 3.0);
            
            // transfer new values to implement greyscale
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // iteration though rows and columns
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel_color = image[i][j];
            
            // application of formula
            int sepiaRed = round(.393 * pixel_color.rgbtRed + .769 * pixel_color.rgbtGreen + .189 * pixel_color.rgbtBlue);
            int sepiaGreen = round(.349 * pixel_color.rgbtRed + .686 * pixel_color.rgbtGreen + .168 * pixel_color.rgbtBlue);
            int sepiaBlue = round(.272 * pixel_color.rgbtRed + .534 * pixel_color.rgbtGreen + .131 * pixel_color.rgbtBlue);
            
            // maing sure we wont exceed available size
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            
            // transfer new values to implement sepia
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[height][width];
    
    // iteration though rows
    for (int i = 0; i < height; i++)
    {
        // reverse position storage
        int revpos = 0;
        // iteration through columns reversed
        for (int j = width - 1; j >= 0; j--, revpos++)
        {
            // inserting reversed row of columns to buffer
            buffer[i][revpos] = image[i][j];
        }
    }
    
    // loop to change original image with reversed
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = buffer[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[height][width];
    
    // iteration through rows
    for (int row = 0; row < height; row++)
    {
        // iteration through columns
        for (int column = 0; column < width; column++)
        {
            int count = 0;
            
            // set array of "x" and "y" coordinates
            int xCoord[] = { row - 1, row, row + 1 };
            int yCoord[] = { column - 1, column, column + 1 };
            
            float totalR = 0;
            float totalG = 0;
            float totalB = 0;
            
            // loop to iterate through 3x3 grid of coordinates for each pixel
            for (int a = 0; a < 3; a++)
            {
                for (int b = 0; b < 3; b++)
                {
                    int curRow = xCoord[a];
                    int curColumn = yCoord[b];
                    
                    // validating pixels and adding colors in totals
                    if (curRow >= 0 && curRow < height && curColumn >= 0 && curColumn < width)
                    {
                        RGBTRIPLE pixel = image[curRow][curColumn];
                        
                        totalR += pixel.rgbtRed;
                        totalG += pixel.rgbtGreen;
                        totalB += pixel.rgbtBlue;
                        count++;
                    }
                }
            }
            // calculating new values for pixels in buffer
            buffer[row][column].rgbtRed = round(totalR / count);
            buffer[row][column].rgbtGreen = round(totalG / count);
            buffer[row][column].rgbtBlue = round(totalB / count);
        }
    }
    
    // loop to change original image with blurred from buffer
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = buffer[i][j];
        }
    }
    return;
}
