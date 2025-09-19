gcc -c .\sha-256.c -o sha.o;
rm ..\commonLib\hash.a;
ar rcs ..\commonLib\hash.a *.o
