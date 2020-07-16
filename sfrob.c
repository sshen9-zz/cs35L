#include <stdio.h>
#include <stdlib.h>
//frobcmp
int frobcmp(char const* a, char const* b){
  //if a>b, return 1, if a<b return -1, equal return 0
  while(1){
    if(*a == ' ' && *b != ' '){
      //a<b
      return -1;
    }else if(*a != ' ' && *b == ' '){
      //a>b
      return 1;
    }else if(*a == ' ' && *b == ' ' ){
      //a==b
      return 0;
    }

    char ch_a = (*a) ^ 42;
    char ch_b = (*b) ^ 42;

    if(ch_a < ch_b){
      return -1;
    }else if(ch_a > ch_b){
      return 1;
    }else{
      a+=1;
      b+=1;
    }
    
  }

}

int compare(const void* a, const void* b){
    char const* x = *(const char**) a;
    char const* y = *(const char**) b;
    return frobcmp(x, y);
}

int main(){
    int r = 1;
    int c = 1;
    char ch;
    
    //allocate array of row pointers:
    char** rows;
    rows = malloc(sizeof(char *) * 1);
    rows[0] = malloc(sizeof(char) * 1);
    
    while((ch = getchar()) != EOF){
        if(ch != ' '){
            //add onto word
            rows[r-1][c-1] = ch;
            c+=1;
            rows[r-1] = realloc(rows[r-1], c * sizeof(char));
        }else{
            //change last spot to a space
            rows[r-1][c-1] = ch;
            //allocate new row
            c = 1;
            r+=1;
            rows = realloc(rows, r * sizeof(char *));
            rows[r-1] = malloc(sizeof(char) * 1);
        }
    }
    rows[r-1][c-1] =  ' ';

    qsort(rows, r, sizeof(char*), compare);

    for(int i=0; i<r; i++){
        int col = 0;
        while(rows[i][col] != ' '){
            putchar(rows[i][col]);
            col+=1;
        }
        putchar(rows[i][col]);
    }
    
    for(int i=0; i<r; i++){
        free(rows[i]);
    }
    free(rows);

}
