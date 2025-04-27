#include <stdlib.h>

int main(){
	int *ptr = malloc(100 * sizeof(int));
	ptr[0] = 43;
	return 0;
}
