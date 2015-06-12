/***************************************/
/*             TCP client              */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024
#define PORT_NO 6363
#define error(a,b) fprintf(stderr, a, b)

typedef struct lap
{
   char szin[40];
   char szam[40];
} LAP;

void lapot(int l, LAP kez[], int serverSocket)
{
   char message[BUFSIZE+1];
   recv(serverSocket, message, BUFSIZE, 0);
   printf("%s ", message);
   strcpy(kez[l].szin,message);
   send(serverSocket, "ok",strlen("ok")+1, 0);
   recv(serverSocket, message, BUFSIZE, 0);
   printf("%s\n", message);
   strcpy(kez[l].szam,message);
   send(serverSocket, "ok",strlen("ok")+1, 0);
}

void rendez(LAP kez[])
{
   LAP tomb[32];
   int j;
   int k=0;

   for(j=0;j<32;j++)
   {
      sprintf(tomb[j].szin,"ures");
      sprintf(tomb[j].szam,"ures");
   }

   for(j=0;j<32;j++)
   {
      if(strcmp(kez[j].szin,"ures")!=0 && strcmp(kez[j].szam,"ures")!=0)
      {
      strcpy(tomb[k].szin,kez[j].szin);
      strcpy(tomb[k].szam,kez[j].szam);
      k++;
      }
   }

   for(j=0;j<32;j++)
   {
      strcpy(kez[j].szin,tomb[j].szin);
      strcpy(kez[j].szam,tomb[j].szam);
   }
}

