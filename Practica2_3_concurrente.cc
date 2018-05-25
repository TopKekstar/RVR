/*
	UDP concurrent server that outputs the time or the date to the connection.
	Made by Francisco Lopez Bleda and Manuel Hernández for the course of Videogame Networks

*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include<cstdio>
#include <iostream>
#include <string.h>

#define BUFFER_SIZE 100
#define NUM_THREADS 4

bool closeS = false;

class ServerThread
{
public:
	ServerThread(int s):socket(s){};
	virtual ~ServerThread(){}
	void doMessage(){

		while(!closeS)
		{
		char buffer[256];
		struct sockaddr src_addr;
		socklen_t addrlen;

		ssize_t bytes =
		recvfrom(socket, &buffer, 255, 0, &src_addr, &addrlen); 
		buffer[bytes] = '\n';

		char host [NI_MAXHOST];
		char serv [NI_MAXSERV];

 		getnameinfo(&src_addr, addrlen, host, NI_MAXHOST,
      		serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);

  		std::cout << "Conexión desde Host: "<< host << " Puerto: " << socket << std::endl;
 	 	std::cout << "Mensaje("<< bytes <<"): "<< buffer << std::endl;
		
		char send_buff [258];
		
		if(buffer[0] == 't')
			returnTime(send_buff, BUFFER_SIZE);

		else if(buffer[0] == 'd')
			returnDay(send_buff, BUFFER_SIZE);
		else {
			strcpy(send_buff, "Command not recognized.");
		}


  		sendto(socket, send_buff,strlen(send_buff) , 0, &src_addr, addrlen);

		}

	std::cout << "Closing thread" << std::endl;
	
	
	}

private:
	int socket;
	void returnTime(char * buffer, size_t buflen){

		time_t tAct;
		time(&tAct);

		struct tm * timeinfo  = localtime(&tAct);
		strftime(buffer, buflen, "%H:%M:%S\n",  timeinfo);

	}
	void returnDay(char * buffer, size_t buflen){

		time_t tAct;
		time(&tAct);

		struct tm * timeinfo  = localtime(&tAct);
		strftime(buffer, buflen, "It's %A!, %d of %B of %G.\n", timeinfo);

	}


};


extern "C" void *start_routine (void * _st)
{
	ServerThread * st = static_cast<ServerThread * >(_st);
	
	st->doMessage();
	

};

int main (int argc, char ** argv){

	//Variable hints para almacenar los filtros a pasarle.
	//No es puntero para evitar accesos descontrolados a memoria dinámica
	struct addrinfo hints;
	
	//--------------------------------------------------------------------------------------
	//1. Inicialización del socketio
	//--------------------------------------------------------------------------------------
	
	
	//memset nos vale para poner toda una sección de memoria
	//al mismo valor. En este caso, 0. DUDAS: terminal-> man memset
	memset((void*)&hints, '\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET; //Para IPv4 usamos AF_INET
	hints.ai_socktype = SOCK_DGRAM;


	struct addrinfo * res;

	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	if(rc != 0){

		std::cout << "Error: " << gai_strerror(rc) << std::endl;

		return -1;
	}
	
	size_t sock = 
	socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	bind(sock, (struct sockaddr *)res->ai_addr, res->ai_addrlen);

   	freeaddrinfo(res);
   	//--------------------------------------------------------------------------------------
   	//2. Inicializar Pool de THreads
   	//--------------------------------------------------------------------------------------
   	for(int i = 0; i < NUM_THREADS; i++){

   		pthread_t tid;
   		pthread_attr_t attr;
   		
   		pthread_attr_init(&attr);
   		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   		
   		ServerThread * st = new ServerThread(sock);
   		
   		pthread_create(&tid, &attr, start_routine, static_cast<void *>(st));
   	
   	}
   	
	do {
		std::cout << "Press q to close the server" << std::endl;
		char a = getchar();
		if(a == 'q') closeS = true;
	}
	while(!closeS);	

}
