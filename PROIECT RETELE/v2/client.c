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
int logged=0;
/* portul de conectare la server*/
int port;
int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  		// mesajul trimis
  int nr=0;
  char buf[10];

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

  //INCEPUT
  //while(1)
  //{
    
    //while(logged==0)
    { 
      int cmd;
      char username[15], password[15];
      while(1)
      {
      
      printf("\nMENIU:\n");
      printf("1. Logare\n");
      printf("2. Inregistrare (admin / user)\n");
      printf("3. Quit\n");
      printf("Introduceti numarul comenzii alese: ");
      fflush(stdout);
      scanf("%d",&cmd);
      //printf("\n%d",cmd);

      if(cmd==1 || cmd==2 || cmd==3) break;
      else printf("Numarul comenzii introduse este gresit! Mai incercati\n");
      }
      // trimitem comanda spre server
      if (write (sd,&cmd,sizeof(int)) <= 0)
      {
        perror ("[client]Eroare la write() spre server (cmd).\n");
        return errno;
      }

      if(cmd==1) //LOGIN
      {
        while(logged==0){

        
          bzero(username,15);
          bzero(password,15);

          printf("\n Va rugam introduceti fara spatii urmatoarele informatii despre contul dumneavoastra: ");
          printf("\nUsername: ");
          scanf("%s",username);
          printf("\nPassword: ");
          scanf("%s",password);
          
          username[strlen(username)]='\0';
          password[strlen(password)]='\0';
          printf("\n%s : %s",username,password);
           // trimitere username catre server
           int nr1= strlen(username), nr2=strlen(password);
          if (write (sd,&nr1,sizeof(int)) <= 0)
            {
              perror ("[client]Eroare la write() spre server (trimitere userlength).\n");
              return errno;
            }
          if (write (sd,&username,sizeof(char)*strlen(username)) <= 0)
            {
              perror ("[client]Eroare la write() spre server (trimitere username).\n");
              return errno;
            }
  
          // trimitere password catre server

          if (write (sd,&nr2,sizeof(int)) <= 0)
           {
             perror ("[client]Eroare la write() spre server (trimitere passlength).\n");
             return errno;
           }
          if (write (sd,&password,sizeof(char)*strlen(password)) <= 0)
           {
             perror ("[client]Eroare la write() spre server (trimitere password).\n");
             return errno;
           }
           break;
        }

      }

  /* citirea mesajului */
  //printf ("[client]Introduceti un numar: ");
  //fflush (stdout);
  //read (0, buf, sizeof(buf));
  //nr=atoi(buf);
  //scanf("%d",&nr);
  
  //printf("[client] Am citit %d\n",nr);

  /* trimiterea mesajului la server */
  /*if (write (sd,&nr,sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
*/
  /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
  /*if (read (sd, &nr,sizeof(int)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }*/
  /* afisam mesajul primit */
  //printf ("[client]Mesajul primit este: %d\n", nr);

    }

  //}
  /* inchidem conexiunea, am terminat */
  close (sd);
}
