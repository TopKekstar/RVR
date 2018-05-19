#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include <iostream>
#include <string.h>

#define BUFFER_SIZE 100
#define MAX_NUM_THREADS 10
typedef struct {
	struct sockaddr cliente,
	socklen_t cliente_len;
}threadParams;

void returnDay(char * buf, size_t buflen);
void returnTime(char * buf, size_t buflen);
void** manageConection(void ** params);
//El argumento 1 es la dirección ip del servidor
//El argumento 2 es el puerto al que hacer bind
int main (int argc, char ** argv){


	//Variable hints para almacenar los filtros a pasarle.
	//No es puntero para evitar accesos descontrolados a memoria dinámica
	struct addrinfo hints;

	//memset nos vale para poner toda una sección de memoria
	//al mismo valor. En este caso, 0. DUDAS: terminal-> man memset
	memset((void*)&hints, '\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET; //Para IPv4 usamos AF_INET
	hints.ai_socktype = SOCK_DGRAM;

	//solo es un puntero porque la función que se encargará de
	//inicializarlo es getaddrinfo.
	//Este addrinfo es de salida de la función.
	//Lleva ya codificado en binario la diureccion y el puerto
	struct addrinfo * res;

	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	if(rc != 0){

		std::cout << "Error: " << gai_strerror(rc) << std::endl;

		return -1;
	}

	size_t sock =
	socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	bind(sock, (struct sockaddr *)res->ai_addr, res->ai_addrlen);


	bool close = false;
	struct sockaddr cliente;
	socklen_t cliente_len;
	int numThreads = 0;

	while(!close)
	{

		int bytes =
		recvfrom(sock, &buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cliente, &cliente_len);
		buffer[bytes] = '\n';


		//-----------------------HILO AQUI---------------------------

	}
}
void * manageConection (void * params)
{
	char buffer [BUFFER_SIZE];
	char send_buff[BUFFER_SIZE];
	
	struct sockaddr * cliente;
	socklen_t cliente_len;
	bool connected;

	while(connected)
	{

		char host [NI_MAXHOST];
		char serv [NI_MAXSERV];

 		getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST,
		serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);

		std::cout << "Conexión desde Host: "<< host << " Puerto: " << serv<< std::endl;
 	 	std::cout << "Mensaje("<< bytes <<"): "<< buffer << std::endl;


		if(buffer[0] == 'q')
		{
			strcpy(send_buff, "Quitting...");
			close = true;
		}
		else if(buffer[0] == 't')
					returnTime(send_buff, BUFFER_SIZE);
		else if(buffer[0] == 'd')
					returnDay(send_buff, BUFFER_SIZE);
		else {
					strcpy(send_buff, "Command not recognized.");
		}
		sendto(sock, send_buff, strlen(send_buff) , 0 , (struct sockaddr *) &cliente, cliente_len);

		}

}

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
