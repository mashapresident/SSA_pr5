# SSA_pr5


## Valgrind setup and testing
>$sudo apt-get install valgrind

Для тестування створимо файли з кодом, що порушує роботу памʼяті
### leak.c
```
#include <stdlib.h>

int main(){
	int *ptr = malloc(100 * sizeof(int));
	ptr[0] = 43;
	return 0;
}
```
>$gcc -g leak.c -o leak
>
>$valgrind --leak-check=full ./leak
>
>==17753== Memcheck, a memory error detector
>
>==17753== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
>
>==17753== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
>
>==17753== Command: ./leak
>
>==17753==
>
>==17753==
>
>==17753== HEAP SUMMARY:
>
>==17753==     in use at exit: 400 bytes in 1 blocks
>
>==17753==   total heap usage: 1 allocs, 0 frees, 400 bytes allocated
>
>==17753==
>
>==17753== 400 bytes in 1 blocks are definitely lost in loss record 1 of 1
>
>==17753==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
>
>==17753==    by 0x10915E: main (leak.c:4)
>
>==17753==
>
>==17753== LEAK SUMMARY:
>
>==17753==    definitely lost: 400 bytes in 1 blocks
>
>==17753==    indirectly lost: 0 bytes in 0 blocks
>
>==17753==      possibly lost: 0 bytes in 0 blocks
>
>==17753==    still reachable: 0 bytes in 0 blocks
>
>==17753==         suppressed: 0 bytes in 0 blocks
>
>==17753==
>
>==17753== For lists of detected and suppressed errors, rerun with: -s
>
>==17753== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
>

### uaf.c
```
#include <stdlib.h>
#include <stdio.h>

int main(){
	int *p = malloc(sizeof(int));
	*p = 10;
	free(p);
	*p = 20;
	return 0;
}
```
>$gcc -g uaf.c -o uaf
>
>$valgrind ./uaf
>
>==17894== Memcheck, a memory error detector
>
>==17894== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
>
>==17894== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
>
>==17894== Command: ./uaf
>
>==17894==
>
>==17894== Invalid write of size 4
>
>==17894==    at 0x10919D: main (uaf.c:8)
>
>==17894==  Address 0x4a96040 is 0 bytes inside a block of size 4 free'd
>
>==17894==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
>
>==17894==    by 0x109198: main (uaf.c:7)
>
>==17894==  Block was alloc'd at
>
>==17894==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
>
>==17894==    by 0x10917E: main (uaf.c:5)
>
>==17894==
>
>==17894==
>
>==17894== HEAP SUMMARY:
>
>==17894==     in use at exit: 0 bytes in 0 blocks
>
>==17894==   total heap usage: 1 allocs, 1 frees, 4 bytes allocated
>
>==17894==
>
>==17894== All heap blocks were freed -- no leaks are possible
>
>==17894==
>
>==17894== For lists of detected and suppressed errors, rerun with: -s
>
>==17894== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

### double_free.c
```
#include <stdlib.h>

int main(){
	int *p = malloc(sizeof(int));
	free(p);
	free(p);
	return 0;
}
```
>$ gcc -g double_free.c -o
>
>valgrind ./double_free
>
>==18343== Memcheck, a memory error detector
>
>==18343== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
>
>==18343== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
>
>==18343== Command: ./double_free
>
>==18343==
>
>==18343== Invalid free() / delete / delete[] / realloc()
>
>==18343==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
>
>==18343==    by 0x10919A: main (double_free.c:6)
>
>==18343==  Address 0x4a96040 is 0 bytes inside a block of size 4 free'd
>
>==18343==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
>
>==18343==    by 0x10918E: main (double_free.c:5)
>
>==18343==  Block was alloc'd at
>
>==18343==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
>
>==18343==    by 0x10917E: main (double_free.c:4)
>
>==18343==
>
>==18343==
>
>==18343== HEAP SUMMARY:
>
>==18343==     in use at exit: 0 bytes in 0 blocks
>
>==18343==   total heap usage: 1 allocs, 2 frees, 4 bytes allocated
>
>==18343==
>
>==18343== All heap blocks were freed -- no leaks are possible
>
>==18343==
>
>==18343== For lists of detected and suppressed errors, rerun with: -s
>
>==18343== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)




## Завдання

### Текст завдання
Побудуйте тест, де дві змінні випадково використовують ту саму адресу пам’яті (через обхід типів) — і виявити це можливо лише за допомогою логування.
____
### *Реалізація*
```
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
```


>$gcc -g memory_overlap.c -o memory_overlap
>
>$./memory_overlap
>
>address of int      -> 0x7ffc334669f4
>
>address of char arr -> 0x7ffc334669f4
>
>char_array values -> 1 0 0 0
>
>Conflict detected: Same memory used for different types!

____
### *Пояснення*
Використовуємо уніон для спільної комірки памʼяті. Судячи з логування, помилка виникла, а саме використання спільної памʼяті. Хоча явно ця помилка існує, фреймворк Valgrind звітує наступним 

> ==19698== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

У C стандартах подібний доступ вважається "законним", і Valgrind не має механізму для ідентифікації таких програмних ситуацій як логічних конфліктів.

