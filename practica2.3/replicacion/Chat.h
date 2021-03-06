#include <string>
#include <unistd.h>
#include <string.h>
#include "Serializable.h"
#include "Socket.h"
#include "UDPServer.h"
#include <stdio.h>
#include <exception>

class ChatMessage: public Serializable
{
public:
    ChatMessage(){};

    ChatMessage(const char * n, const std::string m)
    {
      strcpy(nick, n);
    strcpy(message, m.c_str());
    };

    void to_bin()
    {

      int32_t offset = sizeof(int32_t);
      alloc_data(sizeof(char)*8 + sizeof(char)*80);

      memcpy(_data+offset, nick, sizeof(char)*8);
      offset += sizeof(char)*8;

      memcpy(_data+offset, message, sizeof(char)*80);


    }

    virtual int from_bin(char * bobj)
    {

      size_t offset =sizeof(int32_t);
      memcpy(nick, _data+offset, sizeof(char)*8);
      offset+= sizeof(char)*8;
      memcpy(message, _data+offset, sizeof(char)*80);

    return 0;
    }

    char message[80];

    char nick[8];
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ChatServer : public UDPServer
{
public:
    ChatServer(const char * s, const char * p): UDPServer(s, p){};

    void do_message(char * buffer)
    {
      //recepción de mensajes
      Socket * a;
      socket.recv(buffer, &a);
      std::cout << "Message Received: "<< std::endl;
      //Serialización del mensaje
      ChatMessage msg = ChatMessage();
      msg.from_bin(buffer);
      //Envío del mensaje por la red
      for(int i = 0; i < connections.size(); i++)
      {
        socket.send(&msg, connections[i]);
      }
    }
};
#include <vector>
class ChatClient
{
public:
    ChatClient(const char * s, const char * p, const char * n):socket(s, p)
        {
          strcpy(nick, n);
        };

    void input_thread()
    {
      while(true)
      {
        char input [80];
        std::cin >> input;
        ChatMessage msg = ChatMessage(nick, input);
        msg.to_bin();



        socket.send(&msg, &socket);
      }
    }

    void net_thread()
    {
      //recepción de mensajes
      Socket * a = NULL;

      while(true)
      {
        std::cout << "A"<<std::endl;
        char * buffer;
        socket.recv(buffer, &a);
        std::cout << "B"<<std::endl;
        std::cout << buffer<<std::endl;
        std::cout << "C"<<std::endl;
        //Serialización del mensaje
        ChatMessage msg;
        msg.from_bin(buffer);
        //Envío del mensaje por la red
        if(msg.nick != nick) std::cout << msg.nick<<": "<< msg.message << std::endl;
      }
    }

private:
    char nick[8];

    Socket socket;
};
