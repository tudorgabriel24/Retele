#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
int pid;
FILE *f=fopen("Fiu.txt","r");
FILE *g=fopen("Parinte.txt","r");
char l[250];
int i=0;
pid=fork();
if(pid==0)
while(!feof(f))
{
fscanf(f,"%s",&l);
printf("%s\n",l);
sleep(1);
}
else 
while(!feof(g))
{
fscanf(g,"%s",&l);
printf("%s\n",l);
sleep(1);
}
return 0;
}
