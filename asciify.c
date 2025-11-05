// TODOs
// - getopt usage

// Include libraries
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"

int main(int argc, char *argv[]) {

    // Check for invalid usage

    if (argc != 3) {
        printf("Invalid number of arguments | usage : asciify <imagepath> <height>");
        return 1;
    }

    // Check for valid height

    int asciiHeight = atoi(argv[2]);
    if (asciiHeight == 0) {
        printf("<height> provided must be a valid integer | usage : asciify <imagepath> <height>");
        return 2;
    }

    // Load image data

    int width, height, comps, channelCount;
    channelCount = 3;
    unsigned char *data = stbi_load(argv[1], &width, &height, &comps, channelCount);

    // Check for failure to load image

    if (data == NULL) {
        printf("Failure to load image data");
        return 2;
    }

    // Cacluclate scaling for ascii art

    int rowIncrement = height / asciiHeight;
    int columnIncrement = rowIncrement / 2;

    int asciiWidth = width / columnIncrement;
    int asciiArray[asciiHeight][asciiWidth];

    int shadeLowerBound = 300;
    int shadeUpperBound = 0;

    // Find averages for each ascii "block/character"
    for (int asciiRow = 0; asciiRow < asciiHeight; asciiRow += 1) {
        for (int asciiColumn = 0; asciiColumn < asciiWidth; asciiColumn += 1) {
            int blockTotal = 0;
            for (int row = asciiRow*rowIncrement; row < (asciiRow + 1) * rowIncrement; row += 1) {
                for (int column = asciiColumn*columnIncrement; column < (asciiColumn + 1) * columnIncrement; column += 1) {
                    unsigned int pixelIndex = (row * width + column) * channelCount;
                    unsigned char red = data[pixelIndex];
                    unsigned char green = data[pixelIndex + 1];
                    unsigned char blue = data[pixelIndex + 1];
                    int average = (red + green + blue) / 3;
                    blockTotal += average;
                }
            }
            int blockAverage = blockTotal / (rowIncrement * columnIncrement);
            if (blockAverage < shadeLowerBound) {
                shadeLowerBound = blockAverage;
            } 
            else if (blockAverage > shadeUpperBound) {
                shadeUpperBound = blockAverage;
            }
            asciiArray[asciiRow][asciiColumn] = blockAverage;
        }
    }

    // Determine shade range
    int shadeRange = shadeUpperBound - shadeLowerBound;
    printf("%i %i %i", shadeLowerBound, shadeUpperBound, shadeRange);

    // Setting up ascii art support
    setlocale(LC_ALL, "en_US.UTF-8");

    // Print!!
    for (int asciiRow = 0; asciiRow < asciiHeight; asciiRow += 1) {
        for (int asciiColumn = 0; asciiColumn < asciiWidth; asciiColumn += 1) {
            int currentBlock = asciiArray[asciiRow][asciiColumn];
            if (currentBlock >= shadeUpperBound - shadeRange / 4) {
                printf("\u2588");
            }
            else if (currentBlock >= shadeUpperBound - shadeRange / 2) {
                printf("\u2593");
            }
            else if (currentBlock >= shadeUpperBound - shadeRange * 7 / 10) {
                printf("\u2592");
            }
            else if (currentBlock >= 50) {
                printf("\u2591");
            }
            else {
                printf(" ");
            }
        }
        printf("\n");
    }

    // Free image data
    stbi_image_free(data);

}
