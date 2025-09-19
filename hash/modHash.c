#include <stdio.h>
#include <time.h>
#include <math.h>
#include "..\include\hash.h"


void transformCubeRoot(WORD* input, int len){
    double p[len];
	for(int i = 0; i < len; i++) p[i] = (double)input[i];
	for(int i = 0; i < len; i++) {
		p[i] = cbrt(p[i]);
		p[i] -= trunc(p[i]);
		input[i] = (WORD)(p[i] * 0x100000000); //2**32
	}
}
void transformSquareRoot(WORD* input, int len){
    double p[len];
	for(int i = 0; i < len; i++) p[i] = (double)input[i];
	for(int i = 0; i < len; i++) {
		p[i] = sqrt(p[i]);
		p[i] -= trunc(p[i]);
		input[i] = (WORD)(p[i] * 0x100000000); //2**32
	}
}
void transformLog(WORD* input, int len){
    double p[len];
	for(int i = 0; i < len; i++) p[i] = (double)input[i];
	for(int i = 0; i < len; i++) {
		p[i] = log(p[i]);
		p[i] -= trunc(p[i]);
		input[i] = (WORD)(p[i] * 0x100000000); //2**32
	}
}
void reverse(BYTE* arr, int len){
    for(int i = 0; i < len/2; i++){
        int last = (len-1)-i;
        BYTE temp = arr[last];
        arr[last] = arr[i];
        arr[i] = temp;
    }
}
void modHash(BYTE* input, KEY* key){ 
    if(!key->len){
        sha256(input, key->hash);
        key->len = 32;
        return;
    }
    if(key->len < 288){
        for(int i = 0; i < key->len; i++){
        }
    }
    reverse(key->arr, key->len);
	size_t len = 0;
	WORD k[64] = {0};
	WORD reg[8] = {0};
	WORD prev[8] = {0};
	size_t numBlock = 0;
	WORD schedule[64] = {0};
	while(input[len])len++;
	numBlock = ((len/64)+1)*64;
	BYTE block[numBlock]; 
	for(size_t i = 0; i < numBlock; i++) block[i] = 0x0;
	for(size_t i = 0; i < len; i++) block[i] = (BYTE)input[i];
	block[len] = 0x80;
	len *= 8;
	int tempLen = (len/512)+1;
	for(WORD i = 0; i < 8; i++) {
		block[(numBlock-i)-1] = (len)&0xff;
		len >>= 8;
	}
	len = 0;
	while(tempLen){
		for(int ii = 0, i = len*64; ii < 16; ii++) { 
			schedule[ii] = (WORD)((block[i]<<24u) | (block[i+1]<<16u) | (block[i+2]<<8u) | block[i+3]);
			i+=4;
		}
		for(int i = 16; i < 64; i++){
			size_t temp = 0;
            if(key->len-4 > 0){
                WORD t = 0;
                t |= (key->arr[key->len--]<<24u);
                t |= (key->arr[key->len--]<<16u);
                t |= (key->arr[key->len--]<<8u);
                t |= (key->arr[key->len--]);
                temp = addWithMod(schedule[i-16], rotationK(schedule[i-15], t));
            } else {
                temp = addWithMod(schedule[i-16], rotationA(schedule[i-15]));
            }
			temp = addWithMod(temp, schedule[i-7]);
            if(key->len-4 > 0){
                WORD t = 0;
                t |= (key->arr[key->len--]<<24u);
                t |= (key->arr[key->len--]<<16u);
                t |= (key->arr[key->len--]<<8u);
                t |= (key->arr[key->len--]);
                temp = addWithMod(temp, rotationK(schedule[i-2], t));
            } else {
                temp = addWithMod(temp, rotationB(schedule[i-2]));
            }
			schedule[i] = (WORD)temp;
			// printf("%d: ", i);
			// printBin(schedule[i], sizeof(WORD));
		}
		for(int i = 0; i < 8; i++) reg[i] = prev[i];
		for(int i = 0; i < 64; i++){
            size_t t1 = 0;
            size_t t2 = 0;
            if(key->len-4 > 0){
                WORD t = 0;
                t |= (key->arr[key->len--]<<24u);
                t |= (key->arr[key->len--]<<16u);
                t |= (key->arr[key->len--]<<8u);
                t |= (key->arr[key->len--]);
                t1 = addWithMod(choose(reg[4], reg[5], reg[6]), rotationK(reg[4], t));
            } else {
                t1 = addWithMod(choose(reg[4], reg[5], reg[6]), rotationD(reg[4]));
            }
            if(key->len-4 > 0){
                WORD t = 0;
                t |= (key->arr[key->len--]<<24u);
                t |= (key->arr[key->len--]<<16u);
                t |= (key->arr[key->len--]<<8u);
                t |= (key->arr[key->len--]);
		        t2 = addWithMod(rotationK(reg[0], t), majority(reg[0], reg[1], reg[2]));
            } else {
		        t2 = addWithMod(rotationC(reg[0]), majority(reg[0], reg[1], reg[2]));
            }
			t1 = addWithMod(t1, reg[7]);
			t1 = addWithMod(t1, k[i]);
			t1 = addWithMod(t1, schedule[i]);
			for(int i = 7; i > 0; i--) reg[i] = reg[i-1];
			reg[0] = addWithMod(t1, t2);
			reg[4] = addWithMod(reg[4], t1);
		}
		for(int i = 0; i < 8; i++) prev[i] = addWithMod(prev[i], reg[i]);
		tempLen--;
		len++;
		//printing for debugging.
		//putc(10, stdout);
		//for(size_t i = 0; i < numBlock; i++) {printf("%llu: ", i);  printBin(block[i], sizeof(BYTE)); }
		//putc(10, stdout);
		//for(int i = 0; i < 64; i++) {printf("w%d: ", i); printBin(schedule[i], sizeof(WORD)); }
		//putc(10, stdout);
		//for(int i = 0; i < 8; i++) {printf("%d: ", i); printBin(prev[i], sizeof(WORD)); }
		//for(int i = 0; i < 32; i++) {printf("%d: ", i); printBin(output[i], sizeof(BYTE)); }
	}		
    key->len = 32;
	for(int i = 0, iii = 0; i < 8; i++) {
		for(int ii = 3; ii > -1; ii--){
			key->hash[iii++] = (prev[i]>>(ii*8))&0xff;
		}
	}
}
void test(){
    u32 num = 0xfefefefe;
    BYTE out[32] = {0};
    out[0] = (num>>24u)&0xff;
    out[1] = (num>>16u)&0xff;
    out[2] = (num>>8u)&0xff;
    out[3] = (num)&0xff;
    sha256(out, out);
    pHash(out);
    KEY key = {0};
    modHash(out, &key);
    pHash(key.hash);
    //for(double d = 0; d < 256; d += 0.1f){
        //printf("%f, %f\n", d, lgamma(d));
    //}
	WORD in[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311};	
    //WORD in[255] = {0};
    //for(int d = 0; d < 256; d++){
        //in[d] = d;
    //}
    int len = (int)sizeof(in)/sizeof(in[0]);
    //transformSquareRoot(in, len);
    //transformCubeRoot(in, len);
    transformLog(in, len);
    for(int d = 0; d < len; d++){
        printf("%d: ", d);
        printBin(in[d], 4ULL);
    }
}
int main(int c, char** v){
	putc(10, stdout);

    if(c == 1){
        test();
        return 0;
    }
    for(int i = 0; i < c; i++){
    }

	putc(10, stdout);
	return 0;
}
