#include <stdio.h>
#include <pthread.h>
#include "sha256.c"
#include "fileExplorer.c"


void hashFile(char* path){
	if(dexist(path)) return;
	FILE* fp = fopen(path, "rb");
	if(!fp){
		fprintf(stderr, "[Warning] failed to open '%s'\n", path);
		perror("");
		return;
	}

	byte out[32] = {0};
	printf("%s: ", path);
	int ch, len;
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	rewind(fp);
	byte* buff = malloc(len*sizeof(byte));
	len = 0;
	while((ch = getc(fp)) != EOF){
		buff[len] = (byte)ch;
		len++;
	}
	sha256(buff, (u64)len, out);
	pHash(out);

	free(buff);
	fclose(fp);
}
void test(){
	byte out[32] = {0};
	byte t[] = { 'a', 'b', 'c' };
	//sha256(t, sizeof(t)/sizeof(t[0]), out);
	sha256(t, 0, out);
	pHash(out);
}
void multThreadWrapper(char* path){
		pthread_t thread1, thread2;
		int id1 = pthread_create(&thread1, NULL, hashFile, (void*)path);
		//int id2 = pthread_create(&thread2, NULL, temp, (void*)path);
		pthread_join(thread1, NULL);
		pthread_join(thread2, NULL);
		if(id1) hashFile(path);
}
int main(int c, char* v[]){
	putc(10, stdout);

	if(c == 1) {
		//printf("no command line arguments provided.\n");
		test();
		return 0;
	}
	for(int i = 1; i < c; i++){
		locateFn(v[i], multThreadWrapper);
	}

	putc(10, stdout);
	return 0;
}
