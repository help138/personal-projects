#include <stdio.h>
#include "..\include\hash.h"


int main(int c, char** v){
	putc(10, stdout);

    BYTE out[32] = {0};
    if(c == 1){
        BYTE t[] = { 'a', 'b', '\000', 'c' };
        sha256(t, out);
        pHash(out);
        return 0;
    }
    for(int i = 0; i < c; i++){
        sha256((BYTE*)v[i], out);
        pHash(out);
    }

	putc(10, stdout);
	return 0;
}
