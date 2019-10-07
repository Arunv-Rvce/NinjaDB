#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "constants.c"

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

MetaCommandResult createMetaCommand(InputBuffer* inputBuffer) {
    if (strcmp(inputBuffer -> buffer, ".exit") == 0) {
        closeInputBuffer(inputBuffer);
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED;
    }
}

PrepareResult prepareStatement(InputBuffer* inputBuffer, Statement* statement) {
    if (strncmp(inputBuffer -> buffer, "insert", 6) == 0) {
        statement -> type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (strncmp(inputBuffer -> buffer, "select", 6) == 0) {
        statement -> type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void executeStatement(Statement *statement) {
    switch (statement -> type) {
        case STATEMENT_INSERT:
            printf("Insert Statment");
            break;
        case STATEMENT_SELECT:
            printf("Select Statement");
            break;
    }
}

int main(int argc, char* argv[]) {
    InputBuffer* inputBuffer = createInputBuffer();
    for (;;) {
        printPrompt();
        readInput(inputBuffer);

        if(inputBuffer -> buffer[0] == '.') {
            switch (createMetaCommand(inputBuffer)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED:
                    printf("Unrecognized command '%s'\n", inputBuffer -> buffer);
                    exit(EXIT_FAILURE);
            }
        }

        Statement statement;
        switch (prepareStatement(inputBuffer, &statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at start of '%s'.\n", inputBuffer -> buffer);
                continue;
        }

        executeStatement(&statement);
    }
}