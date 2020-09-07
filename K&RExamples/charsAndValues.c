#include <stdio.h>

int main(){

	for (int i = -127; i < 255; i++){
		printf("%c => %d\n", i, i);
	}
}
