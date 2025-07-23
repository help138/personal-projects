#include <stdio.h>

char* const names[] = {
	"Fizz",
	"Buzz",
	"Cizz",
	"Duzz",
	"Eizz",
	"Guzz",
	"Hizz",
	"Juzz",
	"Kizz",
	"Luzz",
	"Mizz",
	"Nuzz",
	"Pizz",
	"Ruzz",
	"Sizz",
	"Tuzz",
};


int charToNum(char ch){
	switch(ch){
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		default: 
			return 0;
	}
}
size_t strToNum(char* str){
	size_t out = 0;
	int len = 0;
	while(str[len]){
		out *= 10;
		int t = charToNum(str[len]);
		if(t == -1) {
			printf("%s isn't a number.\n", str);
			return 0;
		}
		out += t; 
		len++;
	}
	return out;
}
int main(int c, char** v){
	putc(10, stdout);

	if(c == 1) {
		printf("this program expects an input");
		return 0;
	}
	size_t len = 0;
	size_t filt[16] = {0};
	for(int i = 0; i < c; i++){
		if(v[i][0] == '-'){
			switch(v[i][1]){
				case 'n':				
				case 'N': 
					i++;
					if(i >= c) {
						printf("no number provide for argument '%s'\n", v[i-1]);
						return -1;
					}
					len = strToNum(v[i]);
					if(len == 0) return 0;
					break;
				case 'f':
				case 'F':
					i++;
					if(i >= c) {
						printf("no number provide for argument %s\n", v[i-1]);
						return -1;
					}	
					for(int ii = 0; ii < 16; ii++){
						filt[ii] = strToNum(v[i]);
						if(++i >= c || v[i][0] == '-') break;
					}
					break;
				default: 
					break;
			}
		}
	}
	if(len <= 0){
		printf("%llu rounds is impossable. Please use '-num' to give the number of rounds you want to play.\n", len);
		return 0;
	}
	if(filt[0] == 0){
		printf("Please use '-filt' to give the number you want to play.\n");
		return 0;
	}
	int numFilt = 0;
	for(size_t i = 0; i < 16; i++) {
		if(filt[i] == 0) break;
		numFilt++; 
	}
	
	for(size_t i = 0; i < len; i++) {
		char out[65] = {0};
		int c = 0;
		for(int ii = 0; ii < numFilt; ii++) {
			if(i%filt[ii] == 0){
				for(int iii = c; iii < c+4; iii++) out[iii] = names[ii][iii];
				c+=3;
			}
		}
		if(out[0] == '\000') 
			printf("%d\n", i);
		else
			printf("%s\n", out);
	}

	putc(10, stdout);
	return 0; 
}
