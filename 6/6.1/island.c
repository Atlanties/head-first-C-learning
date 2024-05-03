#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct island
{
    char *name;
    char *opens;
    char *closes;
    struct island *next;
} island;

void display(island* start){

    island *i = start;

    for(; i != NULL; i = i->next){
        printf("name: %s\nopen: %s-%s\n", i->name, i->opens, i->closes);
    }
}

char *my_strdup(const char *s)
{
    char *copy = malloc(strlen(s) + 1);
    if (copy == NULL) {
        return NULL;
    }
    strcpy(copy, s);
    return copy;
}

island* create(char *name){
    island *i = malloc(sizeof(island));
    i->name = my_strdup(name);
    i->opens = "0:00";
    i->closes = "17:00";
    i->next = NULL;
    return i;
}

void release(island *start){

    island *i = start;
    island *next = NULL;
    for(;i != NULL; i = next){
        next = i->next;
        free(i->name);
        free(i);        
    }
}

int main(){
    island amity = {"Amity", "09:00", "17:00", NULL};
    island craggy = {"Craggy", "09:00", "17:00", NULL};
    island isla_nublar = {"Isla Nublar", "09:00", "17:00", NULL};
    island shutter = {"Shutter", "09:00", "17:00", NULL};

    amity.next = &craggy;
    craggy.next = &isla_nublar;
    isla_nublar.next = &shutter;

    display(&amity);
    return 0;
}
