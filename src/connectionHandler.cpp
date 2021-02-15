#include <connectionHandler.h>
 
using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
 
ConnectionHandler::ConnectionHandler(string host, short port): host_(host), port_(port), io_service_(), socket_(io_service_) , finish(false) , opcode("0"){}
    
ConnectionHandler::~ConnectionHandler() {
    close();
}
 
bool ConnectionHandler::connect() {
    std::cout << "Starting connect to " 
        << host_ << ":" << port_ << std::endl;
    try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
		boost::system::error_code error;
        socket_.connect(endpoint, error);

        if (error)
			throw boost::system::system_error(error);
    }
    catch (std::exception& e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    size_t tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp ) {
			tmp += socket_.read_some(boost::asio::buffer(bytes+tmp, bytesToRead-tmp), error);			
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }

    return true;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
    int tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp ) {

            tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::getLine(std::string& line) {
    return getFrameAscii(line);
}

bool ConnectionHandler::sendLine(std::string& line) {
    return sendFrameAscii(line);
}
 
bool ConnectionHandler::getFrameAscii(std::string& frame) {
    char* bytes = new char [4];
    char ch;
    int count = 0;
    short opcode;
    short about;
    bool stop(false);
    string data("");

    // Stop when we encounter the null character.
    // Notice that the null character is not appended to the frame string.
    try {
	do{
		if(!getBytes(&ch, 1)){return false;}

		if (count < 4){
		    bytes[count] = (ch);
            count++;
		}

		if (count == 4){
		    char* tmp = new char[2];
		    tmp[0] = bytes[0];
		    tmp[1] = bytes[1];
		    opcode = bytesToShort(tmp);
            tmp[0] = bytes[2];
            tmp[1] = bytes[3];
            about = bytesToShort(tmp);
            delete tmp;
            count++;
            if(opcode == 13){break;}
		}


		else if (count > 4){
		    if (ch != '\0'){data.append(1 , ch);}
		    else{stop = true;}
		}


	}while (!stop);
    } catch (std::exception& e) {
	std::cerr << "recv failed2 (Error: " << e.what() << ')' << std::endl;
        delete bytes;
	return false;
    }

    std::stringstream ss;
    ss << about;
    if (opcode == 12){
        frame.append("ACK ");
        frame.append(ss.str());
        frame.append(data);

    }
    else{
        frame.append("ERROR ");
        frame.append(ss.str());
    }

    delete bytes;
    return true;

}
 
 bool ConnectionHandler::sendFrameAscii(const std::string& frame) {
	return sendBytes(frame.c_str(),frame.length());

}
 
// Close down the connection properly.
void ConnectionHandler::close() {
    try{
        socket_.close();
    } catch (...) {
        std::cout << "closing failed: connection already closed" << std::endl;
    }
}

short ConnectionHandler::bytesToShort(char* bytesArr){
    short result = (short)((bytesArr[0] & 0xFF) << 8);
    result += (short)(bytesArr[1] & 0xFF);
    return result;
}
