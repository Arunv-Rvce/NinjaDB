#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <bits/fcntl-linux.h>
#include <fcntl.h>
#include <unistd.h>
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

void printRow(Row* row) {
    printf("(%d, %s, %s)\n", row -> id, row -> username, row -> email);
}

void serializeRow(Row* source, void* destination) {
    memcpy(destination + ID_OFFSET, &(source -> id), ID_SIZE);
    memcpy(destination + USERNAME_OFFSET, &(source -> username), USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &(source -> email), EMAIL_SIZE);
}

void deserializeRow(void* source, Row* destination) {
    memcpy(&(destination -> id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination -> username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination -> email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

void* rowSlot(Table* table, uint32_t rowNum) {
    uint32_t pageNum = rowNum / ROWS_PER_PAGE;
    void* page = getPage(table -> pager, pageNum);

    uint32_t rowOffset = rowNum % ROWS_PER_PAGE;
    uint32_t byteOffset = rowOffset * ROW_SIZE;
    return page + byteOffset;
}

Pager* pagerOpen(const char* filename) {
    int fd = open(filename,  O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
    if (fd == -1) {
        printf("Unable to open file\n");
        exit(EXIT_FAILURE);
    }

    off_t fileLength = lseek(fd, 0, SEEK_END);
    Pager* pager = malloc(sizeof(Pager));
    pager -> fileDescriptor = fd;
    pager -> fileLength = fileLength;

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        pager->pages[i] = NULL;
    }

    return pager;
}

Table* openDB(const char* filename) {
    Pager* pager = pagerOpen(filename);
    uint32_t num_rows = pager -> fileLength / ROW_SIZE;
}

void freeTable(Table* table) {
    for (int i = 0; table -> pages[i]; i++) {
        free(table -> pages[i]);
    }
    free(table);
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

MetaCommandResult createMetaCommand(InputBuffer* inputBuffer, Table* table) {
    if (strcmp(inputBuffer -> buffer, ".exit") == 0) {
        closeInputBuffer(inputBuffer);
        freeTable(table);
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED;
    }
}

PrepareResult prepare_insert(InputBuffer* inputBuffer, Statement* statement) {
    statement->type = STATEMENT_INSERT;

    char* keyword = strtok(inputBuffer->buffer, " ");
    char* idStr = strtok(NULL, " ");
    char* username = strtok(NULL, " ");
    char* email = strtok(NULL, " ");

    if (idStr == NULL || username == NULL || email == NULL) {
        return PREPARE_SYNTAX_ERROR;
    }

    int id = atoi(idStr);
    if (id < 0) {
        return PREPARE_NEGATIVE_ID;
    }

    if (strlen(username) > COLUMN_USERNAME_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }

    if (strlen(username) > COLUMN_USERNAME_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }

    statement->rowToInsert.id = id;
    strcpy(statement->rowToInsert.username, username);
    strcpy(statement->rowToInsert.email, email);
    return PREPARE_SUCCESS;
}

PrepareResult prepareStatement(InputBuffer* inputBuffer, Statement* statement) {
    if (strncmp(inputBuffer -> buffer, "insert", 6) == 0) {
        return prepare_insert(inputBuffer, statement);
    }
    if (strncmp(inputBuffer -> buffer, "select", 6) == 0) {
        statement -> type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult executeInsert(Statement* statement, Table* table) {
    if (table -> numRows >= TABLE_MAX_ROWS)
        return EXECUTE_TABLE_FULL;

    Row* rowToInsert = &(statement -> rowToInsert);

    serializeRow(rowToInsert, rowSlot(table, table -> numRows));
    table -> numRows += 1;
    return EXECUTE_SUCCESS;
}

ExecuteResult executeSelect(Statement* statement, Table* table) {
    Row row;
    for (uint32_t i = 0; i < table -> numRows; i++) {
        deserializeRow(rowSlot(table, i), &row);
        printRow(&row);
    }
    return EXECUTE_SUCCESS;
}

ExecuteResult executeStatement(Statement *statement, Table* table) {
    switch (statement -> type) {
        case STATEMENT_INSERT:
            return executeInsert(statement, table);
        case STATEMENT_SELECT:
            return executeSelect(statement, table);
    }
}

int main(int argc, char* argv[]) {
    Table* table = getNewTable();
    InputBuffer* inputBuffer = createInputBuffer();
    for (;;) {
        printPrompt();
        readInput(inputBuffer);

        if(inputBuffer -> buffer[0] == '.') {
            switch (createMetaCommand(inputBuffer, table)) {
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
            case PREPARE_NEGATIVE_ID:
                printf("ID must be positive.\n");
                continue;
            case PREPARE_STRING_TOO_LONG:
                printf("String is too long.\n");
                continue;
            case PREPARE_SYNTAX_ERROR:
                printf("Syntax error. Could not parse statement.\n");
                continue;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at start of '%s'.\n", inputBuffer -> buffer);
                continue;
        }

        switch (executeStatement(&statement, table)) {
            case EXECUTE_SUCCESS:
                printf("Executed.\n");
                break;
            case EXECUTE_TABLE_FULL:
                printf("Error: Table full.\n");
                break;
        }
    }
}