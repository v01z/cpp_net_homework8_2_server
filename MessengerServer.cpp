#include "MessengerServer.h"

//------------------------------------------------------------------------------

void resource_access_log(const std::string& url) {
    std::cout << "\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
    std::cout << "New connection is detected:\n";
    std::cout << "ACCESSING: " << url << std::endl;
}

void resource_error_log(const std::string& err_str){
    std::cout << "ERROR: " << err_str << std::endl;
}

//------------------------------------------------------------------------------

MessengerServer::MessengerServer(const unsigned short port, const std::string& key,
                                 const std::string& cert):
        ws_ { httpserver::create_webserver(port)
                      .use_ssl()
                      .https_mem_key(key)
                      .https_mem_cert(cert)
                      .log_access(resource_access_log)
                      .log_error(resource_error_log)
        .start_method(httpserver::http::http_utils::THREAD_PER_CONNECTION)
        }

{
    ws_.register_resource("/", &file_resources_, true);
    ws_.start(true);
}

//------------------------------------------------------------------------------

