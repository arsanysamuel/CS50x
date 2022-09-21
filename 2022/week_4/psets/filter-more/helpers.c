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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Kernels
    int Gx_kernel[][3] =  // initializing 2d array, the first size value could be skipped
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy_kernel[][3] =  // initializing 2d array, the first size value could be skipped
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    // Defining a black pixel
    RGBTRIPLE black_pixel;
    black_pixel.rgbtBlue = 0x00;
    black_pixel.rgbtRed = 0x00;
    black_pixel.rgbtGreen = 0x00;

    // Creating a copy of the image, for the other pixel values do not affect the values of the calculated pixel
    RGBTRIPLE image_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_copy[i][j] = image[i][j];
        }
    }

    // Calculating Sobel Gx, Gy
    RGBTRIPLE surr_pixel;  // the current surrounding pixel
    int G_red, G_green, G_blue, Gx_red, Gx_green, Gx_blue, Gy_red, Gy_green, Gy_blue;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculating Sobel values of surrounding pixels
            G_red = 0, G_green = 0, G_blue = 0;
            Gx_red = 0, Gx_green = 0, Gx_blue = 0, Gy_red = 0, Gy_green = 0, Gy_blue = 0;  // resetting values
            for (int y = 0, k = i - 1; k <= i + 1; y++, k++)
            {
                for (int x = 0, l = j - 1; l <= j + 1; x++, l++)
                {
                    surr_pixel = (k >= height || k < 0 || l >= width || l < 0) ? black_pixel : image_copy[k][l];  // out of bounds?

                    // Calculating Gx, Gy values for all color channels
                    Gx_red += (surr_pixel.rgbtRed * Gx_kernel[y][x]);
                    Gy_red += (surr_pixel.rgbtRed * Gy_kernel[y][x]);
                    Gx_green += (surr_pixel.rgbtGreen * Gx_kernel[y][x]);
                    Gy_green += (surr_pixel.rgbtGreen * Gy_kernel[y][x]);
                    Gx_blue += (surr_pixel.rgbtBlue * Gx_kernel[y][x]);
                    Gy_blue += (surr_pixel.rgbtBlue * Gy_kernel[y][x]);
                }
            }
            // Calculating Sobel pixel value
            G_red = round(sqrt(pow(Gx_red, 2) + pow(Gy_red, 2)));
            G_green = round(sqrt(pow(Gx_green, 2) + pow(Gy_green, 2)));
            G_blue = round(sqrt(pow(Gx_blue, 2) + pow(Gy_blue, 2)));
            image[i][j].rgbtRed = (G_red > 255) ? 255 : G_red;
            image[i][j].rgbtGreen = (G_green > 255) ? 255 : G_green;
            image[i][j].rgbtBlue = (G_blue > 255) ? 255 : G_blue;
        }
    }
    return;
}
