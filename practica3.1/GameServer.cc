#include <unistd.h>
#include "GameServer.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
/**
 *  Esta función envía el mensaje a todos los clientes conectados:
 *  1. Serializa el mundo
 *  2. Recorre las conexiones y para cada uno envía el mundo serializado
 *  3. Si hay error elimina la conexion
 */
void GameServer::broadcast()
{
  gw.to_bin();
  for (size_t i = 0; i < connections.size(); i++) {
    if(socket.send(&gw,connections[i])!=0)
      i = connections.size();
  }

}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

int main(int argc, char ** argv)
{
    GameServer gs(argv[1], argv[2]);

    if ( gs.start() == -1 )
    {
        return -1;
    }

    while (true)
    {
        gs.simulate();

        gs.broadcast();

        usleep(33000);
    }

    return 0;
};
