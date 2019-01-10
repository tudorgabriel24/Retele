#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <fstream>

void sighandler (int sig)
{
 pid_t pid;
 FILE* fisier = open("/f1.txt",O_RDWR);
 if((pid = fork()) <0)
   {
     perror("fork");
     exit(1);
   }
 if(!pid)
   {
     pause();
     

   }
 if(pid)
   {
     
     if(pid%2)
     {
        write(fisier, 
     } 
          
   }
 close("fisier");
}
int main()
{
if(signal (SIGUSR2,sighandler) == SIG_ERR)
}
