#include <iostream>
#include <pthread.h>

#include "PODLServer.h"
#include "PODLClient.h"



//Function for PODL Server
void *serverThread(void* arg)
{   
    PODLServer* serv = reinterpret_cast<PODLServer*>(arg);
	(*serv).Run();

    return 0;
}

int main(int, char**) {
    std::string password = "password";
    int port = 10000;

    // Create Server and Client
    PODLServer* serv = new PODLServer("127.0.0.1", port, password);
    PODLClient* cli = new PODLClient("127.0.0.1", port);

    // Run Server in thread
    pthread_t thread;
    pthread_create(&thread, NULL, serverThread, (void*) serv);
    

	(*cli).Run();

    return 0;
}
