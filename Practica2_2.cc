#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <iostream>
#include <string.h>

/*int getaddrinfo(const char *node,
              	const char *service,
               	const struct addrinfo *hints,
       		struct addrinfo **res);*/
using namespace std;
int main(int argc, char **argv) {

	//Variable hints para almacenar los filtros a pasarle.
	//No es puntero para evitar accesos descontrolados a memoria dinámica
	struct addrinfo hints;

	//memset nos vale para poner toda una sección de memoria
	//al mismo valor. En este caso, 0. DUDAS: terminal-> man memset
	memset((void*)&hints, '\0', sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC; //Para IPv4 usamos AF_INET
	//hints.ai_socktype = SOCKET_DGRAM;

	//solo es un puntero porque la función que se encargará de
	//inicializarlo es getaddrinfo.
	 struct addrinfo * res;

	int rc = getaddrinfo(argv[1], NULL, &hints, &res);

	if(rc != 0){

	std::cout << "Error: " << gai_strerror(rc) << std::endl;

	return -1;

	}
	//int sd = socket (res->ai_family, res->ai_sockype, 0);
	//bind(sd, res->ai_addr, res->ai_addrlen);

	//Ahora vamos a imprimir todos los nombres de la lista enlazada.
	for (struct addrinfo * tmp = res; tmp != NULL; tmp = tmp->ai_next)
	{
		/*int getaddrinfo(const char *node,
                	const char *service,
                	const struct addrinfo *hints,
          		struct addrinfo **res);
	*/
			//Estas constantes que son la máxima longitud para un nombre de host y de
			//servicio,están definidas en el SO
		char host [NI_MAXHOST];
		char serv	[NI_MAXSERV];


		getnameinfo(tmp->ai_addr, tmp->ai_addrlen,
			host, NI_MAXHOST,
		 	serv, NI_MAXSERV,
			NI_NUMERICHOST | NI_NUMERICSERV);
				

		std::cout << host <<" || " << tmp->ai_family << " || " << tmp->ai_socktype << std::endl;
		
		

	}

	freeaddrinfo(res);
	return 0;
}
