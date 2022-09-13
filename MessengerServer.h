
#ifndef MESSENGER_SERVER_H
#define MESSENGER_SERVER_H

#include "ResponsesCreator.h"

//------------------------------------------------------------------------------

inline void resource_access_log(const std::string&);

inline void resource_error_log(const std::string&);

//------------------------------------------------------------------------------

class MessengerServer {
private:
    httpserver::webserver ws_;
    ResponsesCreator file_resources_;
public:
    MessengerServer() = delete;
    MessengerServer(const unsigned short, const std::string&, const std::string&);
};

//------------------------------------------------------------------------------


#endif //MESSENGER_SERVER_H
