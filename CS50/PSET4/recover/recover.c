// Nathan North 
// CS 50
// Recovers data from "lost" image
       
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char buf[512];
} BUF;


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // get image name
    char *infile = argv[1];

    // open input file 
    // printf("Opening %s\n", infile);
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    
    // initialize variables 
    char fileNameBuffer[50];
    int itemsReturned; 
    int fileNumber = 0;
    int writeToFile = 0;
    
    sprintf(fileNameBuffer,"%03i.jpg", fileNumber);
    
    // create your file pointer here
    FILE *outptr = fopen(fileNameBuffer, "w");
    if (outptr == NULL) {
        // if there was an error... close the file pointer
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", fileNameBuffer);
        return 3;
    }
    
    
    // repeat over and over again until you have read through the entire image
    while (1) {
        
        BUF fileBuffer;
        
        // first read in a data block 
        itemsReturned = fread(&fileBuffer, 1, sizeof(BUF), inptr);
        
        // itemsReturned = fread(&fileBuffer, sizeof(BUF), 1, inptr);
        
        // check if you have reached the end of the file
        if (itemsReturned < sizeof(BUF)) {
            // if you have... write remainder to the file and close it
            // fwrite(&fileBuffer, sizeof(BUF), 1, outptr);
            for (int i = 0; i < itemsReturned; i++) {
                fputc(fileBuffer.buf[i], outptr);
            }
            fclose(outptr);
            break;
        }
        
        // check if the header is a standard jpeg header
        if (fileBuffer.buf[0] == (char) 0xff && fileBuffer.buf[1] == (char) 0xd8 && fileBuffer.buf[2] == (char) 0xff && (fileBuffer.buf[3] & 0xf0) == 0xe0) {
            // printf("Found %i jpeg start handle\n", fileNumber);
            writeToFile = 1;
    
            // the very first file ptr is already open... so we don't want to open another one this time!
            if (fileNumber != 0) {

                // you want to close your previously opened file!
                fclose(outptr);
                sprintf(fileNameBuffer,"%03i.jpg", fileNumber);
                outptr = fopen(fileNameBuffer, "w");
                if (outptr == NULL) {
                    fclose(inptr);
                    fprintf(stderr, "Could not create %s.\n", fileNameBuffer);
                    return 3;
                }
            } 
            fileNumber++;
        }
        
        
        if (writeToFile == 1) {
            // now write to whichever file is currently open!
            fwrite(&fileBuffer, sizeof(BUF), 1, outptr);
        }
        
        
    }

    // printf("Closing %s\n", infile);
    // close the input file
    fclose(inptr);

    // success
    return 0;
}
