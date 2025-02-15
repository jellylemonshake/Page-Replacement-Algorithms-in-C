#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PAGES 100

bool isPagePresent(int page, int frames[], int n) {
    for (int i = 0; i < n; i++)
        if (frames[i] == page)
            return true;
    return false;
}

int getLRUPosition(int pages[], int frames[], int curr_pos, int n) {
    int pos = -1, farthest = curr_pos;
    for (int i = 0; i < n; i++) {
        int j;
        for (j = curr_pos - 1; j >= 0; j--) {
            if (frames[i] == pages[j]) {
                if (j < farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }
        if (j == -1) return i;
    }
    return (pos == -1) ? 0 : pos;
}

int getOptimalPosition(int pages[], int frames[], int curr_pos, int n, int total_pages) {
    int pos = -1, farthest = curr_pos;
    for (int i = 0; i < n; i++) {
        int j;
        for (j = curr_pos + 1; j < total_pages; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }
        if (j == total_pages) return i;
    }
    return (pos == -1) ? 0 : pos;
}

void printTableHeader(int frames_count) {
    printf("+--------+-----------+");
    for (int i = 0; i < frames_count; i++) printf("--------+");
    printf("\n|  Page  |   Status  |");
    for (int i = 0; i < frames_count; i++) printf(" Frame%d |", i + 1);
    printf("\n+--------+-----------+");
    for (int i = 0; i < frames_count; i++) printf("--------+");
    printf("\n");
}

void printTableRow(int page, bool fault, int frames[], int frames_count) {
    printf("| %4d   | %-9s |", page, fault ? "Miss" : "Hit");
    for (int i = 0; i < frames_count; i++) {
        if (frames[i] == -1) printf("   -    |");
        else printf("   %d    |", frames[i]);
    }
    printf("\n");
}

void printTableFooter(int frames_count) {
    printf("+--------+-----------+");
    for (int i = 0; i < frames_count; i++) printf("--------+");
    printf("\n");
}

int FIFO(int pages[], int total_pages, int frames_count) {
    printf("\nFIFO Page Replacement Algorithm\n");
    int frames[frames_count], page_faults = 0, current = 0;
    for (int i = 0; i < frames_count; i++) frames[i] = -1;

    printTableHeader(frames_count);
    for (int i = 0; i < total_pages; i++) {
        bool fault = !isPagePresent(pages[i], frames, frames_count);
        if (fault) {
            frames[current] = pages[i];
            current = (current + 1) % frames_count;
            page_faults++;
        }
        printTableRow(pages[i], fault, frames, frames_count);
    }
    printTableFooter(frames_count);

    int page_hits = total_pages - page_faults;
    printf("Total Page Hits: %d/%d\n", page_hits, total_pages);
    printf("Total Page Miss: %d/%d\n", page_faults, total_pages);
    return page_faults;
}

int LRU(int pages[], int total_pages, int frames_count) {
    printf("\nLRU Page Replacement Algorithm\n");
    int frames[frames_count], page_faults = 0;
    for (int i = 0; i < frames_count; i++) frames[i] = -1;

    printTableHeader(frames_count);
    for (int i = 0; i < total_pages; i++) {
        bool fault = !isPagePresent(pages[i], frames, frames_count);
        if (fault) {
            int pos = getLRUPosition(pages, frames, i, frames_count);
            frames[pos] = pages[i];
            page_faults++;
        }
        printTableRow(pages[i], fault, frames, frames_count);
    }
    printTableFooter(frames_count);

    int page_hits = total_pages - page_faults;
    printf("Total Page Hits: %d/%d\n", page_hits, total_pages);
    printf("Total Page Miss: %d/%d\n", page_faults, total_pages);
    return page_faults;
}

int Optimal(int pages[], int total_pages, int frames_count) {
    printf("\nOptimal Page Replacement Algorithm\n");
    int frames[frames_count], page_faults = 0;
    for (int i = 0; i < frames_count; i++) frames[i] = -1;

    printTableHeader(frames_count);
    for (int i = 0; i < total_pages; i++) {
        bool fault = !isPagePresent(pages[i], frames, frames_count);
        if (fault) {
            int pos = getOptimalPosition(pages, frames, i, frames_count, total_pages);
            frames[pos] = pages[i];
            page_faults++;
        }
        printTableRow(pages[i], fault, frames, frames_count);
    }
    printTableFooter(frames_count);

    int page_hits = total_pages - page_faults;
    printf("Total Page Hits: %d/%d\n", page_hits, total_pages);
    printf("Total Page Miss: %d/%d\n", page_faults, total_pages);
    return page_faults;
}

void findMostEfficientMethod(int pages[], int total_pages, int frames_count) {
    int fifo_faults = FIFO(pages, total_pages, frames_count);
    int lru_faults = LRU(pages, total_pages, frames_count);
    int optimal_faults = Optimal(pages, total_pages, frames_count);

    printf("\n--- Efficiency Analysis ---\n");
    if (fifo_faults == lru_faults && lru_faults == optimal_faults) {
        printf("All methods have the same faults. Choosing Optimal as the most efficient.\n");
    } else if (fifo_faults <= lru_faults && fifo_faults <= optimal_faults) {
        printf("Most Efficient Method: FIFO with %d page faults\n", fifo_faults);
    } else if (lru_faults <= fifo_faults && lru_faults <= optimal_faults) {
        printf("Most Efficient Method: LRU with %d page faults\n", lru_faults);
    } else {
        printf("Most Efficient Method: Optimal with %d page faults\n", optimal_faults);
    }
}

int main() {
    int pages[MAX_PAGES], total_pages, new_pages, frames_count, i;

    printf("Enter the number of frames: ");
    scanf("%d", &frames_count);

    printf("Enter the number of pages: ");
    scanf("%d", &total_pages);

    printf("Enter the page reference sequence: ");
    for (i = 0; i < total_pages; i++)
        scanf("%d", &pages[i]);

    findMostEfficientMethod(pages, total_pages, frames_count);

    char choice;
    while (1) {
        printf("\nDo you want to add more pages? (y/n): ");
        scanf(" %c", &choice);
        if (choice == 'n' || choice == 'N') break;

        printf("\nEnter the number of new pages: ");
        scanf("%d", &new_pages);

        printf("Enter the new pages: ");
        for (i = 0; i < new_pages; i++)
            scanf("%d", &pages[total_pages + i]);

        total_pages += new_pages;

        findMostEfficientMethod(pages, total_pages, frames_count);
    }

    return 0;
}
