#include<unistd.h>
#include<stdio.h>

int main()
{
if(fork()==0)
execlp("ls","ls","-l",".",0);
wait(1);
printf("asdf\n");
}
