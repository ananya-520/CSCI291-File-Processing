//Importing Libraries
#include <stdio.h>
#include <stdlib.h>

//Defining Global Constants
#define WIDTH 512
#define HEIGHT 512

//Function Prototypes
int readPGMText(const char *filename, unsigned char *pixels, int width, int height);
int writePGMText(const char *filename, unsigned char *pixels, int width, int height);
int writePGMBinary(const char *filename, unsigned char *pixels, int width, int height);
void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels, int width, int height);
void extractLSB(unsigned char *coverPixels, unsigned char *outputPixels, int width, int height);

int main() {

    //Variables for Images
    char cover_image[]="C:\\Users\\anany\\OneDrive\\Desktop\\Coding\\C Programming\\CSCI291 Work\\Lab 4\\Task 2\\baboon.pgm";
    char secret_image[]="c:/Users/anany/OneDrive/Desktop/Coding/C Programming/CSCI291 Work/Lab 4/Task 2/farm.pgm";
    char stego_image[]="c:/Users/anany/OneDrive/Desktop/Coding/C Programming/CSCI291 Work/Lab 4/Task 2/stego_image_bin.pgm";
    char extracted_secret[]="c:/Users/anany/OneDrive/Desktop/Coding/C Programming/CSCI291 Work/Lab 4/Task 2/extracted_secret.pgm";
    
    //Pointers for the Images
    unsigned char *coverPixels, *secretPixels, *outputPixels;
    int coverWidth=WIDTH, coverHeight=HEIGHT, secretWidth=WIDTH, secretHeight=HEIGHT;

    // Using malloc, dynnamically request extact memory space,referenced by coverPixels, to store the cover image pixel values. 
    coverPixels = (unsigned char *)malloc(WIDTH * HEIGHT);
    if (coverPixels == NULL) {
        printf("Error: Memory allocation for coverPixels failed.\n");
        return 1;
    }
	
    // Read the cover image file (text PGM) and store its pixels in 'coverPixels' referenced memory space.
    if (readPGMText(cover_image, coverPixels, coverWidth, coverHeight) != 0) {
        printf("Error: Cannot read cover image.\n");
        free(coverPixels);
        return 1;
    }

    //  Using malloc, dynamically request extact memory space, referenced by 'secretPixels', to store the secret image pixel values.
    secretPixels = (unsigned char *)malloc(WIDTH * HEIGHT);
    if (secretPixels == NULL) {
        printf("Error: Memory allocation for secretPixels failed.\n");
        free(coverPixels);
        return 1;
    }
    
    // Read the secret image file (text PGM) and store its pixels in 'secretPixels' referenced memory space.
    if (readPGMText(secret_image, secretPixels, secretWidth, secretHeight) != 0) {
        printf("Error: Unable to read secret image.\n");
        free(coverPixels);
        free(secretPixels);
        return 1;
    }

    // Check if the dimensions of both images match, exit if they don't.
    if (coverWidth != secretWidth || coverHeight != secretHeight) {
        printf("Error: Image dimensions do not match.\n");
        free(coverPixels);
        free(secretPixels);
        return 1;
    }
 
    // Call the function embedLSB to embed the secret image into the cover image.
    embedLSB(coverPixels, secretPixels, WIDTH, HEIGHT);
        
    // Save the stego image into binary format to stego_image_bin.pgm
    if (writePGMBinary(stego_image, coverPixels, WIDTH, HEIGHT) != 0) {
        printf("Error: Unable to save stego image.\n");
        free(coverPixels);
        free(secretPixels);
        return 1;
    }
  
    //  Using malloc, dynamically request exact memory space, referenced by 'outputPixels', to store the extracted secret image
    outputPixels = (unsigned char *)malloc(WIDTH * HEIGHT);
    if (outputPixels == NULL){
        printf("Error: Memory allocation for outputPixels failed.\n");
        free(coverPixels);
        free(secretPixels);
        return 1;
    } 

    // Call the function extractLSB to extract the secret image from the stego image
    extractLSB(coverPixels, outputPixels, WIDTH, HEIGHT);
    
    // Save the extracted secret image in a text format to extracted_secret.pgm
    if (writePGMText(extracted_secret, outputPixels, WIDTH, HEIGHT) != 0){
        printf("Error: Unable to save extracted secret image.\n");
    }

    free(coverPixels);
    free(secretPixels);
    free(outputPixels);

    return 0;
}

