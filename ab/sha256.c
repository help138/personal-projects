#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "util.h"
#include <math.h>

typedef byte BYTE;
typedef u32 WORD;


void pHash(BYTE* hash){
    for(int i = 0; i < 32; i++) printf("%02x", hash[i]);
    putc(10, stdout);
}
void pBin(void* memory, u64 sz){
	byte* mem = (byte*)memory;
	//printf("%llu bytes\n", sz);
	for(u64 i = sz; i > 0; i--){
		for(u32 ii = 7; ii > 0; ii--) putc(((mem[i-1]>>ii)&1) ? 49: 48, stdout);
		if(i != 1) putc('-', stdout);
	}
	putc(10, stdout);
}
inline WORD rotation(WORD input, BYTE shift){
	shift %= 32;
	WORD temp = input;
	input >>= shift;
	input |= (temp<<(32-shift));
	return input;
}
inline WORD rotationA(WORD input){
	WORD t1 = rotation(input, 7);
	WORD t2 = rotation(input, 18);
	WORD t3 = input >> 3;
	return (t1 ^ t2) ^ t3;
}
inline WORD rotationB(WORD input){
	WORD t1 = rotation(input, 17);
	WORD t2 = rotation(input, 19);
	WORD t3 = input >> 10;
	return (t1 ^ t2) ^ t3;
}
inline WORD rotationC(WORD input){
	WORD t1 = rotation(input, 2);
	WORD t2 = rotation(input, 13);
	WORD t3 = rotation(input, 22);
	return (t1 ^ t2) ^ t3;
}
inline WORD rotationD(WORD input){
	WORD t1 = rotation(input, 6);
	WORD t2 = rotation(input, 11);
	WORD t3 = rotation(input, 25);
	return (t1 ^ t2) ^ t3;
}
inline WORD choose(WORD input, WORD x, WORD y){
	WORD out = 0;
	for(int i = 0; i < 32; i++) out |= ((input>>i)&1) ? ((x>>i)&1)<<i: ((y>>i)&1)<<i;
	return out;
}
WORD majority(WORD input, WORD x, WORD y){
	WORD out = 0;
	int zero, one;
	for(int i = 0; i < 32; i++){
		zero = one = 0;
		((input>>i)&1) ? one++: zero++;
		((x>>i)&1) ? 	 one++: zero++;
		((y>>i)&1) ? 	 one++: zero++;
		out |= (zero < one) ? (1<<i): (0<<i);
	}
	return out;
}
void initK(WORD* constant, int num){ 
	double p[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311};	
	for(int i = 0; i < num; i++) {
		p[i] = cbrt(p[i]);
		p[i] -= trunc(p[i]);
		constant[i] = (WORD)(p[i] * 0x100000000);
	}
}
void initREG(WORD* constant, int num){ 
	double p[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311};	
	for(int i = 0; i < num; i++) {
		p[i] = sqrt(p[i]);
		p[i] -= trunc(p[i]);
		constant[i] = (WORD)(p[i] * 0x100000000);
	}
}
inline WORD addWithMod(WORD x, WORD y){
	size_t temp = x + y;
	return temp%0x100000000;
}
void sha256(BYTE* input, size_t len, BYTE* output){
	WORD k[64] = {0};
	WORD reg[8] = {0};
	WORD prev[8] = {0};
	size_t numBlock = 0;
	WORD schedule[64] = {0};
	numBlock = ((len/64)+1)*64;
	BYTE* block = (BYTE*)calloc(numBlock, sizeof(BYTE));
	for(size_t i = 0; i < len; i++) block[i] = (BYTE)input[i];
	block[len] = 0x80;
	len *= 8;
	int tempLen = (len/512)+1;
	initK(k, 64);
	initREG(prev, 8);
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
			temp = addWithMod(schedule[i-16], rotationA(schedule[i-15]));
			temp = addWithMod(temp, schedule[i-7]);
			temp = addWithMod(temp, rotationB(schedule[i-2]));
			schedule[i] = (WORD)temp;
		}
		for(int i = 0; i < 8; i++) reg[i] = prev[i];
		for(int i = 0; i < 64; i++){
			size_t t1 = addWithMod(choose(reg[4], reg[5], reg[6]), rotationD(reg[4]));
			size_t t2 = addWithMod(rotationC(reg[0]), majority(reg[0], reg[1], reg[2]));
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
	}
	output[31] = 0x0; //sudo-assert: check if the output pram has enough room for 32 bytes
	for(int i = 0, iii = 0; i < 8; i++) {
		for(int ii = 3; ii > -1; ii--){
			output[iii++] = (prev[i]>>(ii*8))&0xff;
		}
	}
	free(block);
}
void hashFileSTACK(char* path, BYTE* output){ //this function doesn't use malloc and friends meaning files bigger than ~1MB fail.
	FILE* file = fopen(path, "r");
	if(file == NULL){
		fprintf(stderr, "failed to open file '%s'\n", path);
		perror("");
		return;
	}

	char ch;
	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	if(len == -1) {
		fprintf(stderr, "failed to read form file %s\n", path);
		perror("");
		return;
	}
	if(((len>>19)&0xfff) > 0) { //scrappy test, I probably won't fix it.
		for(int i = 0; i < 32; i++) output[i] = 0x0;
		printf("file %s might be too large.\n", path);
		return;
	}
	char buff[len+1]; 
	for(long i = 0; i < len; i++) buff[i] = 0;
	long i = 0;
	rewind(file);
	while((ch = fgetc(file)) != EOF){
		buff[i++] = ch;
	}
	sha256((BYTE*)buff, (size_t)len, output);

	if(fclose(file) != 0){
		printf("failed to close file %s\n", path);
		perror("");
	}
}
void hashfileSHA256(char* path, byte* out){
	FILE* fp = fopen(path, "rb");
	if(!fp){
		fprintf(stderr, "[Warning] failed to open '%s'\n", path);
		perror("");
		return;
	}
	int ch, i;
	fseek(fp, 0, SEEK_END);
	i = ftell(fp);
	rewind(fp);
	byte* buff = malloc(i*sizeof(byte));
	i = 0;
	while((ch = getc(fp)) != EOF){
		buff[i] = (byte)ch;
		i++;
	}
	sha256(buff, (u64)i, out);
	free(buff);
	fclose(fp);
}
void hashStr(char* str, byte* out){
	int len = 0;
	while(str[len] != 0x0)len++;
	sha256((byte*)str, (u64)len, out);
}
/** sorces:
 * https://emn178.github.io/online-tools/sha256.html
 * https://sha256algorithm.com/
 * https://www.youtube.com/watch?v=f9EbD6iY9zI&t=528s
 *
 */
