#include "Socket.h"
#include "Serializable.h"

#include <string.h>

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Implementación Socket
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

bool operator== (const Socket &s1, const Socket &s2)
{
  return s1.sd==s2.sd;
}

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;
}

// ----------------------------------------------------------------------------

Socket::Socket(const char * address, const char * port):sd(-1)
{

  struct addrinfo hints;

	//memset nos vale para poner toda una sección de memoria
	//al mismo valor. En este caso, 0. DUDAS: terminal-> man memset
	memset((void*)&hints, '\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET; //Para IPv4 usamos AF_INET
	hints.ai_socktype = SOCK_DGRAM;

  struct addrinfo * res;

	int rc = getaddrinfo(address,port , &hints, &res);

	if(rc != 0){

		std::cout << "Error: " << gai_strerror(rc) << std::endl;

	}
  else
  {
    sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    sa_len = res->ai_addrlen;
    sa = *res->ai_addr;
  }

}

// ----------------------------------------------------------------------------

int Socket::bind()
{
    return ::bind(sd, (const struct sockaddr *) &sa, sa_len);
}

// ----------------------------------------------------------------------------

int Socket::send(Serializable * obj, Socket * sock)
{
  return sendto(sd, obj->data(), obj->size() , 0 , &sock->sa, sock->sa_len);

}

// ----------------------------------------------------------------------------

int Socket::recv(char * buffer, Socket ** sock)
{
  return recvfrom(sd, buffer, MAX_MESSAGE_SIZE, 0, &(*sock)->sa, &(*sock)->sa_len);

}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
