#include <stdio.h>
#include <time.h>
#include <math.h>

#define ASCII_VAL_TACK 45
#define WORD_BIT 0x100000000

typedef unsigned long int WORD;
typedef unsigned char BYTE;


void binToHexStr(BYTE* input){ // input has the output string

}
void printBin(size_t mem, size_t sz){
	int padding = 0;
	for(size_t i = 0; i < sz; i++){
		if(i%2==0)
			padding += 3;
		else 
			padding += 2;
	}
	printf("%*llu:", padding, mem);
	sz *= 8;
	char buff[sz+1];
	buff[0] = mem&1 ? 49: 48; 
	for(size_t i = 1; i < sz; i++) buff[i] = ((mem>>=1)&1) ? 49: 48;
	for(size_t i = sz; i > 0; i--) {
		if(i%8 == 0) putc(32, stdout);
		putc(buff[i-1], stdout);
	}
	putc(10, stdout);
}
size_t LFSR(size_t seed){
	if(seed == 0)seed++;
	//printBin(seed, 8ULL);
	//putc(10, stdout);
	size_t bit = 0;
	size_t out = 0;
	for(int i = 0; i < 64; i++){ // 2^64, not 64.
		bit = (seed&1)^((seed>>1)&1);
		seed >>= 1;
		seed |= (bit<<63);
		out |= ((seed&1)<<i);
		//printBin(seed, 8ULL);
	}
	//putc(10, stdout);
	//printBin(out, 8ULL);
	return out;
}
int rotation(unsigned input, int num){
	unsigned bit = 0;
	unsigned temp = input;
	for(int i = 0; i < num; i++) {
		bit = temp&1;
		temp >>= 1;
		temp |= (bit << 31);
	}
	return temp;
}
unsigned rotationA(unsigned input){
	unsigned t1 = rotation(input, 7);
	unsigned t2 = rotation(input, 18);
	unsigned t3 = input >> 3;
	return (t1 ^ t2) ^ t3;
}
unsigned rotationB(unsigned input){
	unsigned t1 = rotation(input, 17);
	unsigned t2 = rotation(input, 19);
	unsigned t3 = input >> 10;
	return (t1 ^ t2) ^ t3;
}
unsigned rotationC(unsigned input){
	unsigned t1 = rotation(input, 2);
	unsigned t2 = rotation(input, 13);
	unsigned t3 = rotation(input, 22);
	return (t1 ^ t2) ^ t3;
}
unsigned rotationD(unsigned input){
	unsigned t1 = rotation(input, 6);
	unsigned t2 = rotation(input, 11);
	unsigned t3 = rotation(input, 25);
	return (t1 ^ t2) ^ t3;
}
unsigned choose(unsigned input, unsigned x, unsigned y){
	unsigned out = 0;
	for(int i = 0; i < 32; i++) out |= ((input>>i)&1) ? ((x>>i)&1)<<i: ((y>>i)&1)<<i;
	return out;
}
unsigned majority(unsigned input, unsigned x, unsigned y){
	unsigned out = 0;
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
void initK(WORD* constant, int num){ // TODO fill the p array if values form the prime text file.
	double p[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311};	
	for(int i = 0; i < num; i++) {
		p[i] = cbrt(p[i]);
		p[i] -= trunc(p[i]);
		constant[i] = (WORD)(p[i] * WORD_BIT);
	}
}
void initREG(WORD* constant, int num){ // TODO fill the p array if values form the prime text file.
	double p[] ={ 2, 3, 5, 7, 11, 13, 17, 19 };
	for(int i = 0; i < num; i++) {
		p[i] = sqrt(p[i]);
		p[i] -= trunc(p[i]);
		constant[i] = (WORD)(p[i] * WORD_BIT);
	}
}
unsigned addWithMod(unsigned x, unsigned y){
	size_t temp = x + y;
	return temp%WORD_BIT;
}
void mod_256(BYTE* input, unsigned key, BYTE* output){ 
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
	initK(k, 64);
	initREG(prev, 8);
	for(unsigned i = 0; i < 8; i++) {
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
			// printf("%d: ", i);
			// printBin(schedule[i], sizeof(WORD));
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
		//printing for debugging.
		//putc(10, stdout);
		//for(size_t i = 0; i < numBlock; i++) {printf("%llu: ", i);  printBin(block[i], sizeof(BYTE)); }
		//putc(10, stdout);
		//for(int i = 0; i < 64; i++) {printf("w%d: ", i); printBin(schedule[i], sizeof(WORD)); }
		//putc(10, stdout);
		//for(int i = 0; i < 8; i++) {printf("%d: ", i); printBin(prev[i], sizeof(WORD)); }
		//for(int i = 0; i < 32; i++) {printf("%d: ", i); printBin(output[i], sizeof(BYTE)); }
	}		
	for(int i = 0, iii = 0; i < 8; i++) {
		for(int ii = 3; ii > -1; ii--){
			output[iii++] = (prev[i]>>(ii*8))&0xff;
		}
	}
}
void sha256(BYTE* input, BYTE* output){ // data returned to the output var. It must be a 32 byte array
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
	initK(k, 64);
	initREG(prev, 8);
	for(unsigned i = 0; i < 8; i++) {
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
	for(int i = 0, iii = 0; i < 8; i++) {
		for(int ii = 3; ii > -1; ii--){
			output[iii++] = (prev[i]>>(ii*8))&0xff;
		}
	}
}
void hashFile_STACK(char* path, BYTE* output){ //this function doesn't use malloc and friends meaning files bigger than 1MB fail.
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
	sha256((BYTE*)buff, output);

	if(fclose(file) != 0){
		printf("failed to close file %s\n", path);
		perror("");
	}
}
int rand(BYTE* s){
}
int fexist(char* name){
	FILE* f = fopen(name, "r");
	if(f != NULL){
		fclose(f);
		return 1;
	}
	return 0;
}
int hashToChar(char* hash){
    int sz = sizeof(int);
    BYTE out[32] = {0};
    unsigned temp[32/sz];
    sha256(hash, out);
    for(int i = 0; i < 32/sz; i++) temp[i] = 0;
    for(int i = 0, ii = 0; i < 32/sz; i++){
        temp[i] = out[ii] | ((out[ii+1]) >> 8u) | ((out[ii+2]) >> 16u) | ((out[ii+3]) >> 24u);
        ii+=4;
    }
}
int charToInt(char* input){
    int i = 0;
    int out = 0;
    while(input[i] != '\000'){
        out *= 10;
        switch(input[i]){
            case '0':
                out += 0;
                break;
            case '1':
                out += 1;
                break;
            case '2':
                out += 2;
                break;
            case '3':
                out += 3;
                break;
            case '4':
                out += 4;
                break;
            case '5':
                out += 5;
                break;
            case '6':
                out += 6;
                break;
            case '7':
                out += 7;
                break;
            case '8':
                out += 8;
                break;
            case '9':
                out += 9;
                break;
            default:
                break;
        }
        i++;
    }
    return out;
}
int main(int c, char** v){
	putc(10, stdout);

	if(c == 1) {
		printf("No command line arguments provided.\n");
		return 0;
    }
    BYTE out[33] = {0};

	putc(10, stdout);
    return 0;
}
int temp(int c, char** v){
	putc(10, stdout);

	if(c == 1) {
		printf("No command line arguments provided.\n");
		return 0;
    }
    BYTE out[33] = {0};
	if(c == 2) {
        hashFile_STACK(v[1], out);
        for(int iii = 0; iii < 32; iii++) printf("%02x", out[iii]);
		return 0;
    }
    int numRecur = charToInt(v[1]);
    for(int i = 2; i < c; i++){
        //hashToChar(v[i], out);
        sha256(v[i], out);
        printf("%d: %s\n\n", i, v[i]);
        for(int ii = 0; ii < numRecur; ii++){
            printf("\t%d:\n\t%s\n\t", ii+1, out);
            for(int iii = 0; iii < 32; iii++) printf("%02x", out[iii]);
            sha256(out, out);
            putc(10, stdout);
        }
    }

    putc(10, stdout);
    return 0;
}
/** sorces:
 * https://emn178.github.io/online-tools/sha256.html
 * https://sha256algorithm.com/
 * https://www.youtube.com/watch?v=f9EbD6iY9zI&t=528s
 */
