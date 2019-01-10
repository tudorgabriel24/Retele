/* servTCPConcTh2.c - Exemplu de server TCP concurent care deserveste clientii
   prin crearea unui thread pentru fiecare client.
   Asteapta un numar de la clienti si intoarce clientilor numarul incrementat.
	Intoarce corect identificatorul din program al thread-ului.
  
   
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

/* portul folosit */
#define PORT 2908

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct thData{
	int idThread; //id-ul thread-ului tinut in evidenta de acest program
	int cl; //descriptorul intors de accept
}thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);

int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	
  int nr;		//mesajul primit de trimis la client 
  int sd;		//descriptorul de socket 
  int pid;
  pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
	int i=0;
  

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
  /* utilizarea optiunii SO_REUSEADDR */
  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 2) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }
  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
    {
      int client;
      thData * td; //parametru functia executata de thread     
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      // client= malloc(sizeof(int));
      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
	{
	  perror ("[server]Eroare la accept().\n");
	  continue;
	}
	
        /* s-a realizat conexiunea, se astepta mesajul */
    
	// int idThread; //id-ul threadului
	// int cl; //descriptorul intors de accept

	td=(struct thData*)malloc(sizeof(struct thData));	
	td->idThread=i++;
	td->cl=client;

	pthread_create(&th[i], NULL, &treat, td);	      
				
	}//while    
};				
static void *treat(void * arg)
{		
		struct thData tdL; 
		tdL= *((struct thData*)arg);	
		printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
		fflush (stdout);		 
		pthread_detach(pthread_self());		
		raspunde((struct thData*)arg);
		/* am terminat cu acest client, inchidem conexiunea */
		close ((intptr_t)arg);
		return(NULL);	
  		 		
};


