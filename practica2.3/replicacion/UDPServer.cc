#include "UDPServer.h"
#include "Serializable.h"

UDPServer::UDPServer(const char * serv, const char * port)
    :socket(serv, port)
{
    pthread_mutex_init(&mutex, 0);
};

UDPServer::~UDPServer()
{
    pthread_mutex_destroy(&mutex);
};

// ----------------------------------------------------------------------------

extern "C" void * _server_thread(void *arg)
{
    UDPServer * server = static_cast<UDPServer *>(arg);

    server->server_thread();

    return 0;
}

// ----------------------------------------------------------------------------

int UDPServer::start()
{
}

// ----------------------------------------------------------------------------

void UDPServer::server_thread()
{
}

// ----------------------------------------------------------------------------

void UDPServer::add_connection (Socket * s)
{
  //buscamos si el socket está
  int i = 0;
  bool notFound = true;
  while(i<connections.size()&&notFound){
    if(&connections.at(i)==&s)
      notFound = false;
    i++;
  }
  // si esta se mete si no se mata
  if(notFound)
    connections.push(s);
  else
    delete s;

}

// ----------------------------------------------------------------------------

void UDPServer::del_connection (Socket * s)
{
  int i = 0;
  bool found = false;
  while(i<connections.size()&&!found){
    if(&connections.at(i)==&s)
      found = true;
    i++;
  }

  if(found)
    connections.erase(connections.begin()+i);

}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
