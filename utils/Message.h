//
// Created by justin on 2020/11/23.
//

#ifndef J_TOOL_MESSAGE_H
#define J_TOOL_MESSAGE_H

#include <iostream>
#include <string>

class Message {

public:

    Message(std::string &message);

//    Message(const Message &);
//
//    Message &operator=(const Message &);

    void console();

    void setMessage(std::string &newMessage);

    std::string &getMessage() const;

private:

    std::string &message;

};


#endif //J_TOOL_MESSAGE_H
