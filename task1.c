#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main() {
    float value;
    int invalid_count = 0;
    int valid_count = 0;
    char buffer[256];

    const char *inputFile = "C:\\Users\\anany\\OneDrive\\Desktop\\Coding\\C Programming\\CSCI291 Work\\Lab 4\\data.txt";
    const char *outputFile = "valid_data.txt";

    FILE *readFile = fopen(inputFile, "r");
    FILE *writeFile = fopen(outputFile, "w");

    if (inputFile == NULL) {
        perror("Error opening input file");
    }

    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(readFile);
    }

    while (1) {
        int curr_pos = ftell(readFile);     //Getting the current position
        //If reading is valid
        if (fscanf(readFile, "%255s", buffer) == 1) {
            char *pointer;
            float value = strtof(buffer, &pointer);
            //If the character is decimal
            if (*pointer == '\0') {
                fprintf(writeFile, "%f\n", value);
                valid_count++;
            }
            //If the character is not a decimal
            else {
                fseek(readFile, curr_pos + strlen(buffer) + 1, SEEK_SET);
                invalid_count++;
            }
        }
        //If reading is invalid
        else{
            //Checking for end of file
            if (feof(readFile)) {
                break; // End of file reached
            }
            //Error Handling
            if (ferror(readFile)) {
                perror("Error reading input file");
                break;
            }
        }

    }
    
    fclose(readFile);
    fclose(writeFile);
    printf("The total number of invalid float values: %d\n", invalid_count);
    printf("The total number of valid float values: %d\n", valid_count);
}

