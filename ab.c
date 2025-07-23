#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h> // mkdir();

#define MAX_STACK_SIZE 256

typedef struct str{
    int mal;
    int len;
    char* data;
} string;

typedef struct s{
    int top;
    char** data;
} stack;

char* stringStack[MAX_STACK_SIZE] = {0};
const char* argStr[] = {
    "no-recursion",
    "no-echo",
    "destination",
    "include",
    "exclude",
    "help",
};
const char* specialCase[] = {
    ".git",
    ".bin",
    ".efi",
    ".exe",
    ".o",
    ".a",
    ".so",
    ".dll",
    ".sys",
    ".png",
    ".jpg",
    ".qoi",
    ".bmp",
    ".wav",
    ".midi",
};
char* destination = NULL;
int recursive = 1;


void initStack(stack* s){
    s->data = stringStack;
    s->top = -1;
}
int isEmpty(stack* s){
    if(s->top <= -1) return 1;
    return 0;
}
char* pop(stack* s){
    if(s->top == -1) return NULL;
    return s->data[s->top--];
}
int push(stack* s, char* data){
    if(s->top >= MAX_STACK_SIZE) return 0;
    s->data[s->top++] = data;
    return 1;
}
int dexists(const char* path){
    DIR* d = opendir(path);
    if(d != NULL){
        closedir(d);
        return 1;
    }
    return 0;
}
int fexists(const char* path){
    FILE* file;
    if ((file = fopen(path, "r"))){
        fclose(file);
        return 1;
    }
    return 0;
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
void concat(string* base, const char* str){
    int len = 0;
    while(str[len])len++;
    char* temp = (char*)calloc((base->len+len+1), sizeof(char));
    for(int i = 0; i < base->len; i++) temp[i] = base->data[i];
    for(int i = 0; i < len; i++) temp[i+base->len] = str[i];
    if(base->mal) free(base->data);
    base->mal = 1;
    base->len = base->len+len;
    base->data = temp;
}
void locate(char* path){
    stack paths;
    initStack(&paths);
    if(!push(&paths, path)) putc('r', stdout);
    string activePath = {0, 0, NULL}; 
    while(isEmpty(&paths) != 1){
        char* temp = pop(&paths);
        printf("\n%d: %s\n", paths.top, temp);
        concat(&activePath, temp); 
        //concat(&activePath, "\\"); 
        DIR* dir = opendir(activePath.data);
        if(dir == NULL){
            printf("failed to open %s\n", activePath);
            perror("");
            continue;
        }
        struct dirent* entry = readdir(dir); // '.'
        readdir(dir); // '..'
        while((entry = readdir(dir)) != NULL){
            printf("%s\n", entry->d_name);
            if(fexists(entry->d_name)) {
                putc('r', stdout);
                //TODO
                continue;
            }
            push(&paths, entry->d_name);
        }
    } 
}
void printInfo(){ //TODO make this more robust
    for(int ii = 0; ii < sizeof(argStr)/sizeof(argStr[0]); ii++){
        printf("--%s\n", argStr);
    }
}
void cmdParser(int cmd){
}
int main(int c, char** v){
    putc(10, stdout);

    if(c == 1){
        printf("no input\n");
        return 0;
    }
    for(int i = 1; i < c; i++){
        if(v[i][0] == '-'){
            switch(v[i][1]){
                case '-': 
                    {
                        int len = 0;
                        while(v[i][len+2])len++;
                        char buff[len];
                        for(int ii = 0; ii < len; ii++) buff[len] = v[i][len+2];
                        for(int ii = 0; ii < sizeof(argStr)/sizeof(argStr[0]); ii++) 
                            if(strEquality(argStr[ii], buff)) cmdParser(ii);
                    } break;
                case 'R': 
                case 'r':
                    {
                        if(++i >= c) {
                            recursive = 0;
                            break;
                        }
                        recursive = (v[i][0] == '1') ? 1: 0;
                    } break;
                case 'D':
                case 'd':
                    {
                            destination = v[i];
                            mkdir(destination);
                    } break;
                case 'h':
                    {
                        printInfo();
                    } break;
                default:
                    break;
            }
        } else {
            if(fexists(v[i])) printf("%s is a file\n", v[i]);
            if(dexists(v[i])) {
                locate(v[i]);
            } else {
                destination = v[i];
                if(destination == NULL) mkdir(destination);
            }
        }
    }

    putc(10, stdout);
    return 0;
}
