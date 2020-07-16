#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {
    if(argc!=3){
        fprintf(stderr, "invalid arguments");
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
        fprintf(stderr, "arguments are not the same length");
        exit(1);
    }

    
    for(int i=0; i<len1; i++){
        for(int j=i+1; j<len1; j++){
            if(input[i] == input[j]){
                fprintf(stderr, "duplicate characters found in input argument");
                exit(1);
            }
        }
    }
    
    for(int i=0; i<len2; i++){
        for(int j=i+1; j<len2; j++){
            if(output[i]==output[j]){
                fprintf(stderr, "duplicate characters found in output argument");
                exit(1);
            }
        }
    }
    
    char ch;
    
    //output characters
    while((ch=getchar()) != EOF){
        int t = 0;
        for(int i=0; i<len1; i++){
            if(ch==input[i]){
                putchar(output[i]);
                t = 1;
            }
        }
        if(!t){
            putchar(ch);
        }
    }
    return 0;
}
