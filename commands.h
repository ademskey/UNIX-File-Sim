#include "header.h"

int find_command(char* user_command){
    int i = 0;
    while(cmd[i]){
        if (strcmp(user_command, cmd[i]==0)){
            return i;
        }
        i++;
    }
    return -1;
}

void mkdir(char* path){
    printf("mkdir %s\n", path);
}