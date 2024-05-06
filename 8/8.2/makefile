all: .\hfcal.o .\hfcal.dll .\elliptical.o .\elliptical

.\hfcal.o: .\source\hfcal_UK.c
	gcc -I .\include -c .\source\hfcal_UK.c -o .\hfcal.o

.\hfcal.dll: .\hfcal.o
	gcc -shared .\hfcal.o -o .\hfcal.dll

.\elliptical.o: .\source\elliptical.c
	gcc -I .\include -c .\source\elliptical.c -o .\elliptical.o

.\elliptical: .\hfcal.dll .\elliptical.o
	gcc .\elliptical.o -L . -lhfcal -o .\elliptical

