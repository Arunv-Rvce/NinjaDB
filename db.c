#include "fileOperations.c"

uint32_t* leafNodeNCells(void* node) {
    return (uint32_t*) (node + LEAF_NODE_NUM_CELLS_OFFSET);
}

void initializeLeafNode(void* node) { *leafNodeNCells(node) = 0; }

void pagerFlush(Pager* pager, uint32_t pageNum) {
    if (pager -> pages[pageNum] == NULL) {
        printf("Tried to flush null page\n");
        exit(EXIT_FAILURE);
    }
    off_t offset = lseek(pager -> fileDescriptor, pageNum * PAGE_SIZE, SEEK_SET);

    if (offset == -1) {
        printf("Error seeking: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    ssize_t bytesWritten = write(pager -> fileDescriptor, pager->pages[pageNum], PAGE_SIZE);

    if (bytesWritten == -1) {
        printf("Error writing: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}

Table* openDB(const char* filename) {
    Pager* pager = pagerOpen(filename);
    uint32_t num_rows = pager -> fileLength / ROW_SIZE;

    Table* table = (Table*) malloc(sizeof(Table));
    table -> pager = pager;

    table-> rootPageNum = 0;

    if (pager -> numPages == 0) {
        // New database file. Initialize page 0 as leaf node.
        void* rootNode = getPage(pager, 0);
        initializeLeafNode(rootNode);
    }

    return table;
}

void closeDB(Table* table) {
    Pager* pager = table -> pager;

    for (uint32_t i = 0; i < pager -> numPages; i++) {
        if (pager->pages[i] != NULL) {
            pagerFlush(pager, i);
            free(pager -> pages[i]);
            pager -> pages[i] = NULL;
        }
    }

    int result = close(pager -> fileDescriptor);
    if (result == -1) {
        printf("Error closing db file.\n");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        void* page = pager -> pages[i];
        if (page) {
            free(page);
            pager->pages[i] = NULL;
        }
    }
    free(pager);
    free(table);
}