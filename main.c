#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct {
    char* buffer;
    size_t bufferLength;
    ssize_t inputLength;
} InputBuffer;

InputBuffer* createInputBuffer() {
    InputBuffer* inputBuffer = (InputBuffer*) malloc(sizeof(InputBuffer));
    inputBuffer -> buffer = NULL;
    inputBuffer -> bufferLength = 0;
    inputBuffer -> inputLength = 0;
    return inputBuffer;
}

void closeInputBuffer(InputBuffer* inputBuffer) {
    free(inputBuffer -> buffer);
    free(inputBuffer);
}

void printPrompt() {
    printf("ninja > ");
}

void readInput(InputBuffer* inputBuffer) {
    ssize_t bytesRead = getline(&(inputBuffer -> buffer), &(inputBuffer -> bufferLength), stdin);

    if (bytesRead <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    inputBuffer -> inputLength = bytesRead - 1;
    inputBuffer -> buffer[bytesRead - 1] = 0;
}

int main(int argc, char* argv[]) {
    InputBuffer* inputBuffer = createInputBuffer();
    for (;;) {
        printPrompt();
        readInput(inputBuffer);

        if (strcmp(inputBuffer -> buffer, "quit") == 0) {
            closeInputBuffer(inputBuffer);
            exit(EXIT_SUCCESS);
        } else {
            printf("Unrecognized command '%s'.\n", inputBuffer -> buffer);
        }
    }
}