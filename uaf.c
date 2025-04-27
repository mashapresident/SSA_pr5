#include <stdlib.h>
#include <stdio.h>

int main(){
	int *p = malloc(sizeof(int));
	*p = 10;
	free(p);
	*p = 20;
	return 0;
}
