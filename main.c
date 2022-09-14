#include <stdio.h>
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <webp/decode.h>

int main()
{
    int ret_val = EXIT_SUCCESS;

    FILE *in = fopen("reona_unknown.webp", "rb");
    if (!in)
    {
        fprintf(stderr, "Failed to open input file!\r\n");
        abort();
    }

    fseek(in, 0, SEEK_END);
    long file_size = ftell(in);
    fseek(in, 0, SEEK_SET);

    printf("File size: %ld\r\n", file_size);

    char *input_buffer = (char *)malloc((size_t)file_size);
    if (!input_buffer)
    {
        ret_val = EXIT_FAILURE;
        goto finish;
    }

    size_t read_result = fread(input_buffer, file_size, 1, in);
    printf("Read result: %lu\r\n", read_result);

    int width = 0;
    int height = 0;
    if (!WebPGetInfo((const uint8_t*)input_buffer, file_size, &width, &height))
    {
        fprintf(stderr, "Failed to get WebP info!\r\n");

        ret_val = EXIT_FAILURE;
        goto finish;
    }

    printf("Image width: %d, height: %d\r\n", width, height);

    uint8_t* output_buffer = WebPDecodeRGBA((const uint8_t*)input_buffer, file_size, &width, &height);
    if (!output_buffer)
    {
        fprintf(stderr, "Failed to decode WebP!\r\n");
        
        ret_val = EXIT_FAILURE;
        goto finish;
    }

    png_image w_image;
    memset(&w_image, 0, sizeof(w_image));

    w_image.version = PNG_IMAGE_VERSION;
    w_image.width = width;
    w_image.height = height;
    w_image.format = PNG_FORMAT_RGBA;


    int write_result = png_image_write_to_file(&w_image, "reona_unknown_w.png", 0, output_buffer, 2000, NULL);
    if (!write_result)
    {
        fprintf(stderr, "Failed to write PNG image!\r\n");
        ret_val = EXIT_FAILURE;
    }

    png_image_free(&w_image);
    WebPFree(output_buffer);

finish:
    free(input_buffer);
    fclose(in);

    return ret_val;
}