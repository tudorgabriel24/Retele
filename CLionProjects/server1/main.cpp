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
#include "json.hpp"
#include <mutex>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

std::mutex mutexx; //


/* portul folosit */
#define PORT 2908
using namespace std;
using json = nlohmann::json;

/* codul de eroare returnat de anumite apeluri */

typedef struct thData {
    int idThread; //id-ul thread-ului tinut in evidenta de acest program
    int cl; //descriptorul intors de accept
} thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);

int main() {
    struct sockaddr_in server;    // structura folosita de server
    struct sockaddr_in from;
    int nr;        //mesajul primit de trimis la client
    int sd;        //descriptorul de socket
    int pid;
    pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
    int i = 0;


    /* crearea unui socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("[server]Eroare la socket().\n");
        return errno;
    }
    /* utilizarea optiunii SO_REUSEADDR */
    int on = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    /* pregatirea structurilor de date */
    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
    /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    /* utilizam un port utilizator */
    server.sin_port = htons(PORT);

    /* atasam socketul */
    if (bind(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
        perror("[server]Eroare la bind().\n");
        return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen(sd, 2) == -1) {
        perror("[server]Eroare la listen().\n");
        return errno;
    }
    /* servim in mod concurent clientii...folosind thread-uri */
    while (1) {
        int client;
        thData *td; //parametru functia executata de thread
        int length = sizeof(from);

        printf("[server]Asteptam la portul %d...\n", PORT);
        fflush(stdout);

        // client= malloc(sizeof(int));
        /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
        if ((client = accept(sd, (struct sockaddr *) &from, (socklen_t *) &length)) < 0) {
            perror("[server]Eroare la accept().\n");
            continue;
        }

        /* s-a realizat conexiunea, se astepta mesajul */

        // int idThread; //id-ul threadului
        // int cl; //descriptorul intors de accept

        td = (struct thData *) malloc(sizeof(struct thData));
        td->idThread = i++;
        td->cl = client;

        pthread_create(&th[i], NULL, &treat, td);

    }//while
};

static void *treat(void *arg) {
    struct thData tdL;
    tdL = *((struct thData *) arg);
    printf("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
    fflush(stdout);
    pthread_detach(pthread_self());
    raspunde((struct thData *) arg);
    /* am terminat cu acest client, inchidem conexiunea */
    close((intptr_t) arg);
    return (NULL);

};

int log_search(char *user, char *pass, int &admin_req, int &vote_r, int &reg_check) {
    int OK = reg_check = 0;
    admin_req = vote_r = -1;
    ifstream file("../UsersAndSongs.json");
    json JUsers;
    JUsers = json::parse(file);
    int JLength = JUsers["users"].size();
    //printf("\n%s : %s",user,pass);
    //printf("%d\n",JLength);
    for (int i = 0; i < JLength; ++i) {
        if (JUsers["users"][i]["username"] == user) {
            reg_check = 1;
            if (JUsers["users"][i]["password"] == pass) {
                OK = 1;
                admin_req = JUsers["users"][i]["admin"];
                vote_r = JUsers["users"][i]["voting_right"];
                if (JUsers["users"][i]["logged"] == 1) {
                    OK = 0;
                    break;
                }
            }
        }
    }
    return OK;
}

void logInOn(char *user, char *pass, int nr) {
    json User;
    ifstream file("../UsersAndSongs.json");
    User = json::parse(file);
    int L;
    L = User["users"].size();
    if (L > 0) {
        printf("\n%d", L);
        for (int i = 0; i < L; i++) {
            if (User["users"][i]["username"] == user)
                if (User["users"][i]["password"] == pass) {
                    User["users"][i]["logged"] = nr;
                    printf("\n%d", nr);
                }
        }
        ofstream file2("../UsersAndSongs.json");
        file2 << setw(4) << User << endl;
    }
}

void addMusic(char *songName, char *describe, char *typeOfSong, char *link, int &added) {
    added = 0;
    ifstream file("../UsersAndSongs.json");
    json JSongs;
    JSongs = json::parse(file);
    int JLength = JSongs["songs"].size();
    for (int i = 0; i < JLength; i++) {
        if (JSongs["songs"][i]["name"] == songName)
            break;
        if (i == JLength - 1) added = 1;
    }
    if (JLength == 0) added = 1;
    if (added == 1) {
        json newSong;
        newSong["name"] = songName;
        newSong["describe"] = describe;
        newSong["votes"] = 0;
        char *temp;
        temp = strtok(typeOfSong, " ");
        int i = -1;
        while (temp != NULL) {
            newSong["typeOfSong"][++i]["text"] = temp;
            printf("\n%s", temp);
            temp = strtok(NULL, " ");
        }
        newSong["typeOfSong"][++i]["text"] = "General";
        newSong["link"] = link;
        ofstream file2("../UsersAndSongs.json");
        JSongs["songs"].push_back(newSong);
        file2 << setw(4) << JSongs << endl;
    }
}

void voteSong(int number, char *user, int &OK) {
    ifstream file("../UsersAndSongs.json");
    json JSongs;
    JSongs = json::parse(file);
    OK = 1;
    int NrOfVotes = JSongs["songs"][number]["votes"];
    if (NrOfVotes != 0) {
        for (int j = 0; j < NrOfVotes; j++)
            if (JSongs["songs"][number]["WhoVotes"][j]["name"] == user) {
                OK = 0;
                break;
            }
    }
    if (OK == 1) {

        json temps;
        temps["name"] = user;
        JSongs["songs"][number]["votes"] = NrOfVotes + 1;
        JSongs["songs"][number]["WhoVotes"].push_back(temps);
        ofstream file2("../UsersAndSongs.json");
        file2 << setw(4) << JSongs << endl;
    }
}

void raspunde(void *arg) {
    int cmd, i = 0, logged = 0;
    char UserName[15], PassWord[15];
    struct thData tdL;
    tdL = *((struct thData *) arg);
    while (logged == 0) {
        if (read(tdL.cl, &cmd, sizeof(int)) <= 0) {
            printf("[Thread %d]\n", tdL.idThread);
            perror("Eroare la read() de la client.\n");

        }

        printf("[Thread %d]Mesajul a fost receptionat...%d\n", tdL.idThread, cmd);

        /*pregatim mesajul de raspuns */

        if (cmd == 1) // INREGISTRARE
        {
            int adm_req, admin, voting_right, reg_check;

            char username[15], password[15];
            if (read(tdL.cl, &username, sizeof(char) * 15) <= 0) {
                printf("[Thread %d]\n", tdL.idThread);
                perror("Eroare la read() de la client (username).\n");

            }
            printf("Am primit username: %s\n", username);
            if (read(tdL.cl, &password, sizeof(char) * 15) <= 0) {
                printf("[Thread %d]\n", tdL.idThread);
                perror("Eroare la read() de la client (password).\n");

            }
            printf("Am primit password: %s\n", password);
            if (read(tdL.cl, &adm_req, sizeof(int)) <= 0) {
                printf("[Thread %d]\n", tdL.idThread);
                perror("Eroare la read() de la client (admin_request).\n");

            }
            printf("Am primit admin_request: %d\n", adm_req);

            int temp = log_search(username, password, admin, voting_right, reg_check);

            printf("Account exist: %d", reg_check);

            if (write(tdL.cl, &reg_check, sizeof(int)) <= 0) {
                printf("[Thread %d] ", tdL.idThread);
                perror("[Thread]Eroare la write() catre client.\n");
            } else
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);

            if (reg_check == 0) {
                admin = 0;
                if (adm_req == 1) {
                    char key[15];

                    if (read(tdL.cl, &key, sizeof(char) * 15) <= 0) {
                        printf("[Thread %d]\n", tdL.idThread);
                        perror("Eroare la read() de la client (admin_request).\n");

                    }
                    printf("Am primit key_admin: %s\n", key);
                    if (strcmp(key, "COD198") == 0) admin = 1;
                    else admin = -1;
                    if (write(tdL.cl, &admin, sizeof(int)) <= 0) {
                        printf("[Thread %d] ", tdL.idThread);
                        perror("[Thread]Eroare la write() catre client.\n");
                    } else
                        printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);

                }
                if (admin != -1) {
                    json newUser;
                    newUser["username"] = username;
                    newUser["password"] = password;
                    newUser["admin"] = admin;
                    newUser["voting_right"] = 1;
                    newUser["number_of_votes"] = 0;
                    newUser["logged"] = 0;
                    json Json_users;
                    ifstream fisier("../UsersAndSongs.json");
                    Json_users = json::parse(fisier);
                    Json_users["users"].push_back(newUser);
                    ofstream fisier2("../UsersAndSongs.json");
                    fisier2 << setw(4) << Json_users << endl;

                }
                printf("\n%d : %d", admin, voting_right);
            }
        } else if (cmd == 2) // LOGARE
        {

            char username[15], password[15];
            bzero(username, 15);
            bzero(password, 15);
            int admin, voting_right, reg_check;
            if (read(tdL.cl, &username, sizeof(char) * 15) <= 0) {
                printf("[Thread %d]\n", tdL.idThread);
                perror("Eroare la read() de la client (username).\n");

            }
            printf("Am primit username: %s\n", username);
            if (read(tdL.cl, &password, sizeof(char) * 15) <= 0) {
                printf("[Thread %d]\n", tdL.idThread);
                perror("Eroare la read() de la client (password).\n");

            }
            logged = log_search(username, password, admin, voting_right, reg_check);
            if (logged == 1) printf("\nLOGAT: %d", logged);
            if (write(tdL.cl, &logged, sizeof(int)) <= 0) {
                printf("[Thread %d] ", tdL.idThread);
                perror("[Thread]Eroare la write() catre client.\n");
            } else
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
            if (logged == 1) {
                strcpy(UserName, username);
                strcpy(PassWord, password);
                printf("\n%s : %s", UserName, PassWord);
                logInOn(username, password, 1);


                if (write(tdL.cl, &admin, sizeof(int)) <= 0) {
                    printf("[Thread %d] ", tdL.idThread);
                    perror("[Thread]Eroare la write() catre client.\n");
                } else
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
                if (write(tdL.cl, &voting_right, sizeof(int)) <= 0) {
                    printf("[Thread %d] ", tdL.idThread);
                    perror("[Thread]Eroare la write() catre client.\n");
                } else
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);

            }
        } else if (cmd == 3) {
            logged = 0;
            printf("[Thread %d]Clientul a incheiat conexiunea cu serverul", tdL.idThread);
            break;
        }

    }

    while (logged == 1) {
        int cmd2;
        if (read(tdL.cl, &cmd2, sizeof(int)) <= 0) {
            printf("[Thread %d]\n", tdL.idThread);
            perror("Eroare la read() de la client (cmd2).\n");
        }

        if (cmd2 == 5) {
            logged = 0;
            logInOn(UserName, PassWord, 0);
            printf("[Thread %d]Clientul a incheiat conexiunea cu serverul", tdL.idThread);
            break;
        } else if (cmd2 == 1) {
            char song[255], SongDescribe[1001], typeOfSong[50], link[255];
            int added = 0;
            if (write(tdL.cl, &cmd2, sizeof(int)) <= 0) {
                printf("[Thread %d] ", tdL.idThread);
                perror("[Thread]Eroare la write() catre client.\n");
            } else
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);

            if (read(tdL.cl, &song, sizeof(char) * 255) <= 0) {
                printf("[Thread %d]\n", tdL.idThread);
                perror("Eroare la read() de la client (song).\n");
            }

            if (read(tdL.cl, &SongDescribe, sizeof(char) * 1001) <= 0) {
                printf("[Thread %d]\n", tdL.idThread);
                perror("Eroare la read() de la client (SongDescribe).\n");
            }

            if (read(tdL.cl, &typeOfSong, sizeof(char) * 50) <= 0) {
                printf("[Thread %d]\n", tdL.idThread);
                perror("Eroare la read() de la client (typeOfSong).\n");
            }

            if (read(tdL.cl, &link, sizeof(char) * 255) <= 0) {
                printf("[Thread %d]\n", tdL.idThread);
                perror("Eroare la read() de la client (link).\n");
            }
            printf("\n%s : %s : %s", song, SongDescribe, typeOfSong);
            printf("\n%s", link);
            cout << ":" << strlen(link);
            link[strlen(link)] = '\0';
            addMusic(song, SongDescribe, typeOfSong, link, added);
            printf("\nMelodie adaugata : %d", added);
            if (write(tdL.cl, &added, sizeof(int)) <= 0) {
                printf("[Thread %d] ", tdL.idThread);
                perror("[Thread]Eroare la write() catre client.\n");
            } else
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
            bzero(song, 255);
            bzero(SongDescribe, 1001);
            bzero(typeOfSong, 50);

        } else if (cmd2 == 2) {
            int nrOfSongs = 0, number, OK = 0;
            char AllSongs[100][255];
            bzero(AllSongs, 25500);
            json JSONGS;
            ifstream file("../UsersAndSongs.json");
            JSONGS = json::parse(file);
            nrOfSongs = JSONGS["songs"].size();
            printf("\nNr. de melodii: %d", nrOfSongs);
            for (int i = 0; i < nrOfSongs; i++) {
                int j = i + 1;
                string temp = JSONGS["songs"][i]["name"];
                strcpy(AllSongs[j], temp.data());
                fflush(stdout);
                cout << j << ". " << AllSongs[j] << endl;
            }
            if (write(tdL.cl, &nrOfSongs, sizeof(int)) <= 0) {
                printf("[Thread %d] ", tdL.idThread);
                perror("[Thread]Eroare la write() catre client (nrOfSongs).\n");
            } else
                printf("[Thread %d]Mesajul a fost trasmis cu succes (nrOfSongs2).\n", tdL.idThread);

            if (write(tdL.cl, &AllSongs, sizeof(char) * 25500) <= 0) {
                printf("[Thread %d] ", tdL.idThread);
                perror("[Thread]Eroare la write() catre client (AllSongs).\n");
            } else
                printf("[Thread %d]Mesajul a fost trasmis cu succes (Top).\n", tdL.idThread);

            if (read(tdL.cl, &number, sizeof(int)) <= 0) {
                printf("[Thread %d]\n", tdL.idThread);
                perror("Eroare la read() de la client (link).\n");
            }
            printf("\nNumarul melodiei pentru vot este: %d", number);
            number--;
            voteSong(number, UserName, OK);

            if (write(tdL.cl, &OK, sizeof(int)) <= 0) {
                printf("[Thread %d] ", tdL.idThread);
                perror("[Thread]Eroare la write() catre client.\n");
            } else
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
        } else if (cmd2 == 3) {
            int gen, nrOfSongs = 0, vSongs[100], OK;
            char TopSongs[100][255];
            char GenMelodie[15];
            bzero(vSongs, 100);
            bzero(TopSongs, 25500);
            bzero(GenMelodie, 15);
            if (read(tdL.cl, &gen, sizeof(int)) <= 0) {
                printf("[Thread %d]\n", tdL.idThread);
                perror("Eroare la read() de la client (link).\n");
            }
            printf("\nNumar gen melodie ales: %d", gen);
            if (gen == 0) strcpy(GenMelodie, "General");
            else if (gen == 1) strcpy(GenMelodie, "Hip-Hop");
            else if (gen == 2) strcpy(GenMelodie, "Rock");
            else if (gen == 3) strcpy(GenMelodie, "Pop");
            else if (gen == 4) strcpy(GenMelodie, "Country");
            else if (gen == 5) strcpy(GenMelodie, "Dupstep");
            else if (gen == 6) strcpy(GenMelodie, "Clasic");
            printf("\nGen: %s", GenMelodie);
            json JSONGS;
            ifstream file("../UsersAndSongs.json");
            JSONGS = json::parse(file);
            int JLength = JSONGS["songs"].size();
            printf("\nNr. de melodii: %d", JLength);
            for (int i = 0; i < JLength; i++) {
                OK = 0;
                //cout<<"AICI: "<<i <<endl;
                int JTYPE = JSONGS["songs"][i]["typeOfSong"].size();
                //printf("\n%d",JTYPE);
                if (JTYPE != 0) {
                    for (int j = 0; j < JTYPE; j++) {

                        if (JSONGS["songs"][i]["typeOfSong"][j]["text"] == GenMelodie) {
                            OK = 1;
                            break;
                        }
                        //cout<< JSONGS["songs"][i]["typeOfSong"][j]["text"]<<endl;
                    }
                }
                cout << endl << OK;
                if (OK == 1) {
                    nrOfSongs++;
                    vSongs[nrOfSongs] = JSONGS["songs"][i]["votes"];
                    string temp = JSONGS["songs"][i]["name"];
                    strcpy(TopSongs[nrOfSongs], temp.data());
                    fflush(stdout);
                    cout << nrOfSongs << " " << TopSongs[nrOfSongs] << endl;
                }
            }
            for (int i = 1; i <= nrOfSongs; i++)
                cout << i << " " << TopSongs[i] << endl;
            for (int i = 1; i < nrOfSongs; i++)
                for (int j = i + 1; j <= nrOfSongs; j++) {
                    if (vSongs[i] < vSongs[j]) {
                        char temp[255];
                        int temp2;
                        strcpy(temp, TopSongs[i]);
                        strcpy(TopSongs[i], TopSongs[j]);
                        strcpy(TopSongs[j], temp);
                        temp2 = vSongs[i];
                        vSongs[i] = vSongs[j];
                        vSongs[j] = temp2;
                    }
                }
            for (int i = 1; i <= nrOfSongs; i++)
                cout << i << " " << TopSongs[i] << " " << vSongs[i] << endl;

            if (write(tdL.cl, &nrOfSongs, sizeof(int)) <= 0) {
                printf("[Thread %d] ", tdL.idThread);
                perror("[Thread]Eroare la write() catre client (nrOfSongs).\n");
            } else
                printf("[Thread %d]Mesajul a fost trasmis cu succes (nrOfSongs).\n", tdL.idThread);

            if (write(tdL.cl, &vSongs, sizeof(int) * nrOfSongs) <= 0) {
                printf("[Thread %d] ", tdL.idThread);
                perror("[Thread]Eroare la write() catre client (vSongs).\n");
            } else
                printf("[Thread %d]Mesajul a fost trasmis cu succes (vSongs).\n", tdL.idThread);
            if (write(tdL.cl, &TopSongs, sizeof(char) * 25500) <= 0) {
                printf("[Thread %d] ", tdL.idThread);
                perror("[Thread]Eroare la write() catre client (Top).\n");
            } else
                printf("[Thread %d]Mesajul a fost trasmis cu succes (Top).\n", tdL.idThread);
        } else if (cmd2 == 4) {
            int nrOfSongs = 0;
            char AllSongs[100][255], comment[500];
            bzero(AllSongs, 25500);
            json JSONGS;
            ifstream file("../UsersAndSongs.json");
            JSONGS = json::parse(file);
            nrOfSongs = JSONGS["songs"].size();
            printf("\nNr. de melodii: %d", nrOfSongs);
            for (int i = 0; i < nrOfSongs; i++) {
                int j = i + 1;
                string temp = JSONGS["songs"][i]["name"];
                strcpy(AllSongs[j], temp.data());
                fflush(stdout);
                cout << j << ". " << AllSongs[j] << endl;
            }
            if (write(tdL.cl, &nrOfSongs, sizeof(int)) <= 0) {
                printf("[Thread %d] ", tdL.idThread);
                perror("[Thread]Eroare la write() catre client (nrOfSongs).\n");
            } else
                printf("[Thread %d]Mesajul a fost trasmis cu succes (nrOfSongs2).\n", tdL.idThread);

            if (write(tdL.cl, &AllSongs, sizeof(char) * 25500) <= 0) {
                printf("[Thread %d] ", tdL.idThread);
                perror("[Thread]Eroare la write() catre client (AllSongs).\n");
            } else
                printf("[Thread %d]Mesajul a fost trasmis cu succes (Top).\n", tdL.idThread);
            int number;
            if (read(tdL.cl, &number, sizeof(int)) <= 0) {
                printf("[Thread %d]\n", tdL.idThread);
                perror("Eroare la read() de la client (link).\n");
            }
            if (read(tdL.cl, &comment, sizeof(char) * 500) <= 0) {
                printf("[Thread %d]\n", tdL.idThread);
                perror("Eroare la read() de la client (link).\n");
            }
            cout << number << " " << comment << endl;
            int nrComments = JSONGS["songs"][number - 1]["comments"].size();
            JSONGS["songs"][number - 1]["comments"][nrComments]["text"] = comment;
            JSONGS["songs"][number - 1]["comments"][nrComments]["name"] = UserName;
            ofstream file2("../UsersAndSongs.json");
            file2 << setw(4) << JSONGS << endl;
        }
    }


}
