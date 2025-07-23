#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#define STACK_MAX 256
#define MAX_TABLE_SZ 256

typedef struct string{
	char* str;
	int len;
} string;
typedef struct stack{
	char* data[STACK_MAX];
	int top;    
} stack;


int isEmpty(stack* s){
	if(s->top <= 0) return 1;
	return 0;
}
int isFull(stack* s){
	if(s->top >= STACK_MAX) return 1;
	return 0;
}
void push(stack* s, char* data){
	int i = 0;
	while(data[i]){
		s->data[s->top][i] = data[i]; 
		i++;
	}
	s->top++;
}
char* pop(stack* s){
	int len = 0; 
	while(s->data[s->top][len])len++;
	char* out = (char*)calloc(len+1, sizeof(char));
	for(int i = 0; i < len; i++) out[i] = s->data[s->top][i];
	s->data[s->top--] = (char*){0};
	return out;
}
int fexits(char* name){
	FILE* file = NULL;
	if((file = fopen(name, "r"))){
		fclose(file);
		return 1;
	}
	return 0;
}
void compress(char* str){
	int len = 0;
	int table_sz = 0;
	int table[MAX_TABLE_SZ+1];
	int hist[MAX_TABLE_SZ] = {0};
	while(str[len])len++;
	for(int i = 0; i < len; i++) hist[(int)str[i]]++;
	for(int i = 0; i < MAX_TABLE_SZ; i++){
		if(hist[i] != 0) table[table_sz++] = (char)i;
	}

	int ii = 0;
	while(table[ii]){
		printf("%d: %c\n", ii, table[ii]);
		ii++;
	}
}
void printFile(FILE* file){
	char ch;
	while((ch = fgetc(file)) != EOF) putc(ch, stdout);
}
void readFile(char* name, int testcase){
	FILE* file = fopen(name, "r");
	if(file == NULL){
		fprintf(stderr, "failed to open file %s\n", name);
		perror("");
		return;
	}

	printf("\tfile: %s\n", name);
	printFile(file);

	if(testcase){
		char ch;	
		size_t fileLen = 0;
		while((ch = fgetc(file)) != EOF)fileLen++;
		rewind(file);
		char* rawFileData = (char*)malloc(fileLen+1 * sizeof(char));
		rawFileData[fileLen+1] = '\000';
		while((ch = fgetc(file)) != EOF)fileLen++;
		compress(rawFileData);
	}
	// TODO: lossless compression
	// then write to an out file

	if(fclose(file) != 0){
		fprintf(stderr, "failed to close file %s", name);
		perror("");
	}
	return;
}
char* concat(const char* str1, const char* str2){
	int len = 1;
	while(str1[len])len++;	
	while(str2[len])len++;	
	char* out = (char*)calloc(len, sizeof(char));
	int i = 0;
	int ii = 0;
	while(str1[i]){
		out[i] = str1[i];
		i++;
	}
	while(str2[ii]){
		out[i+ii] = str2[ii];
		ii++;
	}
	return out;
}
int strEquality(const char* str1, const char* str2){
	int len1 = 0;
	int len2 = 0;
	while(str1[len1])len1++;
	while(str2[len2])len2++;
	if(len1 != len2) return 0;
	for(int i = 0; i < len1; i++) if(str1[i] != str2[i]) return 0;
	return 1;
}
void readDirOrFile(char* name){
	DIR* folder = opendir(name);
	if(folder == NULL){
		fprintf(stderr, "failed to open %s\n", name);
		perror("");
		return;
	}

	stack path; 
	push(path, name);
	struct dirent* folderContents; 
	while((folderContents = readdir(folder)) != NULL){
		if(strEquality(folderContents->d_name, ".") == 1 || strEquality(folderContents->d_name, "..") == 1) continue;
		if(fexits(folderContents->d_name) == 1){
			char* buff = pop(path);
			while(!isEmpty(path)){
				buff = concat(
			}
			readFile(folderContents->d_name, 0);	
		} else {
			printf("%s\n", folderContents->d_name); 
		} 
	}
	closedir(folder);
}
int main(void){
	putc(10, stdout);

	// TODO: multi-thread this programe to pull from multiple drives simutaniously.

	unsigned num = 32;
	printf("%u\n", num-64);
	readDirOrFile("G:\\workspace\\tests\\");

	putc(10, stdout);
	return 0;
}
