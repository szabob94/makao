/***************************************/
/*              TCP server             */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> 

#define BUFSIZE 1024                      // buffer size
#define PORT_NO 6363                      // port number
#define error(a,b) fprintf(stderr, a, b)  // error 'function'

typedef struct lap
{
	char szin[40];
	char szam[40];
} LAP;

int vanMegLap(int allapot[32])
{
	int j;
	for(j=0;j<32;j++)
	{
		if(allapot[j]==0)
			return 1;
	}
	return 0;
}

void kever(int allapot[32])	/* nincs tesztelve */
{
	int j;
	for(j=0;j<32;j++)
	{
		if (allapot[j]==1)
		{
			allapot[j]=0;
		}
	}
}

void lapot(int allapot[], LAP pakli[], int jatekos)
{
	char message[BUFSIZE+1];
	int veletlen=rand()%32;
	while(allapot[veletlen]!=0)
		veletlen=rand()%32;
	if(allapot[veletlen]==0)
	{
		if(jatekos==4)
			allapot[veletlen]=2;
		if(jatekos==5)
			allapot[veletlen]=3;
		if(jatekos==6)
			allapot[veletlen]=4;
		if(jatekos==7)
			allapot[veletlen]=5;
		printf("%d. jatekos kapta: ",jatekos-3);
		send(jatekos,pakli[veletlen].szin,strlen(pakli[veletlen].szin)+1,0);
		printf("%s ",pakli[veletlen].szin );
		recv(jatekos,message,BUFSIZE,0);
		send(jatekos,pakli[veletlen].szam,strlen(pakli[veletlen].szam)+1,0);
		printf("%s\n",pakli[veletlen].szam );
		recv(jatekos,message,BUFSIZE,0);
	}
}

int kezben(int player, int allapot[])
{
	int j;
	int darab=0;
	for(j=0;j<32;j++)
		if(allapot[j]==player-2)
			darab++;
	return darab;
}

int fels(int allapot[])
{
	int j;
	for(j=0;j<32;j++)
		if(allapot[j]==6)
			return j;
}

void legfelso(int jatekos, int felso, LAP pakli[])
{
	char message[BUFSIZE+1];
	send(jatekos,pakli[felso].szin,strlen(pakli[felso].szin)+1,0);
	recv(jatekos,message,BUFSIZE,0);
	send(jatekos,pakli[felso].szam,strlen(pakli[felso].szam)+1,0);
	recv(jatekos,message,BUFSIZE,0);
	printf("Legfelso lap: %s %s\n",pakli[felso].szin, pakli[felso].szam );
}

void allapot_valt(LAP erkezett,LAP pakli[], int allapot[])
{
	int j;
	for(j=0;j<32;j++)
		if(allapot[j]==6)
			allapot[j]=1;
	for(j=0;j<32;j++)
	{
		if(strcmp(pakli[j].szin,erkezett.szin)==0 && strcmp(pakli[j].szam,erkezett.szam)==0)
		{
			printf("%s %s %d->",pakli[j].szin, pakli[j].szam, allapot[j] );
			allapot[j]=6;
			printf("%d\n",allapot[j] );
			break;
		}
		if(strcmp(pakli[j].szin,erkezett.szin)==0 && strcmp(erkezett.szam,"ASZ")==0 && strcmp(pakli[j].szam,"asz")==0)
		{
			printf("%s %s %d->",pakli[j].szin, pakli[j].szam, allapot[j] );
			allapot[j]=6;
			printf("%d\n",allapot[j] );
			break;
		}
	}
}

int egyel(int el[])
{
	int j;
	int darab=0;
	for(j=0;j<4;j++)
		if(el[j]==1)
			darab++;
	return darab;
}

void elfogytak(int el, int player)
{
	char message[BUFSIZE+1];
	if(el==1)
	{
		send(player,"Gratulalok, te nyertel",strlen("Gratulalok, te nyertel")+1,0);
		recv(player,message,BUFSIZE,0);
	}
	if(el==0)
	{
		send(player,"Sajnos vesztettel",strlen("Sajnos vesztettel")+1,0);
		recv(player,message,BUFSIZE,0);
	}

}

void eredmeny(int player, int kez)
{
	char message[BUFSIZE+1];
	if(kez==0)
	{
		send(player,"Gratulalok, te nyertel",strlen("Gratulalok, te nyertel")+1,0);
		recv(player,message,BUFSIZE,0);
	}
	else
	{
		send(player,"Sajnos vesztettel",strlen("Sajnos vesztettel")+1,0);
		recv(player,message,BUFSIZE,0);
	}
}

void korvege(int player)
{
	char message[BUFSIZE+1];
	send(player,"kor vege",strlen("kor vege")+1,0);
	recv(player,message,BUFSIZE,0);
}

