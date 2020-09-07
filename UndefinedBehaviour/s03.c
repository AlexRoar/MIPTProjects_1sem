#include <stdio.h>


int main(){
	long long unsigned u, l;
	scanf("%llu%llu", &u, &l);
	printf("%llu >> %llu = %llu\n", u, l, u>>l);
}
