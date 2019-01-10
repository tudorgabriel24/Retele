/* cliTCPIt.c - Exemplu de client TCP
   Trimite un numar la server; primeste de la server numarul incrementat.
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  		// mesajul trimis
  int OK;
  char buf1[25],buf2[10];

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }


//                                            MENIU 1
  OK=0;
  int drept_de_vot, admin,logged=0;
  while(logged==0)
  {
  char comanda[2];
  printf("\nMENIU:");
  printf("\n1.Inregistrare user/admin");
  printf("\n2.Logare");
  printf("\n3.Quit\n");
  bzero(comanda,2);
  printf("\nIntroduceti numarul comenzii: ");
  fflush(stdout);
  scanf("%s",comanda);
  comanda[strlen(comanda)]='\0';
  if (write (sd,&comanda,sizeof(char)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
  if(strcmp(comanda,"2")==0)//       *         LOGARE
  {
  
  /* citirea mesajului */
  printf ("[client]Introduceti username-ul: ");
  fflush (stdout);
  scanf("%s",buf1);
  printf("%s",buf1);
  printf ("[client]Introduceti parola: ");
  scanf("%s",buf2);
  printf("%s",buf2);
  strcat(buf1," ");
  strcat(buf1,buf2);
  
  buf1[strlen(buf1)]='\0';

  //scanf("%d",&nr);
  printf("[client]Am citit %s\n",buf1);
  /* trimiterea mesajului la server */
  int nr= strlen(buf1);
  printf("%d",nr);
  if (write (sd,&nr,sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }

  if (write (sd,&buf1,sizeof(char)*strlen(buf1)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }

  /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
  if (read (sd, &OK,sizeof(int)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  if (read (sd, &drept_de_vot,sizeof(int)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
   if (read (sd, &admin,sizeof(int)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
    printf("\n%d",OK);
  /* afisam mesajul primit */
  if(OK==1)
  {
  printf ("[client]Mesajul primit este: %d\n", OK);
  printf("\nUtilizator logat:%s",buf1);
  printf("\nDrept de vot:%d",drept_de_vot);
  printf("\nDrept de admin: %d",admin);
  logged =1;
  }
  else printf("\nA-ti introdus numele sau parola gresite!");
  }
  if(strcmp(comanda,"1")==0)  //                  INREGISTRARE
  {
    char setadmin[1]; // variabila folosita pentru inregistrarea adminilor
    admin=0; // starea de admin a contului
    while(1)
    {
    bzero(setadmin,1);
    printf("\nInregistrare admin?(1/0): ");
    fflush(stdout);
    scanf("%s",setadmin);
    printf("\n%s",setadmin);
    if(strcmp(setadmin,"1")==0 || strcmp(setadmin,"0")==0) break;
    else printf("Raspundeti doar cu '1' (da) sau '0' (nu)!");
    }

    if (write (sd,&setadmin,sizeof(char)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    if(strcmp(setadmin,"1")==0)
    {
      char key[20],cod[20];
      if (read (sd, &key,sizeof(char)*16) < 0)
      {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
      }
      printf("\nIntrodu key-ul pentru admin: ");
      scanf("%s",cod);
      if(strcmp(key,cod)==0) admin=1;
      if(write (sd,&admin,sizeof(int)) <= 0)
      {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
      }
      if(admin==1) strcpy(setadmin,"0");
    }
    if(strcmp(setadmin,"0")==0){

      printf ("[client]Introduceti username-ul: ");
      fflush (stdout);
      scanf("%s",buf1);
      printf("%s",buf1);
      printf ("[client]Introduceti parola: ");
      scanf("%s",buf2);
      printf("%s",buf2);
      strcat(buf1," ");
      strcat(buf1,buf2);
  
      buf1[strlen(buf1)]='\0';

      //scanf("%d",&nr);
      printf("[client]Am citit %s\n",buf1);
      /* trimiterea mesajului la server */
      int nr= strlen(buf1);
      printf("%d",nr);
      if (write (sd,&nr,sizeof(int)) <= 0)
      {
       perror ("[client]Eroare la write() spre server.\n");
       return errno;
      }
      if (write (sd,&buf1,sizeof(char)*strlen(buf1)) <= 0)
      {
       perror ("[client]Eroare la write() spre server.\n");
       return errno;
      }
      /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
      if (read (sd, &OK,sizeof(int)) < 0)
      {
        perror ("[client]Eroare la read() de la server.\n");
        return errno;
      }

      if(OK==1) printf("Acest cont exista deja!");
      else printf("Cont creat cu succes!");
    }
  }
  
  
}
/* inchidem conexiunea, am terminat */
  close (sd);
}