
#include "Abonent.h"
#include <random>

//------------------------------------------------------------------------------

bool Message::operator==(const Message &other_message) const {
   return this->from_ == other_message.from_ &&
    this->text_ == other_message.text_;
}

//------------------------------------------------------------------------------

Abonent::Abonent():
        income_messages_{},
        unique_id_{ generateUniqueID() },
        name_{},
        password_{}
{}

//------------------------------------------------------------------------------

Abonent::Abonent(const std::string& id, const std::string& name, const std::string& pass):
        income_messages_{},
        unique_id_{ id },
        name_{ name },
        password_{ pass }
{}

//------------------------------------------------------------------------------

Abonent::Abonent(const Abonent &other_user):
    income_messages_{ other_user.income_messages_ },
    unique_id_{ other_user.unique_id_ },
    name_{ other_user.name_ },
    password_{ other_user.password_ }
{}

//------------------------------------------------------------------------------

Abonent::Abonent(const std::string &name):
        Abonent{}
{
  name_ = name;
}

//------------------------------------------------------------------------------

Abonent &Abonent::operator=(const Abonent &other_user) {
    if(this == &other_user)
        return *this;

    income_messages_ = other_user.income_messages_;
    unique_id_ = other_user.unique_id_;
    name_ = other_user.name_;
    password_ = other_user.password_;

    return *this;
}

//------------------------------------------------------------------------------

Abonent &Abonent::operator=(Abonent &&other_user) {
    if(this == &other_user)
        return *this;

    income_messages_ = other_user.income_messages_;
    unique_id_ = other_user.unique_id_;
    name_ = other_user.name_;
    password_ = other_user.password_;

    other_user.income_messages_.clear();
    other_user.unique_id_.clear();
    other_user.name_.clear();
    other_user.password_.clear();

    return *this;
}

//------------------------------------------------------------------------------

const std::string Abonent::generateUniqueID(){

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution <double> dist(0, MAX_USERS_COUNT);

        return std::string("id" + std::to_string(dist(mt)));
}

//------------------------------------------------------------------------------

void Abonent::addIncomeMsg(const Message& msg) {
    income_messages_.push_back(msg);
}

//------------------------------------------------------------------------------

bool Abonent::removeIncomeMsg(const Message& msg) {
    if(income_messages_.size() == 0)
    {
        std::cout << "FAIL: " << "Input message box for user \'"
                  << name_ << "\' is empty." << std::endl;
        return false;
    }

    auto messages_iter =
            find_if(income_messages_.begin(), income_messages_.end(),
                    [&msg](const Message& msg_in_vec){
                return msg_in_vec == msg; });

    if (messages_iter == income_messages_.end())
    {
        std::cout << "FAIL: Message not found.\n";
        return false;
    }

    income_messages_.erase(messages_iter);

    return true;
}

//------------------------------------------------------------------------------

const std::vector<Message> &Abonent::getIncomeMessages() const {
   return income_messages_;
}

//------------------------------------------------------------------------------

const std::string &Abonent::getUniqueID() const {
    return unique_id_;
}

//------------------------------------------------------------------------------

const std::string &Abonent::getName() const {
    return name_;
}

//------------------------------------------------------------------------------

const std::string &Abonent::getPass() const {
    return password_;
}

//------------------------------------------------------------------------------

bool Abonent::setName(const std::string& new_name){

    if(std::find_if(new_name.begin(), new_name.end(),
                    [](const char ch){
        return !std::isalnum(ch) || std::isspace(ch);
    }) != new_name.end())
        return false;

    name_ = new_name;
    return true;
}

//------------------------------------------------------------------------------

bool Abonent::setPassword(const std::string &new_pass) {
    if(std::find_if(new_pass.begin(), new_pass.end(),
                    [](const char ch){
        return !std::isalnum(ch) || std::isspace(ch);
    }) != new_pass.end())
        return false;

    password_ = new_pass;
    return true;
}

//------------------------------------------------------------------------------

bool Abonent::operator==(const Abonent &other_user) const {
    return this->unique_id_ == other_user.unique_id_ &&
        //this->name_ == other_user.name_ &&
            this->password_ == other_user.password_;
}

//------------------------------------------------------------------------------

std::ostream &operator<<(std::ostream& stream, const Abonent& user){
    return stream << user.unique_id_ << " " << user.name_
        << " " << user.password_;
}

//------------------------------------------------------------------------------
