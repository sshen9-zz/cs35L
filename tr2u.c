#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void checkDuplicates(const char* p, int len){
    for(int i=0; i<len; i++){
        for(int j=i+1; j<len; j++){
            if(p[i] == p[j]){
                char a[]="duplicate characters found in argument";
                write(2, a, strlen(a));
                exit(1);
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    if(argc!=3){
        char a[] = "invalid arguments";
        write(2, a, strlen(a));
        exit(1);
    }
    
    const char* input = argv[1];
    const char* output = argv[2];
    
    int len1=0;
    int len2=0;
    
    int n=0;
    while(input[n] != '\0'){
        len1+=1;
        n+=1;
    }
    n=0;
    while(output[n] != '\0'){
        len2+=1;
        n+=1;
    }
    
    if(len1!=len2){
        char a[]="args are not same length.";
        write(2, a, strlen(a));
        exit(1);
    }

    checkDuplicates(input, len1);
    checkDuplicates(output, len2);
    
    ssize_t status = 1;
    while(status>0){
        char a[1];
        status = read(0, a, 1);
        if(status==0){
	  break;
        }else if(status == -1){
	  exit(1);
	}
        char ch = a[0];
        for(int i=0; i<len1; i++){
            if(input[i] == ch){
                a[0] = output[i];
            }
        }
        write(1, a, 1);
    }
}
