//
// Created by spl211 on 01/01/2021.
//

#ifndef BGRSCLIENT_FRAME_H
#define BGRSCLIENT_FRAME_H
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <mutex>
#include <queue>

using namespace std;

class Frame {

private:

    //Client_User queue
    vector<string>* messages;
    //Client_Server queue
    //vector<string>* Client_Server_Queue;
    mutex* mut;
    bool terminate;
    bool handled;
    void clear();


public:
    Frame(mutex* mut);      //constructor
    ~Frame();               //destructor
    Frame(const Frame& other);  //copy constructor
    Frame& operator=(const Frame &other);       //copyAssignment
    //Frame(const Frame &&other);                  //move constructor
    //Frame& operator=(Frame &&other);            //moveAssignment



    void addMessage(string request);
    string getMessage();
     bool shouldTerminate();
    void setTerminate();
    bool getHandled();
    void setHandled(bool b);


};


#endif //BGRSCLIENT_FRAME_H
