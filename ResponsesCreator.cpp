#include "ResponsesCreator.h"
#include <fstream>
#include <cassert>

//------------------------------------------------------------------------------

ResponsesCreator::ResponsesCreator(): abonents_manager_() {
    std::cout << "\n********** Started ****************\n" ;

    std::cout << "Main listening thread id: " << std::this_thread::get_id() << std::endl;
}

//------------------------------------------------------------------------------

const std::shared_ptr<httpserver::http_response> ResponsesCreator::render
        (const httpserver::http_request& req)
{
#ifdef DEBUG
    std::map<std::string, std::string, httpserver::http::header_comparator> headers =
            req.get_headers();

    for (const auto &elem : headers)
    {
        std::cout << elem.first << ": " << elem.second << std::endl;
    }

    std::cout << req.get_content() << std::endl;
#endif //DEBUG

    std::string client { req.get_requestor() + ":"
                         + std::to_string(req.get_requestor_port()) };

    std::cout << "\nClient:\n";
    std::cout << "\t" << client << std::endl;

    std::cout << "\nHandling in thread id: ";
    std::cout << std::this_thread::get_id() << std::endl;

    Abonent abonent{req.get_user(), "unknown", req.get_pass()};

    if(!abonents_manager_.isAbonentValid(abonent))
    {
        std::cout << "Auth failed. Drop client "
            << client << std::endl;
        return
        std::make_shared<httpserver::basic_auth_fail_response>(
                httpserver::basic_auth_fail_response(
                        "FAIL", "supervisor@geekbrains.ru"));
    }

    std::cout << "Auth is OK\n";

    const std::string client_uses_http_method { req.get_method() };
    if (client_uses_http_method == "GET")
    {
        return createResponseToGET(req);
    }
    else if (client_uses_http_method == "POST")
    {
        return createResponseToPOST(req);
    }
    else if (client_uses_http_method == "PATCH")
    {
        return createResponseToPATCH(req);
    }
    else if (client_uses_http_method == "PUT")
    {
        return createResponseToPUT(req);
    }
    else if (client_uses_http_method == "DELETE")
    {
        return createResponseToDELETE(req);
    }
    else //trash
    {
        return std::make_shared<httpserver::string_response>(httpserver::string_response
            ("Method not allowed", 405, "text/plain"));
    }
}

//------------------------------------------------------------------------------

const std::shared_ptr<httpserver::http_response> ResponsesCreator::createResponseToGET
    (const httpserver::http_request &client_request) {

    const auto abonent =
            abonents_manager_.findAbonentByID(client_request.get_user());

    assert(abonent != abonents_manager_.getAbonents().end());

    if(client_request.get_path() == "name")
    {
        return std::make_shared<httpserver::string_response>
                ( httpserver::string_response(abonent->getName(),
    200, "text/plain"));
    }

    if(client_request.get_path() != "income")
    {

       return std::make_shared<httpserver::string_response>
            ( httpserver::string_response("Resource not found: " +
                client_request.get_path(),
                       404, "text/plain"));
    }

    std::string income_msgs{};
    size_t msgs_count = abonent->getIncomeMessages().size();

    for (const auto &msg: abonent->getIncomeMessages())
    {
        income_msgs.append("From " + msg.from_ + ": " + msg.text_ + "\n");
        abonent->removeIncomeMsg(msg);
    }

    switch (msgs_count)
    {
        case(0):
        {
           income_msgs = "You have no message for now.";
           break;
        }
        case(1):
        {
            income_msgs.insert(0, "You've got one message:\n");
            break;
        }
        default: // size() > 1
        {
            income_msgs.insert(0, "You've got " + std::to_string(msgs_count)
                + " messages:\n");
            break;
        }
    }

    return std::make_shared<httpserver::string_response>
            ( httpserver::string_response(income_msgs,
                       200, "text/plain"));

}

//------------------------------------------------------------------------------

const std::shared_ptr<httpserver::http_response> ResponsesCreator::createResponseToPOST
        (const httpserver::http_request &client_request) {

    std::cout << "Client send POST: " << client_request.get_path(); //debug

    const auto abonent =
            abonents_manager_.findAbonentByID(client_request.get_path());

    if(abonent != abonents_manager_.getAbonents().end())
    {
        const Message message{client_request.get_user(),
    client_request.get_content() };
        abonent->addIncomeMsg(message); //what if message.txt_ is empty?

#ifdef DEBUG
        std::cout << "\n********************************************\n";
        std::cout << "Size of abonents_: " << abonents_manager_.getAbonents().size()
            << std::endl;

        std::cout << "\nMessage \'" + message.text_ + "\' sent "
            "to abonent \'" + "\'" + abonent->getName() + "\' from "
                << client_request.get_user() << std::endl;

        for (const auto &elem : abonents_manager_.getAbonents())
        {
            std::cout << "\n@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
            std::cout << "id: " << elem.getUniqueID() << std::endl;
            std::cout << "name: " << elem.getName() << std::endl;
            std::cout << "pass: " << elem.getPass() << std::endl;
            std::cout << "Size of inc msgs: " << elem.getIncomeMessages().size() << ":";
            std::cout << std::endl;
            for (const auto &msg : elem.getIncomeMessages())
            {
                std::cout << "msg from " << msg.from_ << ": " << msg.text_ << std::endl;
            }
        }
        std::cout << "\n********************************************\n";
#endif //DEBUG
        return std::make_shared<httpserver::string_response>
                (httpserver::string_response("Message \'" + message.text_
                    + "\' sent to abonent \'" + abonent->getName() + "\'.",
            200, "text/plain"));
    }

   return std::make_shared<httpserver::string_response>
            ( httpserver::string_response("Abonent not found: " +
            client_request.get_path(),404, "text/plain"));
}

