#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            BYTE blue = image[row][col].rgbtBlue;
            BYTE green = image[row][col].rgbtGreen;
            BYTE red = image[row][col].rgbtRed;

            float avg = (float)(blue+green+red)/3.0;

            BYTE out = (BYTE) round(avg);

            image[row][col].rgbtBlue = out;
            image[row][col].rgbtGreen = out;
            image[row][col].rgbtRed = out;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width/2; ++col)
        {
            RGBTRIPLE tmp = image[row][col];
            image[row][col] = image[row][width-col-1];
            image[row][width-col-1] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            float blue_avg = 0;
            float green_avg = 0;
            float red_avg = 0;
            float count = 0;

            // 3x3 Grid
            for (int i = row-1; i <= row+1; ++i)
            {
                if (i == -1 || i == height)
                {
                    continue;
                } else
                {
                    for (int j = col-1; j <= col+1; ++j)
                    {
                        if (j == -1 || j == width)
                        {
                            continue;
                        } else
                        {
                            BYTE blue = image[i][j].rgbtBlue;
                            BYTE green = image[i][j].rgbtGreen;
                            BYTE red = image[i][j].rgbtRed;

                            blue_avg += (float) blue;
                            green_avg += (float) green;
                            red_avg += (float) red;
                            count ++;
                        }
                    }
                }
            }

            float avgb =  blue_avg / count;
            float avgg =  green_avg / count;
            float avgr =  red_avg / count;

            temp[row][col].rgbtBlue = round(avgb);
            temp[row][col].rgbtGreen = round(avgg);
            temp[row][col].rgbtRed = round(avgr);
        }
    }

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            image[row][col].rgbtBlue = (BYTE) temp[row][col].rgbtBlue;
            image[row][col].rgbtGreen = (BYTE) temp[row][col].rgbtGreen;
            image[row][col].rgbtRed = (BYTE) temp[row][col].rgbtRed;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            float GxBlue = 0;
            float GxGreen = 0;
            float GxRed = 0;

            float GyBlue = 0;
            float GyGreen = 0;
            float GyRed = 0;

            float GxKernel[3][3] = {
                {-1,0,1},
                {-2,0,2},
                {-1,0,1}
            };

            float GyKernel[3][3] = {
                {-1,-2,-1},
                {0,0,0},
                {1,2,1}
            };

            // 3x3 Grid
            for (int i = -1; i < 2; ++i)
            {
                for (int j = -1; j < 2; ++j)
                {
                    if (i+row == -1 || i+row == height || j+col == -1 || j+col == width)
                    {
                        BYTE blue = 0;
                        BYTE green = 0;
                        BYTE red = 0;
                    } else
                    {
                        BYTE blue = image[i+row][j+col].rgbtBlue;
                        BYTE green = image[i+row][j+col].rgbtGreen;
                        BYTE red = image[i+row][j+col].rgbtRed;

                        GxBlue += GxKernel[i+1][j+1] * (float) blue;
                        GxGreen += GxKernel[i+1][j+1] * (float) green;
                        GxRed += GxKernel[i+1][j+1] * (float) red;

                        GyBlue += GyKernel[i+1][j+1] * (float) blue;
                        GyGreen += GyKernel[i+1][j+1] * (float) green;
                        GyRed += GyKernel[i+1][j+1] * (float) red;
                    }
                }
            }

            float AvgBlue = round(sqrt(GxBlue*GxBlue + GyBlue*GyBlue));
            float AvgGreen = round(sqrt(GxGreen*GxGreen + GyGreen*GyGreen));
            float AvgRed = round(sqrt(GxRed*GxRed + GyRed*GyRed));

            if (AvgBlue < 0)
            {
                AvgBlue = 0;
            } else if (AvgBlue > 255)
            {
                AvgBlue = 255;
            }

            if (AvgGreen < 0)
            {
                AvgGreen = 0;
            } else if (AvgGreen > 255)
            {
                AvgGreen = 255;
            }

            if (AvgRed < 0)
            {
                AvgRed = 0;
            } else if (AvgRed > 255)
            {
                AvgRed = 255;
            }

            temp[row][col].rgbtBlue = AvgBlue;
            temp[row][col].rgbtGreen = AvgGreen;
            temp[row][col].rgbtRed = AvgRed;
        }
    }

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            image[row][col].rgbtBlue = (BYTE) temp[row][col].rgbtBlue;
            image[row][col].rgbtGreen = (BYTE) temp[row][col].rgbtGreen;
            image[row][col].rgbtRed = (BYTE) temp[row][col].rgbtRed;
        }
    }
}
