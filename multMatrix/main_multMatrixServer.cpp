#include "multmatrix_imp.h"
#include "utils.h"

#define PORT 40000

using namespace std;

void clientThread(int clientID)
{
    // HolaMundo_imp* hmi = new HolaMundo_imp(clientID);
    // hmi->recvOp();
    // delete hmi;

    multMatrix_imp* mx = new multMatrix_imp(clientID);
    mx->recvOp();
    delete mx;
}

int main()
{
    int serverID = initServer(PORT);

    while (1)
    {
        if(checkNewConnections())
        {
            int clientID = getNewConnection();
            thread* th = new thread(clientThread, clientID);
        }
        //trabajo util si no hay conexiones
        usleep(100);
    }
    
    return 0;
}