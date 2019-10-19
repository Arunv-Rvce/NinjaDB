#include "fileOperations.c"

void pagerFlush(Pager* pager, uint32_t pageNum, uint32_t size) {
    if (pager -> pages[pageNum] == NULL) {
        printf("Tried to flush null page\n");
        exit(EXIT_FAILURE);
    }
    off_t offset = lseek(pager -> fileDescriptor, pageNum * PAGE_SIZE, SEEK_SET);

    if (offset == -1) {
        printf("Error seeking: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    ssize_t bytesWritten =  write(pager -> fileDescriptor, pager -> pages[pageNum], size);

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
    table -> numRows = num_rows;

    return table;
}

void closeDB(Table* table) {
    Pager* pager = table -> pager;
    uint32_t numFullPages = table -> numRows / ROWS_PER_PAGE;

    for (uint32_t i = 0; i < numFullPages; i++) {
        if (pager->pages[i] != NULL) {
            pagerFlush(pager, i, PAGE_SIZE);
            free(pager -> pages[i]);
            pager -> pages[i] = NULL;
        }
    }
    // There may be a partial page to write to the end of the file
    // This should not be needed after we switch to a B-tree
    uint32_t numAdditionalRows = table -> numRows % ROWS_PER_PAGE;
    if (numAdditionalRows > 0) {
        uint32_t pageNum = numFullPages;
        if (pager -> pages[pageNum] != NULL) {
            pagerFlush(pager, pageNum, numAdditionalRows * ROW_SIZE);
            free(pager -> pages[pageNum]);
            pager -> pages[pageNum] = NULL;
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