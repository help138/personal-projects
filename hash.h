#ifndef M_HASH_H_
#define M_HASH_H_

#define ASCII_VAL_TACK 45
#define WORD_BIT 0x100000000

typedef unsigned long int WORD;
typedef unsigned char BYTE;

void printBin(size_t mem, size_t sz);
//this function doesn't use malloc and friends meaning files bigger than 1MB fail.
//the function will warn you if this is the case.
void hashFile_STACK(char* path, BYTE* output);
//data returned to the output var. It must be a 32 byte array
void sha256(BYTE* input, BYTE* output);
int fexist(char* name);

#endif //M_HASH_H_
