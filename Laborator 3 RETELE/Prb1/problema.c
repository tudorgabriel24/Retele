#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void prb1 ()
{
  int pfd1[2];
  int pfd2[2];
  int  readbuff1;
  char readbuff2[4];

  if(pipe1(pfd1)==-1)
  {
    fprintf(stderr, "pipe1\n");
    exit(1); 
  }
  if(pipe2(pfd2)==-1)
  {
    fprintf(stderr, "pipe2\n");
    exit(1); 
  }
  switch(fork())
  {
    case -1:
      fprintf(stderr, "fork - 1 \n");
      exit(1);

    case 0:
      close(pfd1[1]);
      read (pfd1[0],readbuff1,sizeof(readbuff1));
      if(!readbuff1%2) 
         write(pfd2[1],"no",3);
      else write(pfd2[1],"yes",4);
      close(pfd1[0]);
      
    default:
      close(pfd2[1]);
      write(pfd1[1],readbuff1,sizeof(readbuff1));
      read(pfd2[0],readbuff2,sizeof(readbuff2));
      printf("%s",readbuff2);
      close(pfd2[0]);
   }

}

int main()
{
  prb1();
  return 0;
}
