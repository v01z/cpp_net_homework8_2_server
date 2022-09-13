
#ifndef SERVER_HOMEWORK_8_ABONENT_H
#define SERVER_HOMEWORK_8_ABONENT_H

#include <iostream>
#include <vector>
#include <algorithm>

//------------------------------------------------------------------------------

constexpr size_t MAX_USERS_COUNT { 1000000 };

//------------------------------------------------------------------------------

struct Message{
    std::string from_;
    std::string text_;

    bool operator==(const Message&) const;
};

//------------------------------------------------------------------------------

class Abonent{
private:
    std::vector<Message> income_messages_;

    std::string unique_id_;
    std::string name_;
    std::string password_;

    const std::string generateUniqueID();

public:
    Abonent();
    Abonent(const std::string&, const std::string&, const std::string&);
    Abonent(const Abonent&);
    explicit Abonent(const std::string&);
    Abonent& operator=(const Abonent&);
    Abonent(Abonent&&) = delete;
    Abonent& operator=(Abonent&&);
    ~Abonent() = default;


    void addIncomeMsg(const Message&);

    bool removeIncomeMsg(const Message&);

    [[nodiscard]] const std::vector<Message> &getIncomeMessages() const;

    [[nodiscard]] const std::string &getUniqueID() const;
    [[nodiscard]] const std::string &getName() const;
    [[nodiscard]] const std::string &getPass() const;

    bool setName(const std::string&);
    bool setPassword(const std::string&);

    bool operator==(const Abonent&) const;

    friend std::ostream& operator<<(std::ostream&, const Abonent&);
};

//------------------------------------------------------------------------------

#endif //SERVER_HOMEWORK_8_ABONENT_H