//------------------------------------------------------------------------------

const std::shared_ptr<httpserver::http_response>
    ResponsesCreator::createResponseToPATCH(
        const httpserver::http_request &client_request) {


    std::cout << "Client sent PATCH: " << client_request.get_path(); //debug

    const auto abonent =
            abonents_manager_.findAbonentByID(client_request.get_user());

    assert(abonent != abonents_manager_.getAbonents().end());

    if(client_request.get_path() == "password")
    {
       abonent->setPassword(client_request.get_content());

        return std::make_shared<httpserver::string_response>
                ( httpserver::string_response(
             "Your password changed successfully to \'" + abonent->getPass()
                + "\'.", 200, "text/plain"));
    }
    else
        if(client_request.get_path() == "name")
        {
           abonent->setName(client_request.get_content());

            return std::make_shared<httpserver::string_response>
                    ( httpserver::string_response(
        "Your name changed successfully to \'" +
        abonent->getName() + "\'", 200, "text/plain"));
        }

    return std::make_shared<httpserver::string_response>
            ( httpserver::string_response("Resource not found: " +
                client_request.get_path(),
        404, "text/plain"));
}

//------------------------------------------------------------------------------

const std::shared_ptr<httpserver::http_response>
        ResponsesCreator::createResponseToPUT(
            const httpserver::http_request &client_request) {

    std::optional<std::shared_ptr<httpserver::http_response>> check_for_root =
            createResponseToNonRootRequest(client_request);
    if(check_for_root != std::nullopt)
        return check_for_root.value();

    Abonent new_abonent{ client_request.get_path(), "", client_request.get_content() };
    if(!abonents_manager_.addAbonent(new_abonent))
    {
        return std::make_shared<httpserver::string_response>
            ( httpserver::string_response("Conflict: Abonent \'" +
                new_abonent.getUniqueID() + "\' already registered.",
                    409, "text/plain"));
    }

    return std::make_shared<httpserver::string_response>
            (httpserver::string_response("Abonent \'" +
                new_abonent.getUniqueID() + "\' added successfully.",
                    200, "text/plain"));
}

//------------------------------------------------------------------------------

const std::shared_ptr<httpserver::http_response> ResponsesCreator::createResponseToDELETE
        (const httpserver::http_request &client_request) {

    std::optional<std::shared_ptr<httpserver::http_response>> check_for_root =
            createResponseToNonRootRequest(client_request);
    if(check_for_root != std::nullopt)
        return check_for_root.value();

    if (client_request.get_path() == "@exit@")
    {
        std::cout << "Got \'EXIT\' command from client "
                  << client_request.get_requestor() << ":"
                  << client_request.get_requestor_port()<< std::endl;

        abonents_manager_.saveAbonents();
        std::exit(1);
    }

    const auto abonent_to_remove =
            abonents_manager_.findAbonentByID(client_request.get_path());

    if(abonent_to_remove == abonents_manager_.getAbonents().cend())
    {
        return std::make_shared<httpserver::string_response>
                (httpserver::string_response("Abonent not found: " +
            client_request.get_path(),404, "text/plain"));
    }

    const std::string temp_name{ abonent_to_remove->getName() };
    abonents_manager_.removeAbonent(*abonent_to_remove);
    return std::make_shared<httpserver::string_response>
            (httpserver::string_response("Abonent \'" + temp_name
                + "\' removed successfully.",200, "text/plain"));
}

//------------------------------------------------------------------------------

const std::optional<std::shared_ptr<httpserver::http_response>>
        ResponsesCreator::createResponseToNonRootRequest(
                const httpserver::http_request &client_request) {

    const auto abonent =
            abonents_manager_.findAbonentByID(client_request.get_user());
    assert(abonent != abonents_manager_.getAbonents().end());

    //only root (first registered user in account list) allowed here
    if (abonent->getUniqueID() != abonents_manager_.getAbonents().at(0).getUniqueID())
    {
        return std::make_shared<httpserver::string_response>
                (httpserver::string_response(
                        "Forbidden", 403, "text/plain"));
    }

    return std::nullopt;
}

//------------------------------------------------------------------------------
