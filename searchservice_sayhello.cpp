//
// Created by ilya on 6/21/16.
//

#include "searchservice_sayhello.h"

void SearchService_SayHello::processRequest() {
    reply_.set_msg(std::string("Hello ") + request_.name());

    responder_.Finish(reply_, Status::OK, this);
}