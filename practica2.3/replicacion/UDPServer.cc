#include "UDPServer.h"
#include "Serializable.h"
#include <pthread.h>



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
    for(int i = 0; i < THREAD_POOL_SIZE; i++)
    {
      pthread_t tid;
      pthread_attr_t attr;

      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

      pthread_create(&tid, &attr, _server_thread,  static_cast<void *>(this));



    }

}

// ----------------------------------------------------------------------------

void UDPServer::server_thread()
{
  while (true) {
    Socket * s;
    char * buffer;
    socket.recv(buffer, &s);
    add_connection(s);
    do_message(buffer);


  }
}

// ----------------------------------------------------------------------------

void UDPServer::add_connection (Socket * s)
{
  //buscamos si el socket está
  pthread_mutex_lock(&mutex);
  int i = 0;
  bool notFound = true;
  while(i<connections.size()&&notFound){
    if(&connections.at(i)==&s)
      notFound = false;
    i++;
  }
  // si esta se mete si no se mata
  if(notFound)
    connections.push_back(s);
  else
    delete s;
  pthread_mutex_unlock(&mutex);


}

// ----------------------------------------------------------------------------

void UDPServer::del_connection (Socket * s)
{
  pthread_mutex_lock(&mutex);
  int i = 0;
  bool found = false;
  while(i<connections.size()&&!found){
    if(&connections.at(i)==&s)
      found = true;
    i++;
  }

  if(found)
    connections.erase(connections.begin()+i);
  pthread_mutex_unlock(&mutex);

}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
