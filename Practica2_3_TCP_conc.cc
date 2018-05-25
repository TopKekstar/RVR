/*
	TCP concurrent server that outputs the time or the date to the connection.
	Made by Francisco Lopez Bleda and Manuel Hernández for the course of Videogame Networks

*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include <cstdio>
#include <iostream>
#include <string.h>

#define BUFFER_SIZE 100
#define MAX_THREADS 10

bool closeS = false;
size_t nThreads;
pthread_mutex_t lock;

class ServerThread
{
public:
	ServerThread(int s):socket(s){};
	virtual ~ServerThread(){}
	void doMessage(){

		while(!closeS)
		{

		char buffer[256];
		int errno = recv(socket, &buffer, 255, 0); 
		buffer[255] = '\n';

		char host [NI_MAXHOST];
		char serv [NI_MAXSERV];

		char send_buff [258];
		if(errno = 0){
			closeS = true;

			if(buffer[0] == 't')
				returnTime(send_buff, BUFFER_SIZE);

			else if(buffer[0] == 'd')
				returnDay(send_buff, BUFFER_SIZE);
			else if(buffer[0] == 'q')
				strcpy(send_buff, "Quitting recognized.");
			else {
				strcpy(send_buff, "Command not recognized.");
			}
		}

  		send(socket, send_buff,strlen(send_buff) , 0);

		}

	std::cout << "Closing thread" << std::endl;

	pthread_mutex_lock(&lock);
		nThreads--;	
	pthread_mutex_unlock(&lock);
	
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
        nThreads = 0;
	pthread_mutex_init(&lock, NULL);
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

	if(rc != 0)
        {

		std::cout << "Error: " << gai_strerror(rc) << std::endl;

		return -1;
	}
	
	size_t sock = 
	socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	bind(sock, (struct sockaddr *)res->ai_addr, res->ai_addrlen);
	
	

   	freeaddrinfo(res);

	std::cout << "Press q to close the server" << std::endl;  

 	bool closeS = false;
	struct sockaddr cliente;
	socklen_t cliente_len;

	while(!closeS)
        {
		
		if(nThreads <= MAX_THREADS)
                {
			char host [NI_MAXHOST];
			char serv [NI_MAXSERV];
			listen(sock, 5);

			int nsock = accept(sock, &cliente, &cliente_len);
			getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST,
     	 		serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);
                        
                        if(nscock < 0){
                                std::cout<< strerror(errno)<< std::endl;
                        }
  			std::cout << "Conexión desde Host: "<< host << std::endl;
  			std::cout << "Client Port: " << nsock << std::endl;
	   		
			pthread_t tid;
	   		pthread_attr_t attr;
	   		
	   		pthread_attr_init(&attr);
	   		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	   		
	   		ServerThread * st = new ServerThread(sock);
	   		
	   		int errcode = pthread_create(&tid, &attr, start_routine, static_cast<void *>(st));
			
			if(errcode != 0){
				char * error;
				perror(error);
				std::cout << "Error while creating thread: "<< error << std::endl;
   			}
			else {
				pthread_mutex_lock(&lock);
				nThreads++;
				pthread_mutex_unlock(&lock);
			}
		}
		else {
		std::cout << nThreads<< " Can not process petition, No threads available"<<std::endl;	
		sleep(2);		
		}
		char a = getchar();
		if(a == 'q') closeS = true;
        }
}
 
