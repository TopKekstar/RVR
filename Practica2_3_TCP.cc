#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>

#include <iostream>
#include <string.h>

#define BUFFER_SIZE 100

void returnDay(char * buf, size_t buflen);
void returnTime(char * buf, size_t buflen);
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
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

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

   	listen(sock, 5);
   	
   	freeaddrinfo(res);
	char buffer [BUFFER_SIZE];
	char send_buff[BUFFER_SIZE];

	bool closeS = false;
	struct sockaddr cliente;
	socklen_t cliente_len;
	
	//Aceptamos conexiones indefinidamente.
	while(true)
	{
		std::cout << "Awaiting connections..." << std::endl;
		int client_s = accept(sock, &cliente, &cliente_len);
		
		
		char host [NI_MAXHOST];
		char serv [NI_MAXSERV];
			
		//Aquí parseamos la información de la conexión que hemos recibido.
		//Para imprimirla por pantalla
 		getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST,
      		serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);

  		std::cout << "Conexión desde Host: "<< host << " Puerto: " << serv<< std::endl;
  		std::cout << "Client ID: " << client_s << std::endl;
  		
  		
		while(!closeS){
			int errcode = recv(client_s, buffer, BUFFER_SIZE, 0);
			
			if(errcode >= 0){
				if(buffer[0] == 'q'){
					strcpy(send_buff, "Quitting...");
					closeS = true;
					close(client_s);
				}
				else if(buffer[0] == 't')
					returnTime(send_buff, BUFFER_SIZE);		
		
				else if(buffer[0] == 'd')
					returnDay(send_buff, BUFFER_SIZE);		
				else {
					strcpy(send_buff, "Command not recognized.");
				}
				
				send(client_s, send_buff, BUFFER_SIZE, 0);
			}
			else {
				closeS = true;
				std::cout << "ERROR: " << gai_strerror(errcode)<< std::endl;
			}
		}
		
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

