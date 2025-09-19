#include <stdio.h>
#include <time.h>
#include <math.h>
#include "..\include\hash.h"


u64 rand(){
    u64 now;
    u32(*fn_ww[])(u32) = { rotationA, rotationB, rotationC, rotationD };
    u64 len_ww = sizeof(fn_ww)/sizeof(fn_ww[0]);
    u64 fn[len_ww+2];
    fn[len_ww] = (u64)(u64(*)(BYTE*)) rand;
    fn[len_ww+1] = (u64)&now;
    for(int i = 0; i < len_ww; i++) fn[i] = (u64)fn_ww[i];
    now = now ^ time(NULL);
    now = (now << 31) & now;
}
u64 rand_s(BYTE* seed){
    u64 now;
    u32(*fn_ww[])(u32) = { rotationA, rotationB, rotationC, rotationD };
    u64 len_ww = sizeof(fn_ww)/sizeof(fn_ww[0]);
    u64 fn[len_ww+2];
    fn[len_ww] = (u64)(u64(*)(BYTE*)) rand;
    fn[len_ww+1] = (u64)&now;
    for(int i = 0; i < len_ww; i++) fn[i] = (u64)fn_ww[i];
    now = now ^ time(NULL);
    now = (now << 31) & now;
    if(!seed) {
        for(int i = 0; i < sizeof(fn)/sizeof(fn[0]); i++){
            printf("%p\n", fn[i]);
            //printBin(ptr[i], sizeof(ptr[i]));
        }
        return 1;
    }
        void* ptr[] = { &now, fn, (void*)seed };
        BYTE mem[sizeof(ptr)];
    return 0;
}
void test(){
}
int main(int c, char** v){
	putc(10, stdout);

    if(c == 1){
        test();
        return 0;
    }
    for(int i = 0; i < c; i++){
        if(v[i][0] == '0')
            rand(NULL);
    }

	putc(10, stdout);
	return 0;
}
