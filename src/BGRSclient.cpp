#include <stdlib.h>
#include <string>
#include <iostream>
#include "../include/connectionHandler.h"
#include "../include/Frame.h"
#include "../include/Client_Server.h"
#include "thread"


using namespace std;


/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {

    //initialize
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    ConnectionHandler* connectionHandler = new ConnectionHandler(host, port);
    if (!(*connectionHandler).connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    //create Client_Server Thread
    mutex* mut = new mutex();
    Frame* frame = new Frame(mut);
    Client_Server* clientServer = new Client_Server(connectionHandler , frame);
    thread t1(&Client_Server::run , clientServer);


    //main loop
    bool terminate(false);
    string line;
    while (!terminate) {

        //cout<< "please enter request" <<endl;
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        line = (buf);
        (*frame).addMessage(line);
        if (line.size() > 5 && line.substr(0 , 6) == "LOGOUT") {
            //cout<<"im in logout" <<endl;
            while (!(*frame).getHandled()) {}
            if (frame->shouldTerminate()) {
                terminate = true;
                //cout<< "User Interface shutting down.." <<endl;
            }

            (*frame).setHandled(false);
        }
    }

    t1.join();
    delete mut;
    delete frame;
    delete clientServer;
    delete connectionHandler;
    return 0;

}
