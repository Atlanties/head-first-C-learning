#include<stdio.h>

int main(){

    float lat;
    float lng;
    char info[80];

    while(scanf("%f, %f, %79[^\n]", &lat, &lng, info) == 3){
        if(lat > 26 && lat < 34)
            if(lng > -76 && lng < -64)
                fprintf(stdout, "{\"latitude\": %f, \"lontitude\": %f, %s}", lat, lng, info);
    }
    return 0;
}