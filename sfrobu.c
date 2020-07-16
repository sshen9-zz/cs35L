#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <limits.h>


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

int frobcmpUpper(char const* a, char const* b){
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
    
      if(ch_a > 0){
          ch_a = toupper(ch_a);
      }
      if(ch_b > 0){
          ch_b = toupper(ch_b);
      }
      
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

int compareUpper(const void* a, const void* b){
    char const* x = *(const char**) a;
    char const* y = *(const char**) b;
    return frobcmpUpper(x, y);
}

int main(int argc, const char * argv[]){
    
    
    if(argc != 1 && argc != 2){
        char m[]="invalid number of arguments";
        write(2, m, strlen(m));
    }


    //check args
    int flag = 0;
    if(argc == 2){
        const char* f = argv[1];
        const char s[] = "-f";
        if(strcmp(f, s) != 0){
            char m[]="invalid option";
            write(2, m, strlen(m));
        }else{
            flag = 1;
        }
    }


    const int KiB = 1024;
    //read into buffer:
    struct stat fileData;
    //STDIN_FILENO = 0
    if (fstat(STDIN_FILENO, &fileData) < 0)
    {
        char m[] = "fstat error";
        write(2, m, strlen(m));
        exit(1);
    }
//    printf("Size of File: %ld \n", fileData.st_size);

    //check if regular file:
    char* buf = NULL;
    long bufSize = 0;
    if(S_ISREG(fileData.st_mode)){
        //allocate buffer to hold whole file
        bufSize = fileData.st_size * 1.3;
        buf = (char*)malloc(sizeof(char) * bufSize);

    }else{
        //allocate 8KiB
        bufSize = 8*KiB;
        buf = (char*)malloc(sizeof(char) * bufSize);
    }

    ssize_t status = 1;
    int bytesRead = 0;
    long spaceLeft = bufSize;
    
    while(status>0){
        status = read(0, buf+bytesRead, spaceLeft);
        if(status==0){
            //EOF
            break;
        }
        else if(status<spaceLeft && status>0)
        {
            bytesRead+=status;
            if(bytesRead > (bufSize/2)){
                bufSize*=2;
                buf = (char*)realloc(buf, sizeof(char)*bufSize);
            }
            spaceLeft = bufSize-bytesRead;
            continue;
        }
        else if(status==-1)
        {
            char a[] = "read error";
            write(2, a, strlen(a));
            exit(1);
        }

        bytesRead+=spaceLeft;
        bufSize*=2;
        buf = (char*)realloc(buf, sizeof(char)*bufSize);
        spaceLeft = bufSize-bytesRead;
    }
    
    int r = 1;
    int c = 1;

    //allocate array of row pointers:
    char** rows;
    rows = malloc(sizeof(char *) * 1);
    rows[0] = malloc(sizeof(char) * 1);

    for(int i=0; i<bytesRead; i++){
        char ch = buf[i];
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

    if(flag){
        qsort(rows, r, sizeof(char*), compareUpper);
    }else{
        qsort(rows, r, sizeof(char*), compare);
    }

    for(int i=0; i<r; i++){
        int col = 0;
        while(rows[i][col] != ' '){
            char ch = rows[i][col];
            write(1, &ch, 1);
            col+=1;
        }
        char ch = rows[i][col];
        write(1, &ch, 1);
    }

    for(int i=0; i<r; i++){
        free(rows[i]);
    }
    free(rows);

    //free buffer
    free(buf);

}