void raspunde(void *arg)
{
  int i=0,OK=0,nr;
  char logare[25], comanda[2];

	struct thData tdL; 
	tdL= *((struct thData*)arg);



  while(OK==0)
  {
  FILE *fisierLOG;
  FILE *adminLOG;
  fisierLOG = fopen("logare.txt","r+");
  adminLOG = fopen("admins.txt","r+");
  bzero(comanda,1);
  if (read (tdL.cl, &comanda,sizeof(char)*2) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client. (COMANDA)\n");
			
			}
  //printf("\n[server]S-a primit comanda: %s",comanda);
  if(strcmp(comanda,"2")==0) // LOGARE
  {
    printf("\nComanda primita este : %s",comanda);
  if (read (tdL.cl, &nr,sizeof(int)) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client. (NR)\n");
			
			}
    printf("\n%d",nr);
	if (read (tdL.cl, &logare,sizeof(char)*nr) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client.(LOGARE) \n");
			
			}
	
  char bufferLOG[25];
  printf("%s",logare);

  int drept_de_vot=0;
  char cu_drept[30],fara_drept[30];

  strcpy(cu_drept,logare);
  strcat(cu_drept," ");
  strcpy(fara_drept,cu_drept);
  strcat(cu_drept,"da");
  strcat(fara_drept,"nu");

  printf("\n%s",cu_drept);
  printf("\n%s",fara_drept);
  while(fgets(bufferLOG,25,fisierLOG)!=NULL)
  {
    printf("\nBufferLOG: %s",bufferLOG);
    bufferLOG[strlen(bufferLOG)-1]='\0';
    if(strcmp(bufferLOG,cu_drept) ==0)
    {
      OK=1;
      printf("\n%d",OK);
      drept_de_vot=1;
    }
    if(strcmp(bufferLOG,fara_drept)==0)
    {
      OK=1;
      drept_de_vot=0;
    }
    printf("\n%d",OK);
  }
  char n[10],n2[10];
  int OKadm=0,i;
  for(i=0;i<strlen(cu_drept);i++) 
  {
    if(cu_drept[i]!=' ') n[i]=cu_drept[i];
  }
  n[i]='\0';
  printf("\n%s",n2);
  while(fgets(n2,10,adminLOG)!=NULL)
  {
    n2[strlen(n2)-1]='\0';
    if(strcmp(n2,n)==0) {OKadm=1; break;}
  }
  printf("\n%s",n2);
	printf ("\n[Thread %d]Mesajul a fost receptionat...%s\n",tdL.idThread, logare);
		      
		      /*pregatim mesajul de raspuns */

	printf("[Thread %d]Trimitem mesajul inapoi...%d\n",tdL.idThread, OK);
		      
		      
		      /* returnam mesajul clientului */
	 if (write (tdL.cl, &OK, sizeof(int)) <= 0)
		{
		 printf("[Thread %d] ",tdL.idThread);
		 perror ("[Thread]Eroare la write() catre client.\n");
		}
	else
		printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",tdL.idThread);	

    if (write (tdL.cl, &drept_de_vot, sizeof(int)) <= 0)
		{
		 printf("[Thread %d] ",tdL.idThread);
		 perror ("[Thread]Eroare la write() catre client.\n");
		}
	else
		printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",tdL.idThread);	

    if (write (tdL.cl, &OKadm, sizeof(int)) <= 0)
		{
		 printf("[Thread %d] ",tdL.idThread);
		 perror ("[Thread]Eroare la write() catre client.\n");
		}
	else
		printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",tdL.idThread);	

    bzero(logare,nr);
    //if(OK==0) break;
    fclose(fisierLOG);
    fclose(adminLOG);
  }

  if(strcmp(comanda,"1")==0) //         INREGISTRARE
  {
    printf("\nComanda primita este: %s",comanda);
    char setadmin[1];
    int admin;
    if (read (tdL.cl, &setadmin,sizeof(char)*2) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client.\n");
			
			}
      printf("\n[server]Admin cerut: %s",setadmin);
    if(strcmp(setadmin,"1")==0)
    {
      char key[20];
      strcpy(key,"ceamaigreaparola");
      if (write (tdL.cl, &key, sizeof(char)*16) <= 0)
      {
		 printf("[Thread %d] ",tdL.idThread);
		 perror ("[Thread]Eroare la write() catre client.\n");
		  }
	    else
		     printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",tdL.idThread);
      
      if (read (tdL.cl, &admin,sizeof(int)) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client.\n");
			
			}
      if(admin==0) admin--;
    }
    else admin=0;

    if(admin>=0)
    {
      int OKREG=0,nrREG;
      char REGISTER[25];
      if (read (tdL.cl, &nrREG,sizeof(int)) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client.\n");
			
			}
      printf("%d",nr);
	    if (read (tdL.cl, &REGISTER,sizeof(char)*nrREG) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client.\n");
			
			}
	
      char bufferREG[25];
      printf("%s",REGISTER);

      int drept_de_votR=0;
      char cu_dreptR[30],fara_dreptR[30];

      strcpy(cu_dreptR,REGISTER);
      strcat(cu_dreptR," ");
      strcpy(fara_dreptR,cu_dreptR);
      strcat(cu_dreptR,"da");
      strcat(fara_dreptR,"nu");

      printf("\n%s",cu_dreptR);
      printf("\n%s",fara_dreptR);
      while(fgets(bufferREG,25,fisierLOG)!=NULL)
      {
        printf("\nBufferLOG: %s",bufferREG);
        bufferREG[strlen(bufferREG)-1]='\0';
        if(strcmp(bufferREG,cu_dreptR) ==0)
        {
          OKREG=1;
          printf("\n%d",OKREG);
        }
        if(strcmp(bufferREG,fara_dreptR)==0)
        {
          OKREG=1;
        }
        printf("\n%d",OKREG);
      }
       /* returnam mesajul clientului */
	  if (write (tdL.cl, &OKREG, sizeof(int)) <= 0)
	   	{
		    printf("[Thread %d] ",tdL.idThread);
		    perror ("[Thread]Eroare la write() catre client.\n");
		  }
	  else
		    printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",tdL.idThread);	

    if(OKREG==0)
    {
      fputs(cu_dreptR,fisierLOG);
      if(admin==1)
      {
        char nume[10];
        int i;
        for(i=0;i<strlen(cu_dreptR);i++) if(cu_dreptR[i]==' ') break;
        ++i; strncpy(nume,cu_dreptR,i);
        printf("\nNUME ADMIN: %s",nume);
        fputs(nume,adminLOG);
      }
    }
    bzero(REGISTER,nrREG);
    //if(OK==0) break;
    fclose(fisierLOG);
    fclose(adminLOG);
   }

    
  }
  }
}
