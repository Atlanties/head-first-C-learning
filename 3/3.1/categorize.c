#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]){

    char line[80];

    if(argc != 6){
        fprintf(stderr, "Your need 5 arguments!");
        return 1;
    }

    FILE *in_file = fopen("spooky.csv", "r");
    FILE *file1 = fopen(argv[2], "w");
    FILE *file2 = fopen(argv[4], "w");
    FILE *file3 = fopen(argv[5], "w");

    while(fscanf(in_file, "%79[^\n]\n", line) == 1){
        if(strstr(line, argv[1]))
            fprintf(file1, "%s\n",line);
        else if(strstr(line, argv[3]))
            fprintf(file2, "%s\n",line);
        else
            fprintf(file3, "%s\n",line);
    }
    fclose(in_file);
    fclose(file1);
    fclose(file2);
    fclose(file3);

    return 0;
}