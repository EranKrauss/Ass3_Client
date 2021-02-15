//
// Created by spl211 on 01/01/2021.
//
#include "../include/Client_Server.h"

Client_Server::Client_Server(ConnectionHandler* connectionHandler , Frame* frame  ) :frame(frame) , shouldTerminate(new bool (false)) , connectionHandler(connectionHandler) {}
Client_Server::~Client_Server(){this->clear();}
//copy constructor
Client_Server::Client_Server(const Client_Server& other) : frame(other.frame) , shouldTerminate(new bool (other.shouldTerminate))
        ,connectionHandler(other.connectionHandler) {
}
//copyAssignment
Client_Server& Client_Server::operator=(const Client_Server &other){
    if(this == &other){
        return *this;
    }
    this->clear();
    this->connectionHandler = other.connectionHandler;
    this->shouldTerminate = other.shouldTerminate;
    this->frame = other.frame;

    return *this;
}


void Client_Server::run(){

    while (!(*shouldTerminate)) {

        string message = frame->getMessage();           //take message from frame
        if(message == ("-1")){continue;}                //no message case

        while(true) {
            bool sent = prepareAndSend(message);              //got message
            if (sent) {
                //wait for message
                string answer;
                if (!(*connectionHandler).getLine(answer)) {
                    std::cout << "Disconnected. Exiting...\n" << std::endl;
                    break;
                }
                cout << answer << endl;

                if (message.length() >= 6 && message.substr(0, 6) == ("LOGOUT")/* && answer.substr(0 ,3) == ("ACK")*/) {
                    if (answer.substr(0, 3) == ("ACK")) {
                        //cout<<"i did this shit" <<endl;
                        *shouldTerminate = true;
                        frame->setTerminate();
                    }

                    frame->setHandled(true);
                }
                //frame->setHandled(true);
                break;
            }
        }
    }
}
vector<string> Client_Server::cutString(string s , string cut){
    vector<string> vec;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(cut)) != std::string::npos) {
        token = s.substr(0, pos);
        vec.push_back(token);
        s.erase(0, pos + cut.length());
    }
    vec.push_back(s);
    return vec;
}
void Client_Server::clear() {
    //delete frame;
    delete shouldTerminate;
}
void Client_Server::shortToBytes(short num , char* bytesArr){
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}
bool Client_Server::prepareAndSend(string m){
    string data;
    vector<string> vec;
    short opcode1;
    short opcode2(0);


    if ( m.length() < 5) {return false;}
    else if(m.length() > 9 && m.substr(0 , 9) == ("ADMINREG ")){
        opcode1 = 1;
        data = (m.substr(9 , m.length()));
        vec = (cutString(data , " "));
        if (vec.size() == 2){
            data = vec.front() + '\0' + vec.back() + '\0';
        }
        else {
            return false;
        }
    }//ADMINREG
    else if(m.length() >  11 && m.substr(0 , 11) == ("STUDENTREG ")){
        opcode1 = 2;
        data = (m.substr(11 , m.length()));
        vec = (cutString(data , " "));
        if (vec.size() == 2){
            string curr1(vec.front());
            string curr2(vec.back());
            data = curr1 + '\0' + curr2 + '\0';
        }
        else {
            return false;
        }
    }//STUDENTREG
    else if(m.length() > 6 && m.substr(0 , 6) == ("LOGIN ")) {
        opcode1 = 3;
        data = (m.substr(6 , m.length()));
        vec = (cutString(data , " "));
        if (vec.size() == 2){
            string curr1(vec.front());
            string curr2(vec.back());
            data = curr1 + '\0' + curr2 + '\0';
        }
        else {
           return false;
        }
    }//LOGIN
    else if(m.length() > 5 && m.substr(0 , 6) == ("LOGOUT")){
        opcode1 = 4;
    }//LOGOUT
    else if(m.length() > 10 && m.substr(0 , 10) == ("COURSEREG ")){
        opcode1 = 5;
        data = (m.substr(10 , m.length()));
        int n = data.length();
        char char_array[n + 1];
        strcpy(char_array , data.c_str());
        opcode2 = atoi(char_array);
    }//COURSEREG
    else if(m.length() > 10 && m.substr(0 , 10) == ("KDAMCHECK ")){
        opcode1 = 6;
        data = (m.substr(10 , m.length()));
        int n = data.length();
        char char_array[n + 1];
        strcpy(char_array , data.c_str());
        opcode2 = atoi(char_array);
    }//KDAMC
    else if(m.length() > 11 && m.substr(0 , 11) == ("COURSESTAT ")){
        opcode1 = 7;
        data = (m.substr(11 , m.length()));
        int n = data.length();
        char char_array[n + 1];
        strcpy(char_array , data.c_str());
        opcode2 = atoi(char_array);

    }//COURSESTAT
    else if(m.length() > 12 && m.substr(0 , 12) == ("STUDENTSTAT ")){
        opcode1 = 8;
        data = (m.substr(12 , m.length()))+ '\0';
    }//STUDENTSTAT
    else if(m.length() > 12 && m.substr(0 , 12) == ("ISREGISTERED")){
        opcode1 = 9;
        data = (m.substr(13 , m.length()));
        int n = data.length();
        char char_array[n + 1];
        strcpy(char_array , data.c_str());
        opcode2 = atoi(char_array);
    }//ISREGISTERED
    else if(m.length() > 10 && m.substr(0 , 11) == ("UNREGISTER ")) {
        opcode1 = 10;
        data = (m.substr(11 , m.length()));
        int n = data.length();
        char char_array[n + 1];
        strcpy(char_array , data.c_str());
        opcode2 = atoi(char_array);
    }//UNREGISTER
    else if(m.length() > 8 && m.substr(0 , 9) == ("MYCOURSES")) {
        opcode1 = 11;
    }//MYCOURSES
    else{
        return false;
    }

    return sendToServer(opcode1 , opcode2 ,data );

}
bool Client_Server::sendToServer(short opcode1 , short opcode2 ,string data ){
    char* opcode1Byte = new char[2];
    char* opcode2Byte = new char[2];
    shortToBytes(opcode1 , opcode1Byte);
    shortToBytes(opcode2 , opcode2Byte);

    if ( !(*connectionHandler).sendBytes(opcode1Byte , 2)) {
        std::cout << "Disconnected. Exiting...\n" << std::endl;
        delete opcode1Byte;
        delete opcode2Byte;
        return false;
    }


    //COURSEREG , KDAMCHECK , COURSESTAT , ISREGISTERED , UNREGISTER
    if (opcode1 == 5 || opcode1 == 6 || opcode1 == 7 || opcode1 == 9 || opcode1 == 10 ){
        if ( !(*connectionHandler).sendBytes(opcode2Byte , 2)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            delete opcode1Byte;
            delete opcode2Byte;
            return false;
        }
        return true;
    }
    //STUDENTSTAT , ADMINREG , STUDENTREG , LOGIN
    else if (opcode1 == 8 || opcode1 == 1 || opcode1 == 2 || opcode1 == 3){

        int n = data.length();
        char char_array[n + 1];
        strcpy(char_array , data.c_str());


        if ( !(*connectionHandler).sendLine(data)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
//            delete ch;
            delete opcode1Byte;
            delete opcode2Byte;
            return false;
        }
        delete opcode1Byte;
        delete opcode2Byte;
        return true;
    }
    //LOGOUT , MYCOURSES
    else{
        delete opcode1Byte;
        delete opcode2Byte;
        return true;}


}


