#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>


int sck[2],logged=0,pfd[2];
char nrc[5],resm[100];
void login()
{
    char user[100],message[100];
    int OK=0;
    close(sck[0]);
    while(!logged)
    {
        printf("login: ");
        scanf("%s",user);
        if(write(sck[1],user,sizeof(user)) < 0) perror("Error: login (write)");
        //printf("Aici!");
        if(read(sck[1],message,100) < 0 ) perror("Error: login (read)");
        //printf("%s",message);
        if(strcmp(message,"succes")==0) OK=1;
        else OK=0;
        if(OK==1) 
        {
            logged =1;
            printf("Te-ai conectat cu succes!\n");
        }
        else {
            printf("Nu te-ai putut conecta! Username incorect.\n");
        }
    }
    close(sck[1]);
}
void afisareMeniu()
{
    printf("\nMeniu comenzi:\n");
    printf("0: Afisati meniul din nou\n");
    printf("1: myfind file - o comanda care permite gasirea unui fisier si afisarea de informatii asociate\n");
    printf("2: mystat file - o comanda ce permite vizualizarea atributelor unui fisier\n");
    printf("3: quit - deconectare\n\n");
}
void comenzi()
{
    char com[100];
        while(logged)
        {
            printf("Introduceti numarul comenzii:");
            scanf("%s",nrc);
            if(write(pfd[0],nrc,sizeof(nrc)) <0) perror("Error: parent commands (write)");
            if(read(sck[1],com,100) <0) perror("Error: parent commands (read)");
            if(strcmp(com,"quit")) {
                printf("Te-ai deconectat cu succes");
                exit(0);
            }
            if(strcmp(com,"menu"))afisareMeniu();
            
        }
    
    printf("Nu sunteti conectat!");

}
//void afis();{}
//void quit(){}
/*char* mystat(char* file)
{
    struct stat st;
    char *mv = malloc (sizeof(char)) * 10 +1);
    file[strlen(file) -1] = '\0';
    if(statistici(file,&stat) == 0)
    {
        afis();
        mode_t perm =st.stat_mode;

        mv[0] = (perm & S_IFDIR) ? 'd' : '-';
        mv[1] = (perm & S_IRUSR) ? 'r' : '-';
        mv[2] = (perm & S_IWUSR) ? 'w' : '-';
        mv[3] = (perm & S_IXUSR) ? 'x' : '-';
        mv[4] = (perm & S_IRGRP) ? 'r' : '-';
        mv[5] = (perm & S_IWGRP) ? 'w' : '-';
        mv[6] = (perm & S_IXGRP) ? 'x' : '-';
        mv[7] = (perm & S_IROTH) ? 'r' : '-';
        mv[8] = (perm & S_IWOTH) ? 'w' : '-';
        mv[9] = (perm & S_IXOTH) ? 'x' : '-';
        mv[10] = '\0';
        return mv;
    }
    else {
        
        return strerror(errno);
    }
}
*/
int main()
{
    FILE *fis; 
    fis = fopen("log.txt","r");
    if(socketpair(AF_UNIX,SOCK_STREAM,0,sck) < 0)
    {
        perror("Error socketpair");
        exit(1);
    }
    int i=1;
    //while(i<3)
    //{
    pid_t pid1=fork();
    
    if(pid1==-1)
    {
        perror("Error fork()"); 
        exit(1);
    }

     if(pid1==0)
    {
        char buffer[100],msg[100],response[100];
        close(sck[1]);
        //printf("111");
        if(read(sck[0],msg,100) <0) perror("Error: child1 (read)");
        //printf("%s\n\n",msg);
        while(!feof(fis))
        {
            fgets(buffer,100,fis);
            //puts(buffer);
            //printf("%s",buffer);
            buffer[strlen(buffer)-1]= '\0';
            if(strcmp(msg,buffer) ==0) 
            {
                strcpy(response,"succes");
                break;
            }
            else strcpy(response,"failure");
        }
        //printf("%s",response);
        if(write(sck[0],response,sizeof(response)) <0) perror("Error: child1 (write)");
        close(sck[0]);
    }
    if(pid1>0)
    {   
        char nr[5];
        if(pipe(pfd) == -1)
            {
                fprintf(stderr,"Pipe error (parent)\n");
                exit(1);
            }
        pid_t pid2;
        switch(pid2= fork())
        {
        
            case -1: perror("Error fork()"); 
            exit(1);

            case 0:
            
            if(dup (pfd[1])!=1)
            {
                fprintf(stderr,"dup err -1\n");
                exit(1);
            }
            if(read(pfd[0],nr,5) < 0) perror("Error: child2 command (read)");
            if(strcmp(nr,"3"))
            {
                strcpy(resm,"quit");
                if(write(pfd[1],resm,sizeof(resm)) <0 ) perror("Error: child2 command (write)");
            }
            if(strcmp(nr,"0"))
            {
                strcpy(resm,"menu");
                if(write(pfd[1],resm,sizeof(resm)) <0 ) perror("Error: child2 command (write)");
            }
            close(pfd[0]);
            close(pfd[1]);

            default:
            comenzi();
            close(pfd[1]);
            close(pfd[0]);
            

        }
        
        close(sck[0]);
        login();
        close(sck[1]);
        wait(NULL);
       //i++; 
    }
    fclose(fis);
    return 0;
}