//---------------------------------------------------------------

// Function to Read a PGM image in Text Format using ASCII Codes
int readPGMText(const char *filename, unsigned char *pixels, int width, int height) {
    
    //Opening the File
    FILE *readFile = fopen(filename, "r");

    //Error Handling for Reading File
    if (readFile==NULL){
        return 1;
    }

    //Checking for P2 which is the ASCII Magic Number for Text Format
    char buffer[16];
        if (fgets(buffer, sizeof(buffer), readFile) == NULL || buffer[0] != 'P' || buffer[1] != '2') {
            fclose(readFile);
        return 1;
    }

    //Skipping Comments
    while(buffer[0] == '#'){
        if (fgets(buffer, sizeof(buffer), readFile) == NULL) {
            fclose(readFile);
            return 1;
        }
        continue;
    }

    //Obtain the Dimensions
    int w, h, maxVal;
    sscanf(buffer, "%d %d", &w, &h);

    //Checking if the Dimensions Match the Requirement
    if(fscanf(readFile, "%d", &maxVal)!=1 || w!=width || h!=height || maxVal!=255){
        fclose(readFile);//close file
        return 1;
    }

    //Loop for Reading the Pixel Data
    for (int i = 0; i < width * height; i++) {
        if (fscanf(readFile, "%hhu", &pixels[i]) != 1) {
            fclose(readFile);
            return 1;
        }
    }

    //Closing the File
    fclose(readFile);
    return 0;
}

// Function to Write a PGM image in Text Format
int writePGMText(const char *filename, unsigned char *pixels, int width, int height) {
    
    //Opening the File
    FILE *writeFile = fopen(filename, "w");

    //Error Handling for Writing File
    if (writeFile==NULL){
        return 1;
    }

    //Printing Header
    fprintf(writeFile,"P2");

    //Write the Pixel Data
    for(int i=0;i<width*height;i++) {
        fprintf(writeFile,"%d\n", pixels[i]);
    }

    //Closing the File
    fclose(writeFile);
    return 0;

}


// Function to Write a PGM image in Binary Format
int writePGMBinary(const char *filename, unsigned char *pixels, int width, int height) {

    //Opening the File
    FILE *writeBinFile = fopen(filename, "wb");

    //Error Handling for Writing File
    if(writeBinFile==NULL){
        return 1;
    }

    //Write the Header
    fprintf(writeBinFile, "P5\n# Created by stego_lsb.c\n%d %d\n255\n", width, height); 

    //Writing the Pixels
    fwrite(pixels, sizeof(unsigned char), width * height, writeBinFile);

    //Closing the File
    fclose(writeBinFile);
    return 0;

}

// Function to hide a secret image using the 4-bit LSB steganography algorithm.
void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels, int width, int height) {
    //Putting Upper 4 Bits into the Lower 4 Bits of Pixels
    for(int i=0;i<width*height;i++){ 
        coverPixels[i]=(coverPixels[i]/16)*16+(secretPixels[i]/16); 
    }
}

// Function to Extract the Secret Image using 4-LSB Steganography Algorithm.
void extractLSB(unsigned char *coverPixels, unsigned char *outputPixels, int width, int height) {
    //Extracting and Reconstructing Secret Pixels
    for(int i=0;i<width*height;i++){
        outputPixels[i]=(coverPixels[i]%16)*16; //after extraction reconstrcution of secret pixel
    }
}
