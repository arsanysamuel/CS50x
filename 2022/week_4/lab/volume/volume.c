// Modifies the volume of an audio file

#include <stdint.h>  // for defining extra int types
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Copy header from input file to output file
    uint8_t *header = malloc(HEADER_SIZE * sizeof(uint8_t));
    fread(header, sizeof(uint8_t), HEADER_SIZE, input);  // Reading from input file into the header
    fwrite(header, sizeof(uint8_t), HEADER_SIZE, output);  // Writing header to the output file
    free(header);

    // Read samples from input file and write updated data to output file
    int16_t buffer;

    // reading into sample, fread returns a number smaller than arg 3 if error or end of file
    while (fread(&buffer, sizeof(int16_t), 1, input))
    {
        buffer *= factor;
        fwrite(&buffer, sizeof(int16_t), 1, output);  // writing the sample to the file, passing the address of the buffer
    }

    // Close files
    fclose(input);
    fclose(output);
}