/*
const char* Client_Server::encode(string data , short opcode){
    const char* dataEncoded = data.c_str();
    char* opcodeEncoded = new char ;
    shortToBytes(opcode , opcodeEncoded);
    char* last = new char[strlen(dataEncoded) + strlen(opcodeEncoded)];
    int ind(0);
    int size(strlen(opcodeEncoded));
    for (int i = 0 ; i < size ; i ++){
        (last)[ind] = (opcodeEncoded)[i];
        ind++;
    }
    size = strlen(dataEncoded);
    for (int i = 0 ; i < size ; i ++){
        (last)[ind] = (dataEncoded)[i];
        ind++;
    }
    delete dataEncoded;
    delete opcodeEncoded;
    return last;
}

 short Client_Server::bytesToShort(char* bytesArr){
    short result = (short)((bytesArr[0] & 0xFF) << 8);
    result += (short)(bytesArr[1] & 0xFF);
    return result;
}

 vector<string> Client_Server::cutStringByChar(string s , char cut){
    vector<string> vec;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(cut)) != std::string::npos) {
        token = s.substr(0, pos);
        vec.push_back(token);
        s.erase(0, pos + 1);
    }
    vec.push_back(s);
    return vec;
}


bool Client_Server::isShouldTerminate() {
    return this->shouldTerminate;
}


void Client_Server::setShouldTerminate(bool b){
    delete shouldTerminate;
    this->shouldTerminate =  new bool(b);
}
 */
