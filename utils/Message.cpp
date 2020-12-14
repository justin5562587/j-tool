//
// Created by justin on 2020/11/23.
//

#include "Message.h"

Message::Message(std::string &message) : message(message) {

}

//Message::Message(const Message &message1) : message(message1.getMessage()) {
//}
//
//Message& Message::operator=(const Message & message1) {
//    message = message1.getMessage();
//    return this;
//}

void Message::console() {
    std::cout << message << std::endl;
}

void Message::setMessage(std::string &newMessage) {
    message = newMessage;
}

std::string &Message::getMessage() const {
    return message;
}

//    QDirIterator it(":/", QDirIterator::Subdirectories);
//    while (it.hasNext()) {
//        qDebug() << it.next();
//    }
