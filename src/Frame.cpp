//
// Created by spl211 on 01/01/2021.
//

#include "../include/Frame.h"
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <mutex>

//constructor
Frame::Frame(mutex* mutex) : messages(new vector<string>()) ,mut(mutex ) , terminate(false) , handled(false){
   // this->messages = new vector<string>();
    //this->Client_Server_Queue = new vector<string>();
}
//destructor
Frame::~Frame(){clear();}
//copy constructor
Frame::Frame(const Frame& other) : messages(new vector<string>()) , mut(other.mut) , terminate(other.terminate) , handled(false) {
    //this->messages = new vector<string>();
    for (string s : *other.messages){
        this->messages->push_back(s);
    }
}
//copy Assignment
Frame& Frame::operator=(const Frame &other){
   if(this == &other){
       return *this;
   }
   clear();
   this->handled = (false);
   this->messages = other.messages;
   //this->Client_Server_Queue = other.Client_Server_Queue;
   this->mut = other.mut;
   this->terminate = other.terminate;
   return *this;

}

void Frame::addMessage(string request){
    lock_guard<mutex> lock(*mut);
    this->messages->push_back(request);
}
string Frame::getMessage(){
    lock_guard<mutex> lock(*mut);
    if (messages->empty()){return "-1";}
    string s(this->messages->front());
    this->messages->erase(messages->begin());
    return s;
}


bool Frame::shouldTerminate() {
    lock_guard<mutex> lock(*mut);
    return terminate;
}
void Frame::setTerminate(){
    //lock_guard<mutex> lock(*mut);
    terminate = true;
}
void Frame::clear() {
    //delete &mut;
    delete messages;
}
bool Frame::getHandled(){
    return this->handled;
}
void Frame::setHandled(bool b){
    handled =b;
}