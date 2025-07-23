#include <stdio.h>
#include "..\include\hash.h"


int rand(BYTE* seed){
    if(!seed) return 1;
    return 0;
}
int main(int c, char** v){
	putc(10, stdout);

    if(c == 1){
        for(int i = 0; i < 100; i++){
           printf("%d\n", rand(NULL));
        }
        return 0;
    }

	putc(10, stdout);
	return 0;
}
