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
#include <arpa/inet.h>
#include <iomanip>
#include <string>
#include <iostream>

using namespace std;


/* portul de conectare la server*/
int port;

int main(int argc, char *argv[]) {
    int sd;            // descriptorul de socket
    struct sockaddr_in server;    // structura folosita pentru conectare
    // mesajul trimis
    int nr = 0;
    char buf[10];

    /* exista toate argumentele in linia de comanda? */
    if (argc != 3) {
        printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    /* stabilim portul */
    port = atoi(argv[2]);

    /* cream socketul */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Eroare la socket().\n");
        return errno;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(argv[1]);
    /* portul de conectare */
    server.sin_port = htons(port);

    /* ne conectam la server */
    if (connect(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
        perror("[client]Eroare la connect().\n");
        return errno;
    }

    /* citirea mesajului */
    int comanda, logged = 0;
    int admin_right, voting_right;
    while (logged == 0) {
        while (1) {
            printf("\nMENIU:");
            printf("\n1. Inregistrare user / admin");
            printf("\n2. Logare");
            printf("\n3. Quit");
            printf("\nIntroduceti numarul comenzii: ");
            cin >> comanda;
            fflush(stdout);
            printf("[client] Am citit %d\n", comanda);
            if (comanda >= 1 && comanda <= 3) break;
            else printf("Numarul comenzii introdus nu exista in meniu. Introduceti alt numar.");
        }

        /* trimiterea mesajului la server */
        if (write(sd, &comanda, sizeof(int)) <= 0) {
            perror("[client]Eroare la write() spre server.\n");
            return errno;
        }

        if (comanda == 3) {
            logged = 0;
            break;
        } else {

            if (comanda == 1) {
                int admin_req, ac_exist, ok_admin = 0;
                char username[15], password[15];
                printf("\nIntroduceti urmatoarele date fara spatii: ");
                printf("\nIntroduceti username-ul dorit: ");
                cin >> username;
                printf("\nIntroduceti parola dorita: ");
                cin >> password;
                printf("\nDoriti drepturi de admin?(0/1): ");
                cin >> admin_req;
                //printf("%s : %s :%d",username,password,admin_req);

                //TRIMITEM USERNAME
                if (write(sd, &username, sizeof(char) * 15) <= 0) {
                    perror("[client]Eroare la write() spre server (username).\n");
                    return errno;
                }

                //TRIMITEM PAROLA
                if (write(sd, &password, sizeof(char) * 15) <= 0) {
                    perror("[client]Eroare la write() spre server (password).\n");
                    return errno;
                }
                //TRIMITEM ADMIN REQUEST
                if (write(sd, &admin_req, sizeof(int)) <= 0) {
                    perror("[client]Eroare la write() spre server (admin_request).\n");
                    return errno;
                }
                if (read(sd, &ac_exist, sizeof(int)) < 0) {
                    perror("[client]Eroare la read() de la server.\n");
                    return errno;
                }
                if (ac_exist == 0) {
                    if (admin_req == 1) {
                        char key[15];
                        printf("\nIntroduceti cod cheie (admin, max. 15 caractere): ");
                        cin >> key;
                        if (write(sd, &key, sizeof(char) * 15) <= 0) {
                            perror("[client]Eroare la write() spre server (admin_request).\n");
                            return errno;
                        }
                        if (read(sd, &ok_admin, sizeof(int)) < 0) {
                            perror("[client]Eroare la read() de la server.\n");
                            return errno;
                        }
                        if (ok_admin == 1) printf("\nAti introdus codul corect si v-ati inregistrat cu succes!");
                        else printf("\nCodul introdus este gresit!\n");
                    } else printf("\nContul a fost creat cu succes!");
                } else printf("\nAcest cont exista deja, introduceti alt username!");
            } else if (comanda == 2) {
                char username[15], password[15];
                bzero(username, 15);
                bzero(password, 15);
                printf("\nIntroduceti urmatoarele date fara spatii: ");
                printf("\nUsername: ");
                cin >> username;
                printf("\nPassword: ");
                cin >> password;

                if (write(sd, &username, sizeof(char) * 15) <= 0) {
                    perror("[client]Eroare la write() spre server (username).\n");
                    return errno;
                }

                if (write(sd, &password, sizeof(char) * 15) <= 0) {
                    perror("[client]Eroare la write() spre server (password).\n");
                    return errno;
                }
                if (read(sd, &logged, sizeof(int)) < 0) {
                    perror("[client]Eroare la read() de la server.\n");
                    return errno;
                }
                printf("\n%d", logged);
                if (logged == 1) {
                    if (read(sd, &admin_right, sizeof(int)) < 0) {
                        perror("[client]Eroare la read() de la server.\n");
                        return errno;
                    }
                    if (read(sd, &voting_right, sizeof(int)) < 0) {
                        perror("[client]Eroare la read() de la server.\n");
                        return errno;
                    }
                    printf("\nBine ai venit %s!", username);
                    printf("\nSTATUS: \nadmin = %d \ndrept de vot = %d", admin_right, voting_right);
                } else printf("\nAcest cont nu exista, ati introdus datele gresit sau este logat!");
            }
        }


        /* citirea raspunsului dat de server
           (apel blocant pina cind serverul raspunde) */
        /* if (read(sd, &comanda, sizeof(int)) < 0) {
             perror("[client]Eroare la read() de la server.\n");
             return errno;
         }

         printf("[client]Mesajul primit este: %d\n", comanda);*/
    }
    while (logged == 1) {
        int comanda2;
        printf("\nMENIU:");
        printf("\n1. Add Song - adauga o melodie");
        printf("\n2. Vote Song - voteaza o melodie");
        printf("\n3. Top Songs general sau pe genuri muzicale");
        printf("\n4. Post Comment - posteaza un comentariu legat de o melodie");
        printf("\n5. Quit");
        if (admin_right == 1) {
            printf("\n6. Ban Vote - restrictioneaza votul unui user");
            printf("\n7. UnBan Vote - scoate restrictia de a vota a unui user");
            printf("\n8. Delete Song - sterge o melodie");
        }
        while (1) {
            printf("\nIntroduceti numarul comenzii: ");
            cin >> comanda2;
            fflush(stdout);
            printf("[client] Am citit %d\n", comanda2);
            if ((comanda2 >= 1 && comanda2 <= 5) || (comanda2 >= 1 && comanda2 <= 8 && admin_right == 1)) break;
            else printf("Numarul comenzii introdus nu exista in meniu. Introduceti alt numar.");
        }

        if (write(sd, &comanda2, sizeof(int)) <= 0) {
            perror("[client]Eroare la write() spre server (comanda2).\n");
            return errno;
        }

        if (comanda2 == 5) {
            break;
        } else if (comanda2 == 1) {
            char song[255], SongDescribe[1001], typeOfSong[50], link[255];
            bzero(song, 255);
            bzero(SongDescribe, 1001);
            bzero(typeOfSong, 50);
            bzero(link, 255);
            int songAdded;
            if (read(sd, &comanda2, sizeof(int)) < 0) {
                perror("[client]Eroare la read() de la server.\n");
                return errno;
            }
            printf("\nComanda1: %d", comanda2);
            printf("\nIntroduceti numele melodiei: ");
            fflush(stdout);
            if (read(0, &song, 255) <= 0) {
                printf("\n[client] Eroare citire nume melodie de la tastatura.");
            }
            printf("\nIntroduceti o descriere melodiei: ");
            fflush(stdout);
            if (read(0, &SongDescribe, 1001) <= 0) {
                printf("\n[client] Eroare citire descrierii melodiei de la tastatura.");
            }
            printf("\nGenuri disponibile: Hip-Hop Rock Pop Country Dupstep Clasic ");
            fflush(stdout);
            printf("\nIntroduceti genurile melodiei (cu spatii intre ele exact ca mai sus): ");
            fflush(stdout);
            bzero(typeOfSong, 50);
            if (read(0, &typeOfSong, 50) <= 0) {
                printf("\n[client] Eroare la citirea genurile melodiei de la tastatura.");
            }

            printf("\nIntroduceti un link catre un site unde se poate asculta melodia (ex:youtube): ");
            fflush(stdout);
            if (read(0, &link, 255) <= 0) {
                printf("\n[client] Eroare la citirea link-ului melodiei de la tastatura.");
            }
            song[strlen(song) - 1] = '\0';
            SongDescribe[strlen(SongDescribe) - 1] = '\0';
            typeOfSong[strlen(typeOfSong) - 1] = '\0';
            link[strlen(link) - 1] = link[strlen(link)] = '\0';
            if (write(sd, &song, sizeof(char) * 255) <= 0) {
                perror("[client]Eroare la write() spre server (comanda2).\n");
                return errno;
            }
            if (write(sd, &SongDescribe, sizeof(char) * 1001) <= 0) {
                perror("[client]Eroare la write() spre server (comanda2).\n");
                return errno;
            }
            if (write(sd, &typeOfSong, sizeof(char) * 50) <= 0) {
                perror("[client]Eroare la write() spre server (comanda2).\n");
                return errno;
            }
            if (write(sd, &link, sizeof(char) * 255) <= 0) {
                perror("[client]Eroare la write() spre server (comanda2).\n");
                return errno;
            }
            printf("%\n %s : %s : %s : %s", song, SongDescribe, typeOfSong, link);
            if (read(sd, &songAdded, sizeof(int)) < 0) {
                perror("[client]Eroare la read() de la server.\n");
                return errno;
            }
            printf("\nMelodie adaugata: %d", songAdded);
        } else if (comanda2 == 2) {
            int songsLength;
            char nameS[100][255];
            int OK_vot;
            bzero(nameS, 25500);
            if (read(sd, &songsLength, sizeof(int)) < 0) {
                perror("[client]Eroare la read() de la server.\n");
                return errno;
            }
            if (read(sd, &nameS, sizeof(char) * 25500) < 0) {
                perror("[client]Eroare la read() de la server.\n");
                return errno;
            }
            for (int i = 1; i <= songsLength; i++) {
                cout << i << ". " << nameS[i] << endl;
            }
            int number;
            printf("\nIntroduceti numarul melodiei pe care doriti sa o votati: ");
            fflush(stdout);
            cin >> number;
            printf("\n%d", number);
            if (write(sd, &number, sizeof(int)) <= 0) {
                perror("[client]Eroare la write() spre server (comanda2).\n");
                return errno;
            }

            if (read(sd, &OK_vot, sizeof(int)) < 0) {
                perror("[client]Eroare la read() de la server.\n");
                return errno;
            }
            printf("\n%d", OK_vot);
        } else if (comanda2 == 3) {
            int gen, Len, vSongs[100];
            char TopSongs[100][255];
            while (1) {

                printf("\nTop-uri muzicale: ");
                printf("\n0. General");
                printf("\n1. Hip-Hop");
                printf("\n2. Rock");
                printf("\n3. Pop");
                printf("\n4. Country");
                printf("\n5. Dupstep");
                printf("\n6. Clasic");
                printf("\nIntroduceti numarul topului dorit: ");
                cin >> gen;
                if (gen >= 0 && gen <= 6) break;
                else printf("\nIntroduceti unul din numerele de mai sus!");
            }
            if (write(sd, &gen, sizeof(int)) <= 0) {
                perror("[client]Eroare la write() spre server (comanda2).\n");
                return errno;
            }
            if (read(sd, &Len, sizeof(int)) < 0) {
                perror("[client]Eroare la read() de la server.\n");
                return errno;
            }
            if (read(sd, &vSongs, sizeof(int) * Len) < 0) {
                perror("[client]Eroare la read() de la server.\n");
                return errno;
            }
            if (read(sd, &TopSongs, sizeof(char) * 25500) < 0) {
                perror("[client]Eroare la read() de la server.\n");
                return errno;
            }
            cout << endl << "TOP:" << endl;
            for (int i = 1; i <= Len; i++)
                cout << i << ". " << TopSongs[i] << "- voturi: " << vSongs[i] << endl;
        } else if (comanda2 == 4) {
            int nrOfSongs = 0, number;
            char AllSongs[100][255], comment[500];
            bzero(AllSongs, 25500);
            bzero(comment, 500);
            if (read(sd, &nrOfSongs, sizeof(int)) < 0) {
                perror("[client]Eroare la read() de la server.\n");
                return errno;
            }
            if (read(sd, &AllSongs, sizeof(char) * 25500) < 0) {
                perror("[client]Eroare la read() de la server.\n");
                return errno;
            }
            cout << "Alegeti o melodie pentru a comenta: " << endl;
            for (int i = 1; i <= nrOfSongs; i++) {
                cout << i << ". " << AllSongs[i] << endl;
            }
            while (1) {
                cout << "Introduceti numarul unei piese: ";
                cin >> number;
                fflush(stdout);
                if (number >= 1 && number <= nrOfSongs) break;
                else cout << "Piesa aleasa nu exista! Alegeti alta!";
            }
            fflush(stdout);
            cout << "Comentariu: ";
            fflush(stdout);
            if (read(0, &comment, 500) <= 0) {
                printf("\n[client] Eroare citire nume melodie de la tastatura.");
            }
            comment[strlen(comment) - 1] = '\0';

            if (write(sd, &number, sizeof(int)) <= 0) {
                perror("[client]Eroare la write() spre server (comanda2).\n");
                return errno;
            }
            if (write(sd, &comment, sizeof(char) * 500) <= 0) {
                perror("[client]Eroare la write() spre server (comanda2).\n");
                return errno;
            }

        }

    }

/* inchidem conexiunea, am terminat */
    close(sd);
}