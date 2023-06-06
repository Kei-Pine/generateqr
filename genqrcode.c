#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qrencode.h>
#include <png.h>


void saveQRCodeAsPNG(const char* filename, const QRcode* qr) {
    int width = qr->width;
    int size = width * width;
    unsigned char* data = qr->data;

    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Failed to open file for writing: %s\n", filename);
        return;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fprintf(stderr, "Failed to create PNG write structure\n");
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fprintf(stderr, "Failed to create PNG info structure\n");
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, width, width, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    png_bytep row = malloc(width);
    if (!row) {
        fprintf(stderr, "Failed to allocate memory\n");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return;
    }

    for (int y = 0; y < width; ++y) {
        for (int x = 0; x < width; ++x) {
            row[x] = (data[y * width + x] & 1) ? 0 : 255;
        }
        png_write_row(png_ptr, row);
    }

    free(row);

    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
}

//PNG section is above

int main() {
    char input[256];
    printf("Enter text to generate QR code: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // Remove newline character

    QRcode* qr = QRcode_encodeString(input, 0, QR_ECLEVEL_Q, QR_MODE_8, 1);
    if (qr != NULL) {
        saveQRCodeAsPNG("qrcode.png", qr);
        QRcode_free(qr);
        printf("QR code saved as qrcode.png\n");
    } else {
        fprintf(stderr, "Failed to generate QR code.\n");
        return 1;
    }

    return 0;
}


