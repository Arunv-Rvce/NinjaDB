#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "db.c"

InputBuffer* createInputBuffer() {
    InputBuffer* inputBuffer = (InputBuffer*) malloc(sizeof(InputBuffer));
    inputBuffer -> buffer = NULL;
    inputBuffer -> bufferLength = 0;
    inputBuffer -> inputLength = 0;
    return inputBuffer;
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

Cursor* tableStart(Table* table) {
    Cursor* cursor = (Cursor*) malloc(sizeof(Cursor));
    cursor -> table = table;
    cursor -> rowNum = 0;
    cursor->endOfTable = (table -> numRows == 0);
    return cursor;
}

Cursor* tableEnd(Table* table) {
    Cursor* cursor = (Cursor*) malloc(sizeof(Cursor));
    cursor -> table = table;
    cursor -> rowNum = table -> numRows;
    cursor->endOfTable = true;
    return cursor;
}

void* cursorValue(Cursor* cursor) {
    uint32_t pageNum = cursor -> rowNum / ROWS_PER_PAGE;
    void* page = getPage(cursor-> table ->pager, pageNum);

    uint32_t rowOffset = cursor -> rowNum % ROWS_PER_PAGE;
    uint32_t byteOffset = rowOffset * ROW_SIZE;
    return page + byteOffset;
}

void cursorAdvance(Cursor* cursor) {
    cursor -> rowNum += 1;
    if (cursor -> rowNum >= cursor -> table -> numRows)
        cursor -> endOfTable = true;
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
        closeDB(table);
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED;
    }
}



PrepareResult prepareStatement(InputBuffer* inputBuffer, Statement* statement) {
    if (strncmp(inputBuffer -> buffer, "insert", 6) == 0) {
        return prepareInsert(inputBuffer, statement);
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
    Cursor* cursor = tableEnd(table);

    serializeRow(rowToInsert, cursorValue(cursor));
    table -> numRows += 1;
    free(cursor);
    return EXECUTE_SUCCESS;
}

ExecuteResult executeSelect(Statement* statement, Table* table) {
    Cursor* cursor = tableStart(table);
    Row row;
    while (!(cursor -> endOfTable)) {
        deserializeRow(cursorValue(cursor), &row);
        printRow(&row);
        cursorAdvance(cursor);
    }
    free(cursor);
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
    if (argc < 2) {
        printf("Must supply a database filename.\n");
        exit(EXIT_FAILURE);
    }
    char* filename = argv[1];
    Table* table = openDB(filename);

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