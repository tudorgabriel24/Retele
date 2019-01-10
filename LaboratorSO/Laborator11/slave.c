#include<string.h>
#include<stdio.h>

int main(int argc, char ** argv)
{
int a=atoi(argv[1]);
int b=atoi(argv[2]);
char semn=argv[3][0];
if (semn=="+")
{
printf("din slave %d\n",a+b);
exit(a+b);
}
else 
{
printf("din slave %d\n",a-b);
exit(a-b);
}
}
