checksum.o: checksum.c
	gcc -c .\checksum.c -o checksum.o
encrypt.o: encrypt.c encrypt.h
	gcc -c .\encrypt.c
# r: update if exist c: not verbose message s: set up index at the beginning of assemble file
# ar -t libhfsecurity.a
# ar -x libhfsecurity.a encrypt.o
libhfsecurity.a: checksum.o -o checksum.o
	ar -rcs libhfsecurity.a encrypt.o checksum.o
test_code: test_code.c libhfsecurity.a
	gcc .\test_code.c -I . -L . -lhfsecurity.a -o test_code
