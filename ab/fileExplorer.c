#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include "util.h"

struct charArr{
	u32 len;
	bool mal;
	char* str;
};

typedef struct charArr string;
typedef struct charArr path;
typedef void(*func)(char*);
typedef struct{
	int len;
	string* arr;
} pathArr;


string concat(char** str, int num){
	int t = 0;
	int len = 0;
	for(int i = 0; i < num; i++) {
		t = 0;
		while(str[i][t])t++;
		len += t;
	}
	string out = { 
		.mal = 1, 
		.str = (char*)calloc(len, sizeof(char)), 
		.len = len,
	};
	len = 0;
	for(int i = 0; i < num; i++){
		t = 0;
		while(str[i][t]){
			out.str[len+t] = str[i][t];
			t++;
		}
		len += t;
	}
	return out;
}
static bool strEquality(char* str1, char* str2){
	int len1 = 0;
	int len2 = 0;
	while(str1[len1])len1++;
	while(str2[len2])len2++;
	if(len1 != len2) return 0;
	for(int i = 0; i < len1; i++) if(str1[i] != str2[i]) return 0;
	return 1;
}
static bool dexist(char* path){
	DIR* d = opendir(path);
	if(d){
		closedir(d);
		return 1;
	}
	return 0;
}
static bool fexist(char* path){
	if(path == NULL) return 0;
	FILE* f = fopen(path, "r");
	if(f){
		fclose(f);
		return 1;
	}
	return 0;
}
void locateFn(char* path, func fn){
	if(fn == NULL) return;
	fn(path);
	if(dexist(path)){
		DIR* dir = opendir(path);
		readdir(dir); // .
		readdir(dir); // ..
		struct dirent* entry;
		while((entry = readdir(dir)) != NULL){
			char* temp[3] = { path, "/", entry->d_name };
			string buff = concat(temp, 3);
			locateFn(buff.str, fn);
			free(buff.str);
		}
	}
}
pathArr locateRet(char* path){
	DIR* dir = opendir(path);
	pathArr out = {0};
	readdir(dir); // .
	readdir(dir); // ..
	struct dirent* entry;
	while((entry = readdir(dir)) != NULL){
		char* temp[3] = { path, "/", entry->d_name };
		string buff = concat(temp, 3);
		if(dexist(buff.str)){
			locateRet(buff.str);
		}
		if(fexist(buff.str)){
			printf("%s\n", buff.str);
		}
	}
	return out;
}
