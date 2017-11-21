/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 1;
    }

    // gather filenames
    char *infile = argv[1];
    char *outfile = argv[2];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    
    // printf("File size is: %i\n", bf.bfSize);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // printf("bi.biWidth is: %i\n", bi.biWidth);
    // printf("Padding is: %i\n", padding);

    // iterate over infile's scanlines
    // int nateCount0 = 0;
    // int nateCount1 = 0;
    
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            
            // assume that any value that is purely one color is noise. Therefore if there is any value that contains a 255 or a 0 (all or none), assume that we can ignore it
            // therefore if it is NOT a pure color (white or red), then it is of value to us!
            if ((triple.rgbtRed != 255 && triple.rgbtRed != 0) || (triple.rgbtGreen != 255 && triple.rgbtGreen != 0) || (triple.rgbtBlue != 255 && triple.rgbtBlue != 0)) {
                // to see it better... lets turn it to pure red and then write it to the file
                triple.rgbtRed = 255;
                triple.rgbtGreen = 0;
                triple.rgbtBlue = 0;
                
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                // nateCount1++;
            } else {
                // if it is a pure color, lets set the color to white so it contrasts with the message we are looking for!
                triple.rgbtRed = 255;
                triple.rgbtGreen = 255;
                triple.rgbtBlue = 255;
                
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            
            
            // nateCount0++;
            
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }
    
    // printf("Found %i non-255/non-0 values out of %i total pairs\n", nateCount1, nateCount0);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
