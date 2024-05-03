#include<stdio.h>

void go_south_east(int* lng, int* lat){

    *lng = (*lng) + 1;
    *lat = (*lat) + 1;
}
int main(){

    int lng = 32;
    int lat = 64;

    go_south_east(&lng, &lat);

    printf("Stop! Now the position is {%i, %i}!", lng, lat);

    return 0;
}