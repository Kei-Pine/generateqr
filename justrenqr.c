#include <stdio.h>
#include <stdlib.h>

int renameFile() {
    char oldName[] = "qrcode.png";
    char newName[100];

    printf("Enter the new file name: ");
    scanf("%99s", newName);  // Limit input to prevent buffer overflow

    // Construct the command to rename the file
    char command[300];  // Increase buffer size to accommodate long file names
    snprintf(command, sizeof(command), "mv %s %s.png", oldName, newName);

    // Execute the command using the system() function
    int status = system(command);

    if (status == 0) {
        printf("File renamed successfully.\n");
    } else {
        printf("File renaming failed.\n");
    }

    return 0;
}

int main() {
    renameFile();
    return 0;
}
