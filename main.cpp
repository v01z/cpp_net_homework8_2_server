#include "MessengerServer.h"

//------------------------------------------------------------------------------

int main(int argc, const char * const argv[]) {

    unsigned short port{ 8080 };
    std::string mem_key{ "key.pem" };
    std::string mem_cert{ "cert.pem" };

    if(argc == 4)
    {
        port = std::stoi(argv[1]);
        mem_key = argv[2];
        mem_cert = argv[3];
    }
    else
    {
        if(argc == 2 && !std::strncmp("--help", argv[1], 6))
        {
            std::cout << "Usage: " << argv[0] << " <port> "
                << "<key> <certificate>\n";
            return EXIT_FAILURE;
        }
        std::cout << "Using default parameters:\n";
        std::cout << argv[0] << " port: " << port
            << " key: " << mem_key
                << " cert: " << mem_cert
                    << std::endl;
    }

    MessengerServer server{port, mem_key, mem_cert };

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
