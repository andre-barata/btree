#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

int main() {
	long millis;
	int cicles = 1000000000;
	int i = 0;
	int cmp1 = 1223;
	int dummy;
	int cnt;

	cnt=0;
	cmp1 = 1223;
	i=0;
	millis = timeGetTime();
	for(i; i < cicles; i++) {
		cmp1 ++;
		if (cmp1 & 0xFFFFFC00) cnt++;
	}
	printf("bitwise-and millis: %d (cnt: %d)\n", timeGetTime() - millis, cnt);

	cnt=0;
	cmp1 = 1223;
	i=0;
	millis = timeGetTime();
	for(i; i < cicles; i++) {
		cmp1 ++;
		if (cmp1 >= 1024) cnt++;
	}
	printf("greater-equ millis: %d (cnt: %d)\n", timeGetTime() - millis, cnt);

	return 0;
}


