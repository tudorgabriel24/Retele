#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>

int main()
{
int status;
if(fork()==0)
execl("slave","slave","0","1","-",0);
wait(&status);

printf("din master %d\n",WEXITSTATUS(status)); 
}
