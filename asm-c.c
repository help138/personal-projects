#include <stdio.h>

#define TEST(x) (x ? 'r': 'n')

typedef struct string {
	char* arr;
	int len;
} string;

int bondCheck(string* s, int index){
}
int main(int c, char** v){
	putc(10, stdout);

	if(c != 1){
		for(int i = 1; i < c; i++){
			printf("%d: %s\n", i, v[i]);
		}
	}
	
	putc(TEST(0), stdout);
	putc(TEST(1), stdout);

	putc(10, stdout);
	return 0;
}
