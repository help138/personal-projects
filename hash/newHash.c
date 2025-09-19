#include <stdio.h>
#include <time.h>
#include <math.h>

#define SET_LFSR_SEED(x) lfsrSeed = x

typedef unsigned long long int u64;
typedef unsigned short int u16;
typedef unsigned long int u32;
typedef unsigned char byte;

u32 lfsrSeed = 0;


void printB(void* mem, u64 sz){
    byte* temp = (byte*)mem;
    for(u64 i = 0; i < sz; i++){
        for(int ii = 7; ii >= 0; ii--) putc(((temp[i]>>ii)&1) ? '1': '0', stdout);
        putc('-', stdout);
    }
    putc(10, stdout);
}
u32 rotation(u32 input, byte shift){
    shift = shift%32;
    u32 temp = input;
    input >>= shift;
    input |= (temp<<(32-shift));
    return input;
}
_Bool lfsr(void){
	u32 bit = 0;
    bit = ((((lfsrSeed>>31)&1) ^ ((lfsrSeed>>21)&1)) ^ ((lfsrSeed>>1)&1)) ^ (lfsrSeed&1);
    lfsrSeed >>= 1;
    lfsrSeed |= (bit<<31);
	return bit&1;
}
byte lfsr_b(void){
    byte out = 0;
    for(int i = 0; i < 8; i++){
        out <<= 1u;
        out |= lfsr();
    }
    return out;
}
void hash(byte* input, u64 sizeOfInput){
    u32 seed = 0;
    u32 feedback = 0;
    for(u64 i = 0; i < sizeOfInput; i++){
        if(sizeOfInput%4 == 0 && i != 0) seed ^= feedback;
        feedback <<= 8u;
        feedback |= input[i];
    }
}
void pFile(char* path){
    FILE* fp = fopen(path, "rb");
    if(!fp){
        printf("[Warning] Failed to open '%s'\n", path);
        perror("");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    char buff[len];
    rewind(fp);
    for(long i = 0; i < len; i++){
        buff[i] = fgetc(fp);
    }
    printB(&buff, sizeof(buff));

    fclose(fp);
}
void test(){
    u32 num = 0xffff;
    SET_LFSR_SEED(num);
    printf("%lu:\n", num);
    printB(&num, sizeof(num));
}
int main(int c, char** v){
    putc(10, stdout);

    if(c == 1) {
        test();
        //printf("No command line arguments provided.\n");
        return 0;
    }
    byte out[32] = {0};
    for(int i = 1; i < c; i++){
        pFile(v[i]);
    }

    putc(10, stdout);
    return 0;
}
