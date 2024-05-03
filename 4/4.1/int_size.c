#include<stdio.h>
#include<limits.h>
#include<float.h>

int main(){

    printf(" The value of Max int is: %d\n", INT_MAX);
    printf(" The value of Min int is: %d\n", INT_MIN);
    printf(" An integer takes %d bytes\n", sizeof(int));
    printf(" The value of Max float is: %f\n", FLT_MAX);
    printf(" The value of Min float is: %f\n", FLT_MIN);
    printf(" A float takes %d bytes\n", sizeof(float));

    return 0;
}