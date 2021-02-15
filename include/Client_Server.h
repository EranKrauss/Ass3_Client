//
// Created by spl211 on 01/01/2021.
//

#ifndef BGRSCLIENT_CLIENT_SERVER_H
#define BGRSCLIENT_CLIENT_SERVER_H
#include <stdlib.h>
#include <string>
#include <iostream>
#include "../include/connectionHandler.h"
#include "../include/Frame.h"
#include <boost/asio.hpp>
#include <mutex>
using namespace std;

class Client_Server {
private:

    Frame* frame;
    bool* shouldTerminate;
    ConnectionHandler* connectionHandler;

    vector<string> cutString(string s , string cut);
    void clear();
    const char* encode(string data , short opcode);
    short bytesToShort(char* bytesArr);
    void shortToBytes(short num , char* bytesArr);
    string prepareToSend(string m);
    bool prepareAndSend(string s);
    bool sendToServer(short opcode1 , short opcode2 ,string data );



public:
    Client_Server(ConnectionHandler* connectionHandler , Frame* frame );
    ~Client_Server();
    Client_Server(const Client_Server& other);  //copy constructor
    Client_Server& operator=(const Client_Server &other);       //copyAssignment
    void run();
    bool isShouldTerminate();
    void setShouldTerminate(bool b);
    vector<string> cutStringByChar(string s , char cut);
    short stringToShort(string s);




};


#endif //BGRSCLIENT_CLIENT_SERVER_H
