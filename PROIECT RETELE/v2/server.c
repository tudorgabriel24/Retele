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
#include <sqlite3.h>

/* portul folosit */
#define PORT 2908

/* codul de eroare returnat de anumite apeluri */
extern int errno;

sqlite3 *db;
sqlite3_stmt *res;

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
  int i=0,nr;
	struct thData tdL; 
	tdL= *((struct thData*)arg);
  int rc = sqlite3_open("file.db",&db);

  if(rc != SQLITE_OK){
    fprintf(stderr,"\nCannot open database: %s\n",sqlite3_errmsg(db));
    sqlite3_close(db);
  }
  else printf("\nDatabase opened with succes!");

  int cmd, logged=0;
  //while(1){

    if (read (tdL.cl, &cmd,sizeof(int)) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client (cmd).\n");
			} 
    // LOGIN
    if(cmd==1){

      while(logged==0)
      {
        int userlength,passlength;
        char username[15],password[15];
        bzero(username,15);
        bzero(password,15);
        // primire username de la client
        if (read (tdL.cl, &userlength,sizeof(int)) <= 0)
			    {
			      printf("[Thread %d]\n",tdL.idThread);
			      perror ("Eroare la read() de la client (userlength).\n");
          }
        if (read (tdL.cl, &username,sizeof(char)*userlength) <= 0)
			    {
			      printf("[Thread %d]\n",tdL.idThread);
			      perror ("Eroare la read() de la client (username).\n");
			    }
    
        // primire password de la client
        if (read (tdL.cl, &passlength,sizeof(int)) <= 0)
			    {
			      printf("[Thread %d]\n",tdL.idThread);
			      perror ("Eroare la read() de la client (passlength).\n");
			    }
        if (read (tdL.cl, &password,sizeof(char)*passlength) <= 0)
			    {
			      printf("[Thread %d]\n",tdL.idThread);
			      perror ("Eroare la read() de la client (password).\n");
			    }

        printf("\nAm primit username: %s",username);
        printf("\nAm primit password: %s",password);
        
        char *search = "SELECT * FROM users WHERE nume = ?";
        rc = sqlite3_prepare_v2(db,search,-1,&res,0);
        if(rc == SQLITE_OK){
          sqlite3_bind_text(res,1,username,userlength,0);
        }

  
        int step = sqlite3_step(res);
        if(step == SQLITE_ROW){
            printf("\n%s\n",sqlite3_column_text(res,1));
            printf("\n%s\n",sqlite3_column_text(res,2));
            printf("\n%s\n",sqlite3_column_text(res,3));
            fflush(stdout);
            //char us[15],pas[15];
            //strcpy(us,sqlite3_column_text(res,1));
            //strcpy(pas,sqlite3_column_text(res,2));
            //us[strlen(us)]='\0';
            //pas[strlen(pas)]='\0';
            if(strcmp(sqlite3_column_text(res,1),username)==0 && strcmp(sqlite3_column_text(res,2),password)==0){
              logged=1;
              break;
            }
            else break;
        }
        else {
          printf("\nUserul nu exista!");
          break;
        }
      }
    }
  //}





	/*if (read (tdL.cl, &nr,sizeof(int)) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client.\n");
			
			} 
	
	printf ("[Thread %d]Mesajul a fost receptionat...%d\n",tdL.idThread, nr);*/
		      
		      /*pregatim mesajul de raspuns */
		     /* nr++;      
	printf("[Thread %d]Trimitem mesajul inapoi...%d\n",tdL.idThread, nr);
		      
		      
		      /* returnam mesajul clientului */
	 /*if (write (tdL.cl, &nr, sizeof(int)) <= 0)
		{
		 printf("[Thread %d] ",tdL.idThread);
		 perror ("[Thread]Eroare la write() catre client.\n");
		}
	else
		printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",tdL.idThread);	
  */
  sqlite3_finalize(res);
  sqlite3_close(db);
}
