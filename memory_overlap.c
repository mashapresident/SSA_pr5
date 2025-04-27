#include <stdio.h>
#include <stdint.h>

void testMemoryOverlap() {
    
    union {
        int32_t int_value;
        char char_array[4];
    } shared_memory;


    printf("address of int      -> %p\n", (void *)&shared_memory.int_value);
    printf("address of char arr -> %p\n", (void *)&shared_memory.char_array);


    shared_memory.int_value = 1;

    printf("char_array values -> %x %x %x %x\n",
           shared_memory.char_array[0],
           shared_memory.char_array[1],
           shared_memory.char_array[2],
           shared_memory.char_array[3]);


    if (*(int32_t *)(shared_memory.char_array) == shared_memory.int_value) {
        printf("Conflict detected: Same memory used for different types!\n");
    } else {
        printf("No conflict detected.\n");
    }
}

int main() {
    testMemoryOverlap();
    return 0;
}
