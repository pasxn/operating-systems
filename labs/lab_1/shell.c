#include <stdio.h>

void main(void){

    char ch;
    char text[100];
    int i=0;
    
    printf(">");
    while((ch=getchar())!=EOF){
        switch(ch){
            case '\n':
                text[i]='\0';
                printf("%s\n",text);
                printf(">");
                i=0;
                break;
            default:
                text[i]=ch;
                i++;
                //putchar(ch);
        }
    }
    printf(".bye......\n");
}
