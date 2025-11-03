// TODOs
// - Light mode handling (use flag/getopt)
// - Use relative shading instead of absolute
// - Improve shading algorithm, avg of area rather than of one pixel

// Include libraries
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    // Setting up ascii art support
    setlocale(LC_ALL, "en_US.UTF-8");

    wchar_t whiteSquare = L'\u2588';
    wchar_t lightGreySquare = L'\u2593';
    wchar_t greySquare = L'\u2592';
    wchar_t darkGreySquare = L'\u2591';

    // Cacluclate scaling for ascii art
    int rowIncrement = round(height / asciiHeight);
    int columnIncrement = round((rowIncrement *3 ) / 5);

    // Print!!!
    for (int row = 0; row < height; row += rowIncrement) {
        for (int column = 0; column < width; column += columnIncrement) {
            unsigned int pixelIndex = (row * width + column) * channelCount;
            unsigned char red = data[pixelIndex];
            unsigned char green = data[pixelIndex + 1];
            unsigned char blue = data[pixelIndex + 1];
            int average = round((red + green + blue) / 3);
            if (average > 180) {
                wprintf(L"%lc", whiteSquare);
            }
            else if (average > 110) {
                wprintf(L"%lc", lightGreySquare);
            }
            else if (average > 80) {
                wprintf(L"%lc", greySquare);
            }
            else if (average > 50) {
                wprintf(L"%lc", darkGreySquare);
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
