/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage of 4 cmd line args!
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    // get resize argument 
    float f = atof(argv[1]);
    if (f < 0 || f > 100) {
        fprintf(stderr, "Must input a value between 0 and 100\n");
        return 6;
    }
    int upSize = 0;
    int newPadding;
    char *lineInNewFile;

    // gather filenames
    char *infile = argv[2];
    char *outfile = argv[3];

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
    
    // save some original values from the image
    int originalPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int originalHeight = abs(bi.biHeight);
    int originalWidth = bi.biWidth;
    
    // TODO: Change the header here before you write it to the next file!!! 
    if (f > 1) {
        // the size factor is greater than 1... so we want to make the image larger!
        // round down to smaller int for upsizing
        upSize = (int) f;
        bi.biWidth = bi.biWidth * upSize;
        bi.biHeight = bi.biHeight * upSize;
        newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
        bi.biSizeImage = (bi.biWidth + newPadding) * bi.biHeight;
        bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    } else {
        // you want to make the image smaller here 
        bi.biWidth = (int) (bi.biWidth * f);
        if (bi.biWidth == 0) {
            // never allow the image to get smaller than a single pixel
            bi.biWidth = 1;
        }
        
        // check to make sure image is at least a single pixle tall... also preserve sign
        if ((int) (bi.biHeight * f) == 0) {
            bi.biHeight = (int) (bi.biHeight/bi.biHeight);
        } else {
            bi.biHeight = (int) (bi.biHeight * f);
        }
        newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
        bi.biSizeImage = (bi.biWidth + newPadding) * bi.biHeight;
        bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    
    if (upSize > 0) {
        // upSize is a value greater than 1... we want to make the image larger
        printf("Scaling image up by: %i\n", upSize);
        lineInNewFile = (char *) malloc(sizeof(RGBTRIPLE) * bi.biWidth);
        if (lineInNewFile == NULL) {
            return 5;
        }

        int count;
        
        // iterate through our old file's length and width fields
        for (int i = 0; i < originalHeight; i++) {
            
            count = 0;
            for (int m = 0; m < upSize; m++) {
                if (count == 0) {
                    // only want it to read from the first file once! Otherwise we'd have to seek back
                    
                    int newLineIndex = 0;
                    // iterate over pixels in the old file
                    for (int j = 0; j < originalWidth; j++) {
                        
                        // temporary storage
                        RGBTRIPLE triple;
            
                        // read RGB triple from infile
                        fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                        
                        for (int l = 0; l < upSize; l++) {
                            // write RGB triple to outfile as many times as scaled up by
                            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                            lineInNewFile[newLineIndex] = triple.rgbtBlue;
                            lineInNewFile[newLineIndex + 1] = triple.rgbtGreen;
                            lineInNewFile[newLineIndex + 2] = triple.rgbtRed;
                            newLineIndex += 3;
                            
                        }
                    }
                } else {
                    // you have read in the line once already. Now just re-write the same line over and over until m hits upSize
                    
                    fwrite(lineInNewFile, (sizeof(RGBTRIPLE) * bi.biWidth), 1, outptr);
                }
                count++;
                    
                // add the new padding on the end of the line
                for (int k = 0; k < newPadding; k++) {
                    fputc(0x00, outptr);
                }
            }
            
            
            // skip over padding, if any in the original file
            fseek(inptr, originalPadding, SEEK_CUR);
            
        }
        free(lineInNewFile);
    } else {
        // you are down sizing the file... 
        printf("Scaling image down to %i percent of its original size\n", (int) (f * 100));
        
        // this will work for every value... let's just start here and then delete the if when it works!
        float saveIndexWidth = (float) originalWidth/(float) bi.biWidth;
        float saveIndexHeight = fabsf((float) originalHeight/(float) bi.biHeight);
        
        
        float tmpIndexHeight = 0.0;
        
        int rowWrites = 0;
        int colWrites;
        
        
        // loop through each row in the bmp file
        for (int i = 0; i < originalHeight; i++) {
            
            // if your index is higher then the tmp one, you want this row!
            if ((float) i >= tmpIndexHeight) {
                
                float tmpIndexWidth = 0.0;
                colWrites = 0;

                // now lets loop through each pixel in this row
                for (int j = 0; j < originalWidth; j++) {
                    
                    // if the "j" index is higher then the tmp one, we want this value!
                    if ((float) j >= tmpIndexWidth) {
                        RGBTRIPLE triple;
                        fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                        colWrites++;
                        tmpIndexWidth = saveIndexWidth * (float) colWrites;

                    } else {
                        // your index "j" is not higher so you want to seek to the next pixle
                        
                        // NOTE: Could this be incrementing once on the last iteration? And causing the image to shift by one? 
                        fseek(inptr, sizeof(RGBTRIPLE), SEEK_CUR);
                    }
                }

                // you have evaluated a row and written something to the new file... add to your rowWrites, re-compute your tmpIndexHeight, and write any padding
                for (int k = 0; k < newPadding; k++) {
                    fputc(0x00, outptr);
                }
                rowWrites++;
                tmpIndexHeight = saveIndexHeight * (float) rowWrites;
                fseek(inptr, originalPadding, SEEK_CUR);
            } else {
                // your index "i" is not higher so you want to seek to the next row instead
                fseek(inptr, ((sizeof(RGBTRIPLE) * originalWidth) + originalPadding), SEEK_CUR);
            }
        }

    }
    

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
