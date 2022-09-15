#include <stdio.h>
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <webp/decode.h>

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: webp2png <input> <output>\r\n\r\n");
        return EXIT_FAILURE;
    }

    const char *input_file = (const char *)argv[1];
    const char *output_file = (const char *)argv[2];

    int ret_val = EXIT_SUCCESS;

    FILE *in = fopen(input_file, "rb");
    if (!in)
    {
        fprintf(stderr, "Failed to open input file!\r\n");
        return EXIT_FAILURE;
    }

    fseek(in, 0, SEEK_END);
    long file_size = ftell(in);
    fseek(in, 0, SEEK_SET);

    char *input_buffer = (char *)malloc((size_t)file_size);
    if (!input_buffer)
    {
        ret_val = EXIT_FAILURE;
        goto finish;
    }

    size_t read_result = fread(input_buffer, file_size, 1, in);
    if (read_result < 0)
    {
        fprintf(stderr, "Failed to read file!");
        ret_val = EXIT_FAILURE;
        goto finish;
    }

    int width = 0;
    int height = 0;
    if (!WebPGetInfo((const uint8_t *)input_buffer, file_size, &width, &height))
    {
        fprintf(stderr, "Failed to get WebP info!\r\n");

        ret_val = EXIT_FAILURE;
        goto finish;
    }

    uint8_t *output_buffer = WebPDecodeRGBA((const uint8_t *)input_buffer, file_size, &width, &height);
    if (!output_buffer)
    {
        fprintf(stderr, "Failed to decode WebP!\r\n");

        ret_val = EXIT_FAILURE;
        goto webp_finish;
    }

    png_image w_image;
    memset(&w_image, 0, sizeof(w_image));

    w_image.version = PNG_IMAGE_VERSION;
    w_image.width = width;
    w_image.height = height;
    w_image.format = PNG_FORMAT_RGBA;

    int write_result = png_image_write_to_file(&w_image, output_file, 0, output_buffer, 2000, NULL);
    if (!write_result)
    {
        fprintf(stderr, "Failed to write PNG image!\r\n");
        ret_val = EXIT_FAILURE;
    }

    png_image_free(&w_image);

webp_finish:
    WebPFree(output_buffer);

finish:
    free(input_buffer);
    fclose(in);

    return ret_val;
}