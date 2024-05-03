#include<stdio.h>

int main(){
    float lat;
    float lng;

    char info[80];
    int started = 0;

    puts("data=[");
    while(scanf("%f, %f, %79[^\n]", &lat, &lng, info) == 3){
        if(started)
            printf(",\n");
        else
            started = 1;
        if(lat < -90.0 || lat > 90.0){
            fprintf(stderr, "error latitude: %s\n", lat);
            return 2;
        }
        if(lng < -180.0 || lng > 180.0){
            fprintf(stderr, "error longtitude: %s\n", lng);
            return 2;
        }
        printf("{\"latitude\": %f, \"longtitude\": %f, \"info\": '%s'}",lat, lng, info);        
    }
    puts("\n]");
    return 0;
}