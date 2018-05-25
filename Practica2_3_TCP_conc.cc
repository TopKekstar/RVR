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