int main(int argc, char *argv[] ){ 	// arg count, arg vector 

   srand(time(NULL));
   /* Declarations */
   int serverSocket;	        	           		// socket endpt
   int flags;                      	// rcv flags
   struct sockaddr_in server;      	// socket name (addr) of server
   int server_size;                	// length of the socket addr. server
   struct sockaddr_in client_a;	     	// socket name of client_a
   int client_a_meret;                	// length of the socket addr. client
   struct sockaddr_in client_b;	     	// socket name of client_a
   int client_b_meret;                	// length of the socket addr. client
   struct sockaddr_in client_c;	     	// socket name of client_a
   int client_c_meret;                	// length of the socket addr. client
   struct sockaddr_in client_d;	     	// socket name of client_a
   int client_d_meret;                	// length of the socket addr. client
   int bytes;		           					// length of buffer 
   int rcvsize;                    	// received bytes
   int trnmsize;                   	// transmitted bytes
   int err;                        	// error code
   char on;                        	// 
   char message[BUFSIZE+1];	     		// datagram dat buffer area 
   int i;
   int veletlen;
   int kliens;
   char szin[20];
   char szam[20];

   int players;  
   int a_jatekos, b_jatekos, c_jatekos, d_jatekos;
   int feladva_a, feladva_b, feladva_c, feladva_d;
   int felso;
   LAP pakli[32];
   LAP erkezett;
   int allapot[33];	//0 - pakliban, 1 - leteve, 2 - player1, 3 - player2, 4 - player3, 5 - player4, 6 - legfelso
   int el[4];
   char tmp[10];
   int kontra=0;

   for(i=0;i<33;i++)
   {
   		allapot[i]=0;
   }

   sprintf(pakli[0].szin,"piros"); sprintf(pakli[0].szam,"7");
   sprintf(pakli[1].szin,"piros"); sprintf(pakli[1].szam,"8");
   sprintf(pakli[2].szin,"piros"); sprintf(pakli[2].szam,"9");
   sprintf(pakli[3].szin,"piros"); sprintf(pakli[3].szam,"10");
   sprintf(pakli[4].szin,"piros"); sprintf(pakli[4].szam,"also");
   sprintf(pakli[5].szin,"piros"); sprintf(pakli[5].szam,"felso");
   sprintf(pakli[6].szin,"piros"); sprintf(pakli[6].szam,"kiraly");
   sprintf(pakli[7].szin,"piros"); sprintf(pakli[7].szam,"asz");
   sprintf(pakli[8].szin,"zold"); sprintf(pakli[8].szam,"7");
   sprintf(pakli[9].szin,"zold"); sprintf(pakli[9].szam,"8");
   sprintf(pakli[10].szin,"zold"); sprintf(pakli[10].szam,"9");
   sprintf(pakli[11].szin,"zold"); sprintf(pakli[11].szam,"10");
   sprintf(pakli[12].szin,"zold"); sprintf(pakli[12].szam,"also");
   sprintf(pakli[13].szin,"zold"); sprintf(pakli[13].szam,"felso");
   sprintf(pakli[14].szin,"zold"); sprintf(pakli[14].szam,"kiraly");
   sprintf(pakli[15].szin,"zold"); sprintf(pakli[15].szam,"asz");
   sprintf(pakli[16].szin,"tok"); sprintf(pakli[16].szam,"7");
   sprintf(pakli[17].szin,"tok"); sprintf(pakli[17].szam,"8");
   sprintf(pakli[18].szin,"tok"); sprintf(pakli[18].szam,"9");
   sprintf(pakli[19].szin,"tok"); sprintf(pakli[19].szam,"10");
   sprintf(pakli[20].szin,"tok"); sprintf(pakli[20].szam,"also");
   sprintf(pakli[21].szin,"tok"); sprintf(pakli[21].szam,"felso");
   sprintf(pakli[22].szin,"tok"); sprintf(pakli[22].szam,"kiraly");
   sprintf(pakli[23].szin,"tok"); sprintf(pakli[23].szam,"asz");
   sprintf(pakli[24].szin,"makk"); sprintf(pakli[24].szam,"7");
   sprintf(pakli[25].szin,"makk"); sprintf(pakli[25].szam,"8");
   sprintf(pakli[26].szin,"makk"); sprintf(pakli[26].szam,"9");
   sprintf(pakli[27].szin,"makk"); sprintf(pakli[27].szam,"10");
   sprintf(pakli[28].szin,"makk"); sprintf(pakli[28].szam,"also");
   sprintf(pakli[29].szin,"makk"); sprintf(pakli[29].szam,"felso");
   sprintf(pakli[30].szin,"makk"); sprintf(pakli[30].szam,"kiraly");
   sprintf(pakli[31].szin,"makk"); sprintf(pakli[31].szam,"asz");

   /* Initialization */
   on                     = 1;
   flags                  = 0;
   bytes                  = BUFSIZE;
   server_size            = sizeof server;
   client_a_meret            = sizeof client_a;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port        = htons(PORT_NO);
   players=atoi(argv[1]);
   feladva_a=0; feladva_b=0; feladva_c=0; feladva_d=0;

   /* player number check*/
   if(players<2 || players >4)
   {
   		error("%d: Jatekosok szama 2 es 4 kozott kell legyen \n",players);
   		exit(1);
   }

   /* Creating socket */
   serverSocket = socket(AF_INET, SOCK_STREAM, 0 );
   if (serverSocket < 0) {
      error("%s: Socket creation error\n",argv[0]);
      exit(1);
      }

   /* Setting socket options */
   setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Binding socket */
   err = bind(serverSocket, (struct sockaddr *) &server, server_size);
   if (err < 0) {
      error("%s: Cannot bind to the socket\n",argv[0]);
      exit(2);
      }

   /* Listening to player1*/
   err = listen(serverSocket, 10);
   if (err < 0) {
      error("%s: Cannot listen to the socket\n",argv[0]);
      exit(3);
      }

   /* Accepting connection request */
   a_jatekos = accept(serverSocket, (struct sockaddr *) &client_a, &client_a_meret);
   if (a_jatekos < 0) {
      error("%s: Cannot accept on socketa\n",argv[0]);
      exit(4);
      }

   err = listen(serverSocket, 10);
   if (err < 0) {
      error("%s: Cannot listen to the socket\n",argv[0]);
      exit(3);
      }

   b_jatekos = accept(serverSocket, (struct sockaddr *) &client_b, &client_b_meret);
   if (b_jatekos < 0) {
      error("%s: Cannot accept on socketb\n",argv[0]);
      exit(4);
      }

   if(players>2){
   	err = listen(serverSocket, 10);
   	if (err < 0) {
      error("%s: Cannot listen to the socket\n",argv[0]);
      exit(3);
      }

   	c_jatekos = accept(serverSocket, (struct sockaddr *) &client_c, &client_c_meret);
   	if (c_jatekos < 0) {
      error("%s: Cannot accept on socketc\n",argv[0]);
      exit(4);
      }
   }

   if (players>3)
   {
   	err = listen(serverSocket, 10);
   if (err < 0) {
      error("%s: Cannot listen to the socket\n",argv[0]);
      exit(3);
      }

   d_jatekos = accept(serverSocket, (struct sockaddr *) &client_d, &client_d_meret);
   if (d_jatekos < 0) {
      error("%s: Cannot accept on socketd\n",argv[0]);
      exit(4);
      }
   }

   /* játékosszámok kiosztása */
   send(a_jatekos,"1",strlen("1")+1,flags);
   recv(a_jatekos,message,BUFSIZE,flags);
   el[0]=1;

   send(b_jatekos,"2",strlen("2")+1,flags);
   recv(b_jatekos,message,BUFSIZE,flags);
   el[1]=1;

   el[2]=0;
   el[3]=0;

   if(players>2){
	   send(c_jatekos,"3",strlen("3")+1,flags);
	   recv(c_jatekos,message,BUFSIZE,flags);
	   el[2]=1;
	}

	if(players>3){
	   send(d_jatekos,"4",strlen("4")+1,flags);
	   recv(d_jatekos,message,BUFSIZE,flags);
	   el[3]=1;
	}

	/* kezdes */
	i=0;
	kliens=0;
	while(i!=10)
	{
		veletlen=rand()%32;
		if(allapot[veletlen]==0)
		{
			i++;
			printf("%d\n",veletlen );
			if(kliens==0)
			{
				allapot[veletlen]=2;
		   		send(a_jatekos,pakli[veletlen].szin,strlen(pakli[veletlen].szin)+1,flags);
		   		recv(a_jatekos,message,BUFSIZE,flags);
		   		printf("%s\n",message );
			   	send(a_jatekos,pakli[veletlen].szam,strlen(pakli[veletlen].szam)+1,flags);
			   	recv(a_jatekos,message,BUFSIZE,flags);
			   	printf("%s\n",message );
			   	printf("kliens: %d\n",kliens );
			   	kliens++;
			   	continue;
			}
			if(kliens==1)
			{
				allapot[veletlen]=3;
		   		send(b_jatekos,pakli[veletlen].szin,strlen(pakli[veletlen].szin) + 1,flags);
		   		recv(b_jatekos,message,BUFSIZE,flags);
		   		printf("%s\n",message );
			   	send(b_jatekos,pakli[veletlen].szam,strlen(pakli[veletlen].szam) + 1,flags);
			   	recv(b_jatekos,message,BUFSIZE,flags);
			   	printf("%s\n",message );
			   	printf("kliens: %d\n",kliens );
			   	kliens=0;
			}
		}
	}
	if(players>=3)
	{
		kliens=2;
		i=0;
		while(i!=5)
		{
			veletlen=rand()%32;
				if(allapot[veletlen]==0)
				{
					i++;
					printf("%d\n",veletlen );
					allapot[veletlen]=4;
		   			send(c_jatekos,pakli[veletlen].szin,strlen(pakli[veletlen].szin)+1,flags);
		   			recv(c_jatekos,message,BUFSIZE,flags);
		   			printf("%s\n",message );
			   		send(c_jatekos,pakli[veletlen].szam,strlen(pakli[veletlen].szam)+1,flags);
			   		recv(c_jatekos,message,BUFSIZE,flags);
			   		printf("%s\n",message );
			   		printf("kliens: %d\n",kliens );
				}
		}
	}
	if(players==4)
	{
		kliens=3;
		i=0;
		while(i!=5)
		{
			veletlen=rand()%32;
				if(allapot[veletlen]==0)
				{
					i++;
					printf("%d\n",veletlen );
					allapot[veletlen]=5;
		   			send(d_jatekos,pakli[veletlen].szin,strlen(pakli[veletlen].szin)+1,flags);
		   			recv(d_jatekos,message,BUFSIZE,flags);
		   			printf("%s\n",message );
			   		send(d_jatekos,pakli[veletlen].szam,strlen(pakli[veletlen].szam)+1,flags);
			   		recv(d_jatekos,message,BUFSIZE,flags);
			   		printf("%s\n",message );
			   		printf("kliens: %d\n",kliens );
				}
		}
	}
	felso=rand()%32;
	while(allapot[felso]!=0 || strcmp(pakli[felso].szam,"asz")==0 || strcmp(pakli[felso].szam,"also")==0 || strcmp(pakli[felso].szam,"felso")==0 || strcmp(pakli[felso].szam,"7")==0)
		felso=rand()%32;
	allapot[felso]=6;

	/* meccs */
	while(1)
	{
		/* player1 kore */
		if(feladva_a==0)
		{
			send(a_jatekos,"tejosz",strlen("tejosz")+1,flags);
			recv(a_jatekos,message,BUFSIZE,flags);
			if(players>=2)
			{
				strcpy(message,"2. jatekos kezeben:");
				sprintf(tmp, "%d", kezben(b_jatekos,allapot));
				strcat(message, tmp);
				strcat(message,"\n");
			}
			if(players>=3)
			{
				strcat(message,"3. jatekos kezeben:");
				sprintf(tmp, "%d", kezben(c_jatekos,allapot));
				strcat(message, tmp);
				strcat(message,"\n");
			}
			if(players==4)
			{
				strcat(message,"4. jatekos kezeben:");
				sprintf(tmp, "%d", kezben(d_jatekos,allapot));
				strcat(message, tmp);
				strcat(message,"\n");
			}
			send(a_jatekos,message,strlen(message)+1,flags);
			recv(a_jatekos,message,BUFSIZE,flags);

			if(strcmp(pakli[fels(allapot)].szam,"asz")==0 && strcmp(erkezett.szam,"ASZ")==0)
			{
				send(a_jatekos,erkezett.szin,strlen(erkezett.szin)+1,0);
				recv(a_jatekos,message,BUFSIZE,0);
				send(a_jatekos,erkezett.szam,strlen(erkezett.szam)+1,0);
				recv(a_jatekos,message,BUFSIZE,0);
				printf("Legfelso lap: %s %s\n",pakli[felso].szin, pakli[felso].szam );
			}
			else
				legfelso(a_jatekos,fels(allapot),pakli);
			if(strcmp(pakli[fels(allapot)].szam,"felso")==0)
			{
				strcpy(message,szin);
				send(a_jatekos,message,strlen(message)+1,flags);
				printf("%d-nek kuldtem szint: %s\n",a_jatekos-3, message );
				recv(a_jatekos,message,BUFSIZE,flags);
			}
			if(strcmp(pakli[fels(allapot)].szam,"also")==0)
			{
				strcpy(message,szam);
				send(a_jatekos,message,strlen(message)+1,flags);
				printf("%d-nek kuldtem szamot: %s\n",a_jatekos-3, message );
				recv(a_jatekos,message,BUFSIZE,flags);
			}
			if(strcmp(pakli[fels(allapot)].szam,"7")==0)
			{
				sprintf(message, "%d", kontra);
				send(a_jatekos,message,strlen(message)+1,flags);
				printf("%d-nek kuldtem kontrat: %s\n",a_jatekos-3, message );
				recv(a_jatekos,message,BUFSIZE,flags);
			}

			recv(a_jatekos,message,BUFSIZE,flags);
			send(a_jatekos,"ok",strlen("ok")+1,flags);
			printf("%s\n", message);
			if(strcmp(message,"lapot")==0)
				if(vanMegLap(allapot)==1)
					lapot(allapot,pakli,a_jatekos);
				else
				{
					kever(allapot);
					lapot(allapot,pakli,a_jatekos);
				}
			if(strcmp(message,"lapotx")==0)
			{
				for(i=0;i<kontra;i++)
				{
					if(vanMegLap(allapot)==1)
						lapot(allapot,pakli,a_jatekos);
					else
					{
						kever(allapot);
						lapot(allapot,pakli,a_jatekos);
					}
				}
				kontra=0;
			}
			if(strcmp(message,"feladom")==0)
			{
				printf("%d. jatekos feladta\n",a_jatekos-3 );
				el[0]=0;
				feladva_a=1;
			}
			if(strcmp(message,"lapjon")==0)
			{
				recv(a_jatekos,message,BUFSIZE,flags);
				send(a_jatekos,"ok",strlen("ok")+1,flags);
				strcpy(erkezett.szin,message);
				recv(a_jatekos,message,BUFSIZE,flags);
				send(a_jatekos,"ok",strlen("ok")+1,flags);
				strcpy(erkezett.szam,message);
				allapot_valt(erkezett, pakli, allapot);
				if(strcmp(erkezett.szam,"felso")==0)
				{
					recv(a_jatekos,message,BUFSIZE,flags);
					send(a_jatekos,"ok",strlen("ok")+1,flags);
					strcpy(szin,message);
					printf("szin erkezett: %s\n", szin);
				}
				if(strcmp(erkezett.szam,"also")==0)
				{
					recv(a_jatekos,message,BUFSIZE,flags);
					send(a_jatekos,"ok",strlen("ok")+1,flags);
					strcpy(szam,message);
					printf("szam erkezett: %s\n", szam);
				}
				if(strcmp(erkezett.szam,"7")==0)
				{
					recv(a_jatekos,message,BUFSIZE,flags);
					send(a_jatekos,"ok",strlen("ok")+1,flags);
					kontra+=atoi(message);
					printf("szam erkezett: %s\n", message);
				}
			}
		}

		/* player 2 kore*/
		if(feladva_b==0)
		{
			send(b_jatekos,"tejosz",strlen("tejosz")+1,flags);
			recv(b_jatekos,message,BUFSIZE,flags);
			if(players>=2)
			{
				strcpy(message,"1. jatekos kezeben:");
				sprintf(tmp, "%d", kezben(a_jatekos,allapot));
				strcat(message, tmp);
				strcat(message,"\n");
			}
			if(players>=3)
			{
				strcat(message,"3. jatekos kezeben:");
				sprintf(tmp, "%d", kezben(c_jatekos,allapot));
				strcat(message, tmp);
				strcat(message,"\n");
			}
			if(players==4)
			{
				strcat(message,"4. jatekos kezeben:");
				sprintf(tmp, "%d", kezben(d_jatekos,allapot));
				strcat(message, tmp);
				strcat(message,"\n");
			}
			send(b_jatekos,message,strlen(message)+1,flags);
			recv(b_jatekos,message,BUFSIZE,flags);

			if(strcmp(pakli[fels(allapot)].szam,"asz")==0 && strcmp(erkezett.szam,"ASZ")==0)
			{
				send(b_jatekos,erkezett.szin,strlen(erkezett.szin)+1,0);
				recv(b_jatekos,message,BUFSIZE,0);
				send(b_jatekos,erkezett.szam,strlen(erkezett.szam)+1,0);
				recv(b_jatekos,message,BUFSIZE,0);
				printf("Legfelso lap: %s %s\n",pakli[felso].szin, pakli[felso].szam );
			}
			else
				legfelso(b_jatekos,fels(allapot),pakli);
			if(strcmp(pakli[fels(allapot)].szam,"felso")==0)
			{
				strcpy(message,szin);
				send(b_jatekos,message,strlen(message)+1,flags);
				printf("%d-nek kuldtem szint: %s\n",b_jatekos-3, message );
				recv(b_jatekos,message,BUFSIZE,flags);
			}
			if(strcmp(pakli[fels(allapot)].szam,"also")==0)
			{
				strcpy(message,szam);
				send(b_jatekos,message,strlen(message)+1,flags);
				printf("%d-nek kuldtem szamot: %s\n",b_jatekos-3, message );
				recv(b_jatekos,message,BUFSIZE,flags);
			}
			if(strcmp(pakli[fels(allapot)].szam,"7")==0)
			{
				sprintf(message, "%d", kontra);
				send(b_jatekos,message,strlen(message)+1,flags);
				printf("%d-nek kuldtem kontrat: %s\n",b_jatekos-3, message );
				recv(b_jatekos,message,BUFSIZE,flags);
			}

			recv(b_jatekos,message,BUFSIZE,flags);
			send(b_jatekos,"ok",strlen("ok")+1,flags);
			printf("%s\n", message);
			if(strcmp(message,"lapot")==0)
				if(vanMegLap(allapot)==1)
					lapot(allapot,pakli,b_jatekos);
				else
				{
					kever(allapot);
					lapot(allapot,pakli,b_jatekos);
				}
			if(strcmp(message,"lapotx")==0)
			{
				for(i=0;i<kontra;i++)
				{
					if(vanMegLap(allapot)==1)
						lapot(allapot,pakli,b_jatekos);
					else
					{
						kever(allapot);
						lapot(allapot,pakli,b_jatekos);
					}
				}
				kontra=0;
			}
			if(strcmp(message,"feladom")==0)
			{
				printf("%d. jatekos feladta\n",b_jatekos-3 );
				el[1]=0;
				feladva_b=1;
			}
			if(strcmp(message,"lapjon")==0)
			{
				recv(b_jatekos,message,BUFSIZE,flags);
				send(b_jatekos,"ok",strlen("ok")+1,flags);
				strcpy(erkezett.szin,message);
				recv(b_jatekos,message,BUFSIZE,flags);
				send(b_jatekos,"ok",strlen("ok")+1,flags);
				strcpy(erkezett.szam,message);
				printf("erkezett: %s %s",erkezett.szin, erkezett.szam);
				allapot_valt(erkezett, pakli, allapot);
				if(strcmp(erkezett.szam,"felso")==0)
				{
					recv(b_jatekos,message,BUFSIZE,flags);
					send(b_jatekos,"ok",strlen("ok")+1,flags);
					strcpy(szin,message);
					printf("szin erkezett: %s\n", szin);
				}
				if(strcmp(erkezett.szam,"also")==0)
				{
					recv(b_jatekos,message,BUFSIZE,flags);
					send(b_jatekos,"ok",strlen("ok")+1,flags);
					strcpy(szam,message);
					printf("szam erkezett: %s\n", szam);
				}
				if(strcmp(erkezett.szam,"7")==0)
				{
					recv(b_jatekos,message,BUFSIZE,flags);
					send(b_jatekos,"ok",strlen("ok")+1,flags);
					kontra+=atoi(message);
					printf("szam erkezett: %s\n", message);
				}
			}
		}

		if(players>=3)
		{
			/* player3 kore */
			if(feladva_c==0)
			{
				send(c_jatekos,"tejosz",strlen("tejosz")+1,flags);
				recv(c_jatekos,message,BUFSIZE,flags);
				if(players>=2)
				{
					strcpy(message,"1. jatekos kezeben:");
					sprintf(tmp, "%d", kezben(a_jatekos,allapot));
					strcat(message, tmp);
					strcat(message,"\n");
				}
				if(players>=3)
				{
					strcat(message,"2. jatekos kezeben:");
					sprintf(tmp, "%d", kezben(b_jatekos,allapot));
					strcat(message, tmp);
					strcat(message,"\n");
				}
				if(players==4)
				{
					strcat(message,"4. jatekos kezeben:");
					sprintf(tmp, "%d", kezben(d_jatekos,allapot));
					strcat(message, tmp);
					strcat(message,"\n");
				}
				send(c_jatekos,message,strlen(message)+1,flags);
				recv(c_jatekos,message,BUFSIZE,flags);

				if(strcmp(pakli[fels(allapot)].szam,"asz")==0 && strcmp(erkezett.szam,"ASZ")==0)
				{
					send(c_jatekos,erkezett.szin,strlen(erkezett.szin)+1,0);
					recv(c_jatekos,message,BUFSIZE,0);
					send(c_jatekos,erkezett.szam,strlen(erkezett.szam)+1,0);
					recv(c_jatekos,message,BUFSIZE,0);
					printf("Legfelso lap: %s %s\n",pakli[felso].szin, pakli[felso].szam );
				}
				else
					legfelso(c_jatekos,fels(allapot),pakli);
				if(strcmp(pakli[fels(allapot)].szam,"felso")==0)
				{
					strcpy(message,szin);
					send(c_jatekos,message,strlen(message)+1,flags);
					printf("%d-nek kuldtem szint: %s\n",c_jatekos-3, message );
					recv(c_jatekos,message,BUFSIZE,flags);
				}
				if(strcmp(pakli[fels(allapot)].szam,"also")==0)
				{
					strcpy(message,szam);
					send(c_jatekos,message,strlen(message)+1,flags);
					printf("%d-nek kuldtem szamot: %s\n",c_jatekos-3, message );
					recv(c_jatekos,message,BUFSIZE,flags);
				}
				if(strcmp(pakli[fels(allapot)].szam,"7")==0)
				{
					sprintf(message, "%d", kontra);
					send(c_jatekos,message,strlen(message)+1,flags);
					printf("%d-nek kuldtem kontrat: %s\n",c_jatekos-3, message );
					recv(c_jatekos,message,BUFSIZE,flags);
				}

				recv(c_jatekos,message,BUFSIZE,flags);
				send(c_jatekos,"ok",strlen("ok")+1,flags);
				printf("%s\n", message);
				if(strcmp(message,"lapot")==0)
					if(vanMegLap(allapot)==1)
						lapot(allapot,pakli,c_jatekos);
					else
					{
						kever(allapot);
						lapot(allapot,pakli,c_jatekos);
					}
				if(strcmp(message,"lapotx")==0)
				{
					for(i=0;i<kontra;i++)
					{
						if(vanMegLap(allapot)==1)
							lapot(allapot,pakli,c_jatekos);
						else
						{
							kever(allapot);
							lapot(allapot,pakli,c_jatekos);
						}
					}
					kontra=0;
				}
				if(strcmp(message,"feladom")==0)
				{
					printf("%d. jatekos feladta\n",c_jatekos-3 );
					el[2]=0;
					feladva_c=1;
				}
				if(strcmp(message,"lapjon")==0)
				{
					recv(c_jatekos,message,BUFSIZE,flags);
					send(c_jatekos,"ok",strlen("ok")+1,flags);
					strcpy(erkezett.szin,message);
					recv(c_jatekos,message,BUFSIZE,flags);
					send(c_jatekos,"ok",strlen("ok")+1,flags);
					strcpy(erkezett.szam,message);
					printf("erkezett: %s %s",erkezett.szin, erkezett.szam);
					allapot_valt(erkezett, pakli, allapot);
					if(strcmp(erkezett.szam,"felso")==0)
					{
						recv(c_jatekos,message,BUFSIZE,flags);
						send(c_jatekos,"ok",strlen("ok")+1,flags);
						strcpy(szin,message);
						printf("szin erkezett: %s\n", szin);
					}
					if(strcmp(erkezett.szam,"also")==0)
					{
						recv(c_jatekos,message,BUFSIZE,flags);
						send(c_jatekos,"ok",strlen("ok")+1,flags);
						strcpy(szam,message);
						printf("szam erkezett: %s\n", szam);
					}
					if(strcmp(erkezett.szam,"7")==0)
					{
						recv(c_jatekos,message,BUFSIZE,flags);
						send(c_jatekos,"ok",strlen("ok")+1,flags);
						kontra+=atoi(message);
						printf("szam erkezett: %s\n", message);
					}
				}
			}
		}
		if(players==4)
		{
			/* player4 kore */
			if(feladva_d==0)
			{
				send(d_jatekos,"tejosz",strlen("tejosz")+1,flags);
				recv(d_jatekos,message,BUFSIZE,flags);
				if(players>=2)
				{
					strcpy(message,"1. jatekos kezeben:");
					sprintf(tmp, "%d", kezben(a_jatekos,allapot));
					strcat(message, tmp);
					strcat(message,"\n");
				}
				if(players>=3)
				{
					strcat(message,"2. jatekos kezeben:");
					sprintf(tmp, "%d", kezben(b_jatekos,allapot));
					strcat(message, tmp);
					strcat(message,"\n");
				}
				if(players==4)
				{
					strcat(message,"4. jatekos kezeben:");
					sprintf(tmp, "%d", kezben(c_jatekos,allapot));
					strcat(message, tmp);
					strcat(message,"\n");
				}
				send(d_jatekos,message,strlen(message)+1,flags);
				recv(d_jatekos,message,BUFSIZE,flags);

				if(strcmp(pakli[fels(allapot)].szam,"asz")==0 && strcmp(erkezett.szam,"ASZ")==0)
				{
					send(d_jatekos,erkezett.szin,strlen(erkezett.szin)+1,0);
					recv(d_jatekos,message,BUFSIZE,0);
					send(d_jatekos,erkezett.szam,strlen(erkezett.szam)+1,0);
					recv(d_jatekos,message,BUFSIZE,0);
					printf("Legfelso lap: %s %s\n",pakli[felso].szin, pakli[felso].szam );
				}
				else
					legfelso(d_jatekos,fels(allapot),pakli);
				if(strcmp(pakli[fels(allapot)].szam,"felso")==0)
				{
					strcpy(message,szin);
					send(d_jatekos,message,strlen(message)+1,flags);
					printf("%d-nek kuldtem szint: %s\n",d_jatekos-3, message );
					recv(d_jatekos,message,BUFSIZE,flags);
				}
				if(strcmp(pakli[fels(allapot)].szam,"also")==0)
				{
					strcpy(message,szam);
					send(d_jatekos,message,strlen(message)+1,flags);
					printf("%d-nek kuldtem szamot: %s\n",d_jatekos-3, message );
					recv(d_jatekos,message,BUFSIZE,flags);
				}
				if(strcmp(pakli[fels(allapot)].szam,"7")==0)
				{
					sprintf(message, "%d", kontra);
					send(d_jatekos,message,strlen(message)+1,flags);
					printf("%d-nek kuldtem kontrat: %s\n",d_jatekos-3, message );
					recv(d_jatekos,message,BUFSIZE,flags);
				}

				recv(d_jatekos,message,BUFSIZE,flags);
				send(d_jatekos,"ok",strlen("ok")+1,flags);
				printf("%s\n", message);
				if(strcmp(message,"lapot")==0)
					if(vanMegLap(allapot)==1)
						lapot(allapot,pakli,d_jatekos);
					else
					{
						kever(allapot);
						lapot(allapot,pakli,d_jatekos);
					}
				if(strcmp(message,"lapotx")==0)
				{
					for(i=0;i<kontra;i++)
					{
						if(vanMegLap(allapot)==1)
							lapot(allapot,pakli,d_jatekos);
						else
						{
							kever(allapot);
							lapot(allapot,pakli,d_jatekos);
						}
					}
					kontra=0;
				}
				if(strcmp(message,"feladom")==0)
				{
					printf("%d. jatekos feladta\n",d_jatekos-3 );
					el[3]=0;
					feladva_d=1;
				}
				if(strcmp(message,"lapjon")==0)
				{
					recv(d_jatekos,message,BUFSIZE,flags);
					send(d_jatekos,"ok",strlen("ok")+1,flags);
					strcpy(erkezett.szin,message);
					recv(d_jatekos,message,BUFSIZE,flags);
					send(d_jatekos,"ok",strlen("ok")+1,flags);
					strcpy(erkezett.szam,message);
					printf("erkezett: %s %s",erkezett.szin, erkezett.szam);
					allapot_valt(erkezett, pakli, allapot);
					if(strcmp(erkezett.szam,"felso")==0)
					{
						recv(d_jatekos,message,BUFSIZE,flags);
						send(d_jatekos,"ok",strlen("ok")+1,flags);
						strcpy(szin,message);
						printf("szin erkezett: %s\n", szin);
					}
					if(strcmp(erkezett.szam,"also")==0)
					{
						recv(d_jatekos,message,BUFSIZE,flags);
						send(d_jatekos,"ok",strlen("ok")+1,flags);
						strcpy(szam,message);
						printf("szam erkezett: %s\n", szam);
					}
					if(strcmp(erkezett.szam,"7")==0)
					{
						recv(d_jatekos,message,BUFSIZE,flags);
						send(d_jatekos,"ok",strlen("ok")+1,flags);
						kontra+=atoi(message);
						printf("szam erkezett: %s\n", message);
					}
				}
			}
		}

		/* vege van?*/

		if(el[0]==1)
			korvege(a_jatekos);
		if(el[1]==1)
			korvege(b_jatekos);
		if(players>2 && el[2]==1)
			korvege(c_jatekos);
		if(players==4 && el[3]==1)
			korvege(d_jatekos);

		if(egyel(el)==1)
		{
			elfogytak(el[0],a_jatekos);
			elfogytak(el[1],b_jatekos);
			if(players>2)
				elfogytak(el[2],c_jatekos);
			if(players==4)
				elfogytak(el[3],d_jatekos);
		break;
		}

		if(players==2)
		{
			if(kezben(a_jatekos,allapot)==0 || kezben(b_jatekos,allapot)==0)
			{
				eredmeny(a_jatekos,kezben(a_jatekos,allapot));
				eredmeny(b_jatekos,kezben(b_jatekos,allapot));
				break;
			}
		}
		if(players==3)
		{
			if(kezben(a_jatekos,allapot)==0 || kezben(b_jatekos,allapot)==0 || kezben(c_jatekos,allapot)==0)
			{
				eredmeny(a_jatekos,kezben(a_jatekos,allapot));
				eredmeny(b_jatekos,kezben(b_jatekos,allapot));
				eredmeny(c_jatekos,kezben(c_jatekos,allapot));
				break;
			}
		}
		if(players==4)
		{
			if(kezben(a_jatekos,allapot)==0 || kezben(b_jatekos,allapot)==0 || kezben(c_jatekos,allapot)==0 || kezben(d_jatekos,allapot)==0)
			{
				eredmeny(a_jatekos,kezben(a_jatekos,allapot));
				eredmeny(b_jatekos,kezben(b_jatekos,allapot));
				eredmeny(c_jatekos,kezben(c_jatekos,allapot));
				eredmeny(d_jatekos,kezben(d_jatekos,allapot));
				break;
			}
		}

	}

   /* Closing sockets and quit */
   close(a_jatekos);
   close(b_jatekos);
   if(players>2)
   	close(c_jatekos);
   if(players==4)
   	close(d_jatekos);
   close(serverSocket);
   exit(0);
} 

