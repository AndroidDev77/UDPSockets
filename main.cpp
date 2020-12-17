#include <iostream>
#include <pthread.h>

#include "PODLServer.h"
#include "PODLClient.h"

//Function for PODL Server
void *serverThread(void* arg)
{
    std::string password = "password";
	PODLServer serv("127.0.0.1", 10000, password);
	serv.Run();

    return 0;
}
int main(int, char**) {
    pthread_t thread;
    pthread_create(&thread, NULL, serverThread, (void*) "Server Thread");
    
    PODLClient cli("127.0.0.1", 10000);
	cli.Run();

    return 0;
}
