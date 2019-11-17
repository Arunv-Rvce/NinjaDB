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

uint32_t* leafNodeNumCells(void* node) {
    return (uint32_t*) (node + LEAF_NODE_NUM_CELLS_OFFSET);
}

void* leafNodeCell(void* node, uint32_t cell_num) {
    return node + LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE;
}

uint32_t* leafNodeKey(void* node, uint32_t cell_num) {
    return (uint32_t*) leafNodeCell(node, cell_num);
}

void* leafNodeValue(void* node, uint32_t cell_num) {
    return leafNodeCell(node, cell_num) + LEAF_NODE_KEY_SIZE;
}


Cursor* tableStart(Table* table) {
    Cursor* cursor = (Cursor*) malloc(sizeof(Cursor));
    cursor -> table = table;
    cursor -> pageNum = table -> rootPageNum;
    cursor -> cellNum = 0;

    void* root_node = getPage(table -> pager, table -> rootPageNum);
    uint32_t num_cells = *leafNodeNumCells(root_node);
    cursor -> endOfTable = (num_cells == 0);
    return cursor;
}

Cursor* tableEnd(Table* table) {
    Cursor* cursor = (Cursor*) malloc(sizeof(Cursor));
    cursor -> table = table;
    cursor -> pageNum = table -> rootPageNum;
    void* root_node = getPage(table -> pager, table -> rootPageNum);
    uint32_t num_cells = *leafNodeNumCells(root_node);
    cursor -> cellNum = num_cells;
    cursor -> endOfTable = true;
    return cursor;
}

void* cursorValue(Cursor* cursor) {
    uint32_t pageNum = cursor -> pageNum;
    void* page = getPage(cursor -> table -> pager, pageNum);
    return leafNodeValue(page, cursor -> cellNum);
}

void cursorAdvance(Cursor* cursor) {
    uint32_t page_num = cursor -> pageNum;
    void* node = getPage(cursor->table->pager, page_num);
    cursor -> cellNum += 1;
    if (cursor -> cellNum >= (*leafNodeNumCells(node))) {
        cursor->endOfTable = true;
    }
}

void printConstants() {
    printf("ROW_SIZE: %d\n", ROW_SIZE);
    printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
    printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
    printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
    printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
    printf("LEAF_NODE_MAX_CELLS: %d\n", LEAF_NODE_MAX_CELLS);
}

void printLeafNode(void* node) {
    uint32_t numCells = *leafNodeNumCells(node);
    printf("leaf (size %d)\n", numCells);
    for (uint32_t i = 0; i < numCells; i++) {
        uint32_t key = *leafNodeKey(node, i);
        printf("  - %d : %d\n", i, key);
    }
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
    } else if (strcmp(inputBuffer -> buffer, ".btree") == 0) {
        printf("Tree:\n");
        printLeafNode(getPage(table->pager, 0));
        return META_COMMAND_SUCCESS;
    } else if (strcmp(inputBuffer -> buffer, ".constants") == 0) {
        printf("Constants:\n");
        printConstants();
        return META_COMMAND_SUCCESS;
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

void leafNodeInsert(Cursor* cursor, uint32_t key, Row* value) {
    void* node = getPage(cursor -> table -> pager, cursor -> pageNum);
    uint32_t num_cells = *leafNodeNumCells(node);
    if (num_cells >= LEAF_NODE_MAX_CELLS) {
        // Node full
        printf("Need to implement splitting a leaf node.\n");
        exit(EXIT_FAILURE);
    }

    if (cursor -> cellNum < num_cells) {
        // Make room for new cell
        for (uint32_t i = num_cells; i > cursor -> cellNum; i--) {
            memcpy(leafNodeCell(node, i), leafNodeCell(node, i - 1), LEAF_NODE_CELL_SIZE);
        }
    }

    *(leafNodeNumCells(node)) += 1;
    *(leafNodeKey(node, cursor -> cellNum)) = key;
    serializeRow(value, leafNodeValue(node, cursor -> cellNum));
}

ExecuteResult executeInsert(Statement* statement, Table* table) {
    void* node = getPage(table -> pager, table -> rootPageNum);
    if ((*leafNodeNumCells(node) >= LEAF_NODE_MAX_CELLS)) {
        return EXECUTE_TABLE_FULL;
    }

    Row* rowToInsert = &(statement -> rowToInsert);
    Cursor* cursor = tableEnd(table);

    leafNodeInsert(cursor, rowToInsert -> id, rowToInsert);
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