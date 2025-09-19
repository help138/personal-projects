#include <stdio.h>
#include <stdio.h>

typedef unsigned long long WORD;
typedef unsigned long u32;


void pHash(BYTE* hash){
    for(int i = 0; i < 32; i++) printf("%02x", hash[i]);
    putc(10, stdout);
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
WORD rotation(WORD input, BYTE shift){
    shift = shift%64;
    WORD temp = input;
    input >>= shift;
    input |= (temp<<(64-shift));
    return input;
}
WORD rotationK(WORD input, WORD key){
	WORD t1 = rotation(input, (key>>24u)&0xff);
	WORD t2 = rotation(input, (key>>16u)&0xff);
	WORD t3 = rotation(input, (key>>8u)&0xff);
	WORD t4 = ( ((((key>>31)&1) ^ ((key>>21)&1)) ^ ((key>>1)&1)) ^ (key&1) ) ? 
        rotation(input, (key)&0xff):
        input >> (key)&0xff;
	return (t1 ^ t2) ^ (t3 ^ t4);
}
WORD choose(WORD input, WORD x, WORD y){
	WORD out = 0;
	for(int i = 0; i < 64; i++) out |= ((input>>i)&1) ? ((x>>i)&1)<<i: ((y>>i)&1)<<i;
	return out;
}
WORD majority(WORD input, WORD x, WORD y){
	WORD out = 0;
	int zero, one;
	for(int i = 0; i < 64; i++){
		zero = one = 0;
		((input>>i)&1) ? one++: zero++;
		((x>>i)&1) ? 	 one++: zero++;
		((y>>i)&1) ? 	 one++: zero++;
		out |= (zero < one) ? (1<<i): (0<<i);
	}
	return out;
}
void test(){
}
int main(int c, char** v){

    if(c == 1){
        test();
        return 0;
    }

    for(int i = 1; i < c; i++){
    }

    return 0;
}
