#include<unistd.h>
#include<stdio.h>
int main()
{
int p[2];
pipe(p);
fin=fdopen(p[0],"r");
fout=fdopen(p[1],"w");
fprintf(fout,"a");
fscanf(fin,"%c",&c);
printf("%c",&c);
}
}
