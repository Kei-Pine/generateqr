#include <stdio.h>
#include <stdlib.h>
#include <string.h>     
#include <qrencode.h>


int main() {
    char input[256];
    printf("Enter text to generate QR code: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // Remove newline character

    QRcode* qr = QRcode_encodeString(input, 0, QR_ECLEVEL_Q, QR_MODE_8, 1);

    if (qr != NULL) {
        for (int y = 0; y < qr->width; ++y) {
            for (int x = 0; x < qr->width; ++x) {
                char module = qr->data[y * qr->width + x] & 1 ? '#' : ' ';
                printf("%c%c", module, module);
            }
            printf("\n");
        }
        QRcode_free(qr);
    } else {
        fprintf(stderr, "Failed to generate QR code.\n");
        return 1;
    }

    return 0;
}
