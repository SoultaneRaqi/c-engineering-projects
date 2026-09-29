#include <stdio.h>
#include <stdlib.h> // REQUIRED for malloc() and free()

// 1. Define the Linked List Node
struct SnakeNode {
    int x;
    int y;
    struct SnakeNode *next; // Pointer to the next body piece
};

int main() {
    // 2. We don't create the struct directly on the stack anymore.
    // We create a POINTER to hold the memory address.
    struct SnakeNode *head = NULL;

    // 3. malloc (Memory Allocate)
    // We ask the OS: "Give me exactly enough bytes to fit a SnakeNode struct."
    // malloc returns the memory address, which we save in 'head'.
    head = malloc(sizeof(struct SnakeNode));

    // Always check if the OS actually gave us the memory!
    if (head == NULL) {
        printf("Memory allocation failed!\n");
        return 1; // Crash gracefully
    }

    // 4. Initialize the Head node's data using the arrow operator
    head->x = 5;
    head->y = 5;
    head->next = NULL; // It's just a head right now, there is no tail.

    printf("Snake spawned at X:%d, Y:%d\n", head->x, head->y);

    // 5. CRITICAL: The Garbage Collector does not exist!
    // We must return the memory to the OS before the program ends.
    free(head);
    printf("Memory freed. Program exiting cleanly.\n");

    return 0;
}