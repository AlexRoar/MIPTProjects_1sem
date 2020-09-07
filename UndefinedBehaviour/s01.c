#include <stdio.h>


int main() {
	int a[10] = {1,2,3,4,5,6,7,8,9,10};
	int x = 0;

	scanf ("%d", &x);

	a[x] = -10;

	printf("a[%d] = %d\n", x, a[x]);	
}
