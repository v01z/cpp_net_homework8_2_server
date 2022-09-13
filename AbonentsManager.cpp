
#include "AbonentsManager.h"
#include <fstream>
#include <sstream>

//------------------------------------------------------------------------------

bool AbonentsManager::isAbonentValid(const Abonent &abonent)const{
    if (abonent.getUniqueID().empty())
        return false;

    if(find_if(abonents_.begin(), abonents_.end(),
               [&abonent](const Abonent& abonent_in_vec){
                   return abonent == abonent_in_vec;
               }) != abonents_.end())
    {
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------

const std::vector<Abonent>::iterator AbonentsManager::findAbonentByID(const std::string &id) {
    if (id.empty())
        return abonents_.end();

    auto vec_iter = find_if(abonents_.begin(), abonents_.end(),
    [&id](const Abonent& abonent_in_vec){
                   return id == abonent_in_vec.getUniqueID();
               });
    if(vec_iter != abonents_.end())
    {
        return vec_iter;
    }

    return abonents_.end();
}

//------------------------------------------------------------------------------

AbonentsManager::AbonentsManager() {
    if(!inflateAbonents())
    {
        abonents_.push_back(ROOT);
    }
}

//------------------------------------------------------------------------------

AbonentsManager::~AbonentsManager() {
    saveAbonents();
}

//------------------------------------------------------------------------------

const std::vector<Abonent> &AbonentsManager::getAbonents() const {
    return abonents_;
}

//------------------------------------------------------------------------------

bool AbonentsManager::inflateAbonents() {
    std::cout << "Checking logins database (" << AUTH_BASE << ")..";
    std::ifstream auth_file{ AUTH_BASE };
    if(!auth_file.is_open())
    {
        std::cout << "Cant open file " << AUTH_BASE
        << std::endl;
        return false;
    }

    std::stringstream file_stream;
    file_stream << auth_file.rdbuf();

    while(file_stream.good())
    {
        std::string id, name, password;
        file_stream >> id >> name >> password;
        Abonent abonent{id, name, password };
        abonents_.push_back(abonent);
    }
    auth_file.close();
    std::cout << ".. OK\n";
    return true;
}

//------------------------------------------------------------------------------

bool AbonentsManager::saveAbonents() const {
    std::cout << "Saving logins database (" << AUTH_BASE << ")..";

    std::ofstream auth_file;
    auth_file.open(AUTH_BASE, std::ofstream::out
        | std::ofstream::trunc);

    if(!auth_file.is_open())
    {
        std::cout << "Can't open file " << AUTH_BASE
        << std::endl;
        return false;
    }

    for (size_t i{}; i < abonents_.size(); ++i)
    {
        auth_file << abonents_.at(i);
        if(i < abonents_.size() - 1)
            auth_file << std::endl;
    }

    auth_file.close();
    std::cout << ".. OK\n";
    return true;
}

//------------------------------------------------------------------------------

bool AbonentsManager::addAbonent(const Abonent &abonent){

    if(find_if(abonents_.begin(), abonents_.end(),
               [&abonent](const Abonent& abonent_in_vec){
        return abonent.getUniqueID() == abonent_in_vec.getUniqueID();
    }) != abonents_.end())
    {
        std::cout << "FAIL: Abonent " << abonent.getName() << " is already registered.\n";
        return false;
    }

    abonents_.emplace_back(abonent);

    return true;
}

//------------------------------------------------------------------------------

bool AbonentsManager::removeAbonent(const Abonent &abonent) {

    auto abonent_iter =
        find_if(abonents_.begin(), abonents_.end(),
                [&abonent](const Abonent& abonent_in_vec){
                    return abonent.getUniqueID() == abonent_in_vec.getUniqueID(); });

    if (abonent_iter == abonents_.end())
    {
        std::cout << "FAIL: Abonent " << abonent.getName() << " not found.\n";
        return false;
    }

    abonents_.erase(abonent_iter);
    return true;
}

//------------------------------------------------------------------------------
