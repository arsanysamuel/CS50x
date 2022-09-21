#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int BLOCK_SIZE = 512;
uint8_t JPEG_SIG[] = {0xff, 0xd8, 0xff};  // jpeg file beginning signature (first 3 bytes)

int main(int argc, char *argv[])
{
    // Accepting one command-line arg
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Opening SD image file
    FILE *sd_img = fopen(argv[1], "r");
    if (sd_img == NULL)
    {
        printf("Invalid file of file doesn't exist.\n");
        return 1;
    }

    // Reading file and recovering
    int file_count = 0;
    FILE *jpeg_file = NULL;  // must be initialized with a value
    char *jpeg_filename = malloc(8 * sizeof(char));  // don't forget the NUL character
    uint8_t *block = malloc(BLOCK_SIZE * sizeof(uint8_t));

    while (fread(block, sizeof(uint8_t), BLOCK_SIZE, sd_img) >= BLOCK_SIZE)
    {
        if (block[0] == JPEG_SIG[0] && block[1] == JPEG_SIG[1] && block[2] == JPEG_SIG[2] && block[3] >= 0xe0 && block[3] <= 0xef)
        {
            if (jpeg_file != NULL)  // closing if file opened
            {
                fclose(jpeg_file);
            }
            sprintf(jpeg_filename, "%03d.jpg", file_count);  // for zero-padded filename, eg. 001.jpeg
            jpeg_file = fopen(jpeg_filename, "w");
            file_count++;
        }

        // Writing the block to the file
        if (jpeg_file != NULL)
        {
            fwrite(block, sizeof(uint8_t), BLOCK_SIZE, jpeg_file);
        }
    }

    // Deallocating
    free(block);
    free(jpeg_filename);
    fclose(sd_img);
    if (jpeg_file != NULL)  // closing if file opened
    {
        fclose(jpeg_file);
    }
}