int main(int argc, char *argv[] ) {// arg count, arg vector   

   /* Declarations */
   int serverSocket;	                       // socket endpt	
   int flags;                      // rcv flags
   struct sockaddr_in server;	     // socket name (addr) of server 
   struct sockaddr_in client;	     // socket name of client 
   int server_size;                // length of the socket addr. server 
   int client_size;                // length of the socket addr. client 
   int bytes;    	                 // length of message 
   int rcvsize;                    // received bytes
   int trnmsize;                   // transmitted bytes
   int err;                        // error code
   int ip;												 // ip address
   char on;                        // 
   char message[BUFSIZE+1];         // datagram dat message area
   char server_addr[16];           // server address	
   int player_number;
   int kartyak_szama;
   char utasitas[50];
   int feladva;

   int i,l;
   int valasztott;
   LAP kez[32];
   LAP legfelso;
   int bekovetkezett;
   char szin[20];
   char szam[20];
   int kontra=0;
   char kontras[20];

   /* Initialization */
   on    = 1;
   flags = 0;
   server_size = sizeof server;
   client_size = sizeof client;
   sprintf(server_addr, "%s", argv[1]);
   ip = inet_addr(server_addr);
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = ip;
   server.sin_port        = htons(PORT_NO);
   
   for(i=0;i<32;i++)
   {
      sprintf(kez[i].szin,"ures");
      sprintf(kez[i].szam,"ures");
   }

   /* Creating socket */
   serverSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (serverSocket < 0) {
      error("%s: Socket creation error.\n",argv[0]);
      exit(1);
   }

   /* Setting socket options */
   setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Connecting to the server */
   err = connect(serverSocket, (struct sockaddr *) &server, server_size);
   if (err < 0) {
      error("%s: Cannot connect to the server.\n", argv[0]);
      exit(2);
   }

   /* player number */
   rcvsize = recv( serverSocket, message, BUFSIZE, flags );
   send( serverSocket,"ok",strlen("ok")+2,flags);
   player_number=atoi(message);
   printf("%d. jatekos vagy.\n",player_number );

   /* kezdés */
   for(l=0;l<5;l++)
   {
      recv(serverSocket, message, BUFSIZE, flags);
      strcpy(kez[l].szin,message);
      send(serverSocket, "ok", strlen("ok")+1, flags);

      recv(serverSocket, message, BUFSIZE, flags);
      strcpy(kez[l].szam,message);
      send(serverSocket, "ok", strlen("ok")+1, flags);
   }

   while(1)
   {
      bekovetkezett=0;
      recv(serverSocket, message, BUFSIZE, flags);
      send(serverSocket, "ok",strlen("ok")+1, flags);
      if(strcmp(message,"tejosz")==0)
      {
         printf("==================================\n");
         printf("TE KOVETKEZEL. KEZED TARTALMA:\n");
         for(i=0;i<l;i++)
         {
            printf("%d. %s ",i+1, kez[i].szin);
            printf("%s\n", kez[i].szam);
         }
         /* tobbiek lapjainak szama*/
         printf("===================================\n");
         recv(serverSocket, message, BUFSIZE, flags);
         printf("%s", message);
         send(serverSocket, "ok",strlen("ok")+1, flags);

         /* legfelso lap*/
         recv(serverSocket, message, BUFSIZE, flags);
         strcpy(legfelso.szin,message);
         send(serverSocket, "ok", strlen("ok")+1, flags);

         recv(serverSocket, message, BUFSIZE, flags);
         strcpy(legfelso.szam,message);
         send(serverSocket, "ok", strlen("ok")+1, flags);
         printf("Legfelso lap: %s %s\n",legfelso.szin, legfelso.szam );

         if(strcmp(legfelso.szam,"asz")==0)
         {
            send(serverSocket, "lapjon", strlen("lapjon")+1, flags);
            recv(serverSocket, message, BUFSIZE, flags);

            send(serverSocket, legfelso.szin, strlen(legfelso.szin)+1, flags);
            recv(serverSocket, message, BUFSIZE, flags);
            send(serverSocket, "ASZ", strlen("ASZ")+1, flags);
            recv(serverSocket, message, BUFSIZE, flags);
            bekovetkezett=1;
         }
         if(strcmp(legfelso.szam,"felso")==0)
         {
            recv(serverSocket, message, BUFSIZE, flags);
            strcpy(legfelso.szin,message);
            strcpy(legfelso.szam,"nincs");
            send(serverSocket, "ok", strlen("ok")+1, flags);
            printf("szint kertek: %s\n",legfelso.szin );
         }
         if(strcmp(legfelso.szam,"also")==0)
         {
            recv(serverSocket, message, BUFSIZE, flags);
            strcpy(legfelso.szin,"nincs");
            strcpy(legfelso.szam,message);
            send(serverSocket, "ok", strlen("ok")+1, flags);
            printf("szamot kertek: %s\n",legfelso.szam );
         }
         else if(strcmp(legfelso.szam,"7")==0)
         {
            recv(serverSocket, message, BUFSIZE, flags);
            kontra=atoi(message);
            send(serverSocket, "ok", strlen("ok")+1, flags);
            printf("kontra ertek: %d\n",kontra );
         }

         /* utasitas kerese */
         
         while(bekovetkezett!=1)
         {
            printf(">");
            scanf("%s",&utasitas);
            sprintf(kontras, "%d", kontra);
            if(strcmp(utasitas,"lapot")==0 && strcmp(kontras,"0")==0)
            {
               send(serverSocket, utasitas, strlen(utasitas)+1, flags);
               recv(serverSocket, message, BUFSIZE, flags);
               lapot(l,kez,serverSocket);
               l++;
               bekovetkezett=1;
               break;
            }
            if(strcmp(legfelso.szam,"7")==0 && strcmp(utasitas,"lapotx")==0)
            {
               send(serverSocket, utasitas, strlen(utasitas)+1, flags);
               recv(serverSocket, message, BUFSIZE, flags);
               for(i=0;i<kontra;i++)
               {
                  lapot(l,kez,serverSocket);
                  l++;
               }
               kontra=0;
               bekovetkezett=1;
               break;
            }
            if(strcmp(utasitas,"feladom")==0)
            {
               send(serverSocket, utasitas, strlen(utasitas)+1, flags);
               recv(serverSocket, message, BUFSIZE, flags);
               feladva=1;
               bekovetkezett=1;
               break;
            }
            if(strcmp(utasitas,"lapot")!=0 && strcmp(utasitas,"feladom")!=0 && strcmp(utasitas,"lapotx")!=0)
            {
               valasztott=atoi(utasitas)-1;
               printf("Valasztott sorszam: %d\n", valasztott+1);
               if((strcmp(legfelso.szin,kez[valasztott].szin)==0 || strcmp(legfelso.szam,kez[valasztott].szam)==0) && kontra==0)
               {
                  send(serverSocket, "lapjon", strlen("lapjon")+1, flags);
                  recv(serverSocket, message, BUFSIZE, flags);

                  send(serverSocket, kez[valasztott].szin, strlen(kez[valasztott].szin)+1, flags);
                  recv(serverSocket, message, BUFSIZE, flags);
                  send(serverSocket, kez[valasztott].szam, strlen(kez[valasztott].szam)+1, flags);
                  recv(serverSocket, message, BUFSIZE, flags);

                  if(strcmp(kez[valasztott].szam,"felso")==0)
                  {
                     printf("Adja meg a szint: ");
                     scanf("%s",&szin);
                     strcpy(message,szin);
                     send(serverSocket, message, strlen(message)+1, flags);
                     recv(serverSocket, message, BUFSIZE, flags);
                  }
                  if(strcmp(kez[valasztott].szam,"also")==0)
                  {
                     printf("Adja meg a szamot: ");
                     scanf("%s",&szam);
                     strcpy(message,szam);
                     send(serverSocket, message, strlen(message)+1, flags);
                     recv(serverSocket, message, BUFSIZE, flags);
                  }
                  if(strcmp(kez[valasztott].szam,"7")==0)
                  {
                     send(serverSocket, "2", strlen("2")+1, flags);
                     recv(serverSocket, message, BUFSIZE, flags);
                     kontra=0;
                  }

                  sprintf(kez[valasztott].szin,"ures");
                  sprintf(kez[valasztott].szam,"ures");
                  l--;
                  rendez(kez);
                  bekovetkezett=1;
                  break;
               }
               if(kontra!=0 && strcmp(legfelso.szam,kez[valasztott].szam)==0)
               {
                  send(serverSocket, "lapjon", strlen("lapjon")+1, flags);
                  recv(serverSocket, message, BUFSIZE, flags);

                  send(serverSocket, kez[valasztott].szin, strlen(kez[valasztott].szin)+1, flags);
                  recv(serverSocket, message, BUFSIZE, flags);
                  send(serverSocket, kez[valasztott].szam, strlen(kez[valasztott].szam)+1, flags);
                  recv(serverSocket, message, BUFSIZE, flags);

                  send(serverSocket, "2", strlen("2")+1, flags);
                  recv(serverSocket, message, BUFSIZE, flags);

                  sprintf(kez[valasztott].szin,"ures");
                  sprintf(kez[valasztott].szam,"ures");
                  l--;
                  rendez(kez);
                  bekovetkezett=1;
                  break;
               }
            }
            printf("Nem illeszkedik a felso lapra vagy nincs ilyen parancs.\n");
         }
         if(feladva==1)
            break;
      }
      recv(serverSocket, message, BUFSIZE, flags);
      send(serverSocket, "ok",strlen("ok")+1, flags);
      printf("%s\n", message);
   }
   if(feladva==1)
   {
      recv(serverSocket, message, BUFSIZE, flags);
      send(serverSocket, "ok",strlen("ok")+1, flags);
      printf("%s\n", message);
   }

   /* Closing sockets and quit */
   close(serverSocket);
   exit(0);
} 

