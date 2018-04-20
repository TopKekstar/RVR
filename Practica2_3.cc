#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <iostream>
#include <string.h>

#define BUFFER_SIZE 100

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

	char * buffer [BUFFER_SIZE];
	bool close = false;
	struct addrinfo client;
	
	while(!close)
	{
		int bytes =
		recvfrom(sock, buffer, BUFFER_SIZE, 	
	

	}





}
