// TODOs
// - 

// Include libraries
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <wchar.h>
#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"

int main(int argc, char *argv[]) {
    // Handle options
    int opt;
    char* imagePath = "";
    int asciiHeight = 30;

    opterr = 0;
    while ((opt = getopt(argc, argv, "h:")) != -1) {
        switch (opt)
        {
            case 'h':
                asciiHeight = atoi(optarg);
                if (asciiHeight <= 0) {
                    printf("Height must be a positive integer\n");
                    return 1;
                }
                break;
            case '?':
                printf("Unknown option -%c\n", optopt);
                return 1;
            default:
                printf("Error handling options\n");
                return 1;
        }
    }

    if (optind == argc) {
        printf("Basic usage: asciify <path to image>\n");
        return 1;
    }
    else {
        imagePath = argv[optind];
    }

    // Load image data

    int width, height, comps, channelCount;
    channelCount = 3;
    unsigned char *data = stbi_load(imagePath, &width, &height, &comps, channelCount);

    // Check for failure to load image

    if (data == NULL) {
        printf("Failure to load image data\n");
        return 2;
    }

    // Cacluclate scaling for ascii art

    int rowIncrement = height / asciiHeight;
    int columnIncrement = rowIncrement / 2;

    int asciiWidth = width / columnIncrement;
    int asciiArray[asciiHeight][asciiWidth];

    // Initialise variables for dynamic shading
     
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

    // Setting up ascii art
    setlocale(LC_ALL, "en.US");

    // █
    // ▓
    // ▒
    // ░

    // Print!!
    for (int asciiRow = 0; asciiRow < asciiHeight; asciiRow += 1) {
        for (int asciiColumn = 0; asciiColumn < asciiWidth; asciiColumn += 1) {
            int currentBlock = asciiArray[asciiRow][asciiColumn];
            if (currentBlock >= shadeUpperBound - shadeRange / 4) {
                printf("█");
            }
            else if (currentBlock >= shadeUpperBound - shadeRange / 2) {
                printf("▓");
            }
            else if (currentBlock >= shadeUpperBound - shadeRange * 7 / 10) {
                printf("▒");
            }
            else if (currentBlock >= shadeUpperBound - shadeRange * 4 / 5) {
                printf("░");
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
