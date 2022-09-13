
#ifndef RESPONSES_CREATOR_H
#define RESPONSES_CREATOR_H

#include "httpserver.hpp"
#include <thread>
#include "AbonentsManager.h"

//------------------------------------------------------------------------------

class ResponsesCreator : public httpserver::http_resource{
private:
    AbonentsManager abonents_manager_;

    const std::shared_ptr<httpserver::http_response>
        createResponseToGET(const httpserver::http_request&);

    const std::shared_ptr<httpserver::http_response>
        createResponseToPOST(const httpserver::http_request&);

    const std::shared_ptr<httpserver::http_response>
        createResponseToPATCH(const httpserver::http_request&);

    const std::shared_ptr<httpserver::http_response>
        createResponseToPUT(const httpserver::http_request&);

    const std::shared_ptr<httpserver::http_response>
        createResponseToDELETE(const httpserver::http_request&);

    const std::optional<std::shared_ptr<httpserver::http_response>>
        createResponseToNonRootRequest(const httpserver::http_request&);

public:
    ResponsesCreator();

    const std::shared_ptr<httpserver::http_response>
        render(const httpserver::http_request&) override;

};

//------------------------------------------------------------------------------

#endif //RESPONSES_CREATOR_H