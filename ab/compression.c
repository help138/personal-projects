#include <stdio.h>
#include <stdlib.h>
#include "util.h"

#define KB 1024

enum {
	MAL = 1,
} flags;

typedef struct {
	u64 len;
	byte flags;
	byte* mem;
} memory;


memory* loadFile(char* path){
	FILE* fp = fopen(path, "rb");
	if(!fp){
		fprintf(stderr, "[error] failed to open '%s'\n", path);
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	byte* buff = (byte*)calloc(len, sizeof(byte));
	rewind(fp);
	int ch, i = 0;
	while((ch = getc(fp)) != EOF){
		buff[i++] = ch&0xff;
	}
	memory* out = (memory*)malloc(sizeof(memory));
	out->len = len;
	out->flags = MAL;
	out->mem = buff;
	fclose(fp);
	return out;
}
memory* runLenghtEncode(memory* mem){
	byte* raw = mem->mem;
	u64 len = mem->len;
	byte* compressed = (byte*)malloc(len);
	u64 i = 1;
	u64 ii = 0;
	while(i < len) {
		int t = 0;
		while(raw[i] == raw[i-1]) {
			t++;
			i++;
		}
		if(t > 2) {
			compressed[ii++] = raw[i-t];
			compressed[ii++] = t&0xff;
			continue;
		}
		compressed[ii++] = raw[i];
		i++;
	}
	memory* out = (memory*)malloc(sizeof(memory));
	out->mem = compressed;
	out->flags = MAL;
	out->len = ii;
	return out;
}
memory* lzssEncode(memory* raw){
	u64 len = raw->len;
	byte* buff = (byte*)malloc(sizeof(byte)*len);
	byte* wind = (byte*)calloc(KB*32, sizeof(byte));
	byte lab[258] = {0};
	memory* out = (memory*)malloc(sizeof(memory));
	out->len = len;
	out->flags = MAL;
	out->mem = NULL;
	free(wind);
	free(buff);
	return out;
}
void pBin(void* memory, u64 sz){
	byte* mem = (byte*)memory;
	printf("%llu bytes\n", sz);
	for(u64 i = sz; i > 0; i--){
		for(u32 ii = 7; ii > 0; ii--) putc(((mem[i-1]>>ii)&1) ? 49: 48, stdout);
		if(i != 1) putc('-', stdout);
	}
	putc(10, stdout);
}
void test(char* path){
	memory* buff = loadFile(path);
	memory* out = runLenghtEncode(buff);
	printf("buff: %lu, out: %lu\n", buff->len, out->len);
	//pBin(buff->mem, buff->len);
	//pBin(out->mem, out->len);
	if(buff->flags&MAL) free(buff->mem);
	if(out->flags&MAL) free(out->mem);
	free(buff);
	free(out);
}
int main(int c, char** v){
	if(c == 1){
		test(v[0]);
		return 0;
	}
	for(int i = 0; i < c; i++){
		test(v[i]);
	}
	return 0;
}
