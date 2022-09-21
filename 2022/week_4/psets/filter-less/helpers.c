#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE gray_value = round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0);
            image[i][j].rgbtBlue = gray_value;
            image[i][j].rgbtGreen = gray_value;
            image[i][j].rgbtRed = gray_value;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int sepiaRed, sepiaGreen, sepiaBlue;
    BYTE originalBlue, originalGreen, originalRed;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculating values
            originalBlue = image[i][j].rgbtBlue;
            originalGreen = image[i][j].rgbtGreen;
            originalRed = image[i][j].rgbtRed;
            sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            // Assigning and limiting values
            image[i][j].rgbtBlue = (sepiaBlue >= 255) ? 255 : sepiaBlue;
            image[i][j].rgbtGreen = (sepiaGreen >= 255) ? 255 : sepiaGreen;
            image[i][j].rgbtRed = (sepiaRed >= 255) ? 255 : sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp_pixel;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int k = width - j - 1;  // mirror pixel index
            if (j >= k)  // overlapping
            {
                break;
            }

            // Swapping pixels
            temp_pixel = image[i][j];
            image[i][j] = image[i][k];
            image[i][k] = temp_pixel;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Creating a copy of the image, for the blurred pixel values do not affect the blur values of the next pixels
    RGBTRIPLE image_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_copy[i][j] = image[i][j];
        }
    }

    int sumRed, sumGreen, sumBlue;
    float pixelCount;  // Producing a float for round()
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Resetting values
            sumRed = 0, sumGreen = 0, sumBlue = 0, pixelCount = 0;

            // Calculating sum of surrounding pixels
            for (int k = i - 1; k <= i + 1; k++)
            {
                if (k >= height || k < 0)  // out of bounds
                {
                    continue;  // pass this iteration
                }
                for (int l = j - 1; l <= j + 1; l++)
                {
                    if (l >= width || l < 0)  // out of bounds
                    {
                        continue;  // pass this iteration
                    }
                    pixelCount++;
                    sumRed += image_copy[k][l].rgbtRed;
                    sumBlue += image_copy[k][l].rgbtBlue;
                    sumGreen += image_copy[k][l].rgbtGreen;
                }
            }

            // Caluculating new pixel values
            image[i][j].rgbtRed = round(sumRed / pixelCount);
            image[i][j].rgbtBlue = round(sumBlue / pixelCount);
            image[i][j].rgbtGreen = round(sumGreen / pixelCount);
        }
    }
    return;
}
