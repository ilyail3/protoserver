//
// Created by ilya on 6/21/16.
//

#include "searchservice_search.h"

void SearchService_Search::processRequest() {
    Result* result = reply_.add_results();

    result->set_title(request_.query() + std::string(" Result"));
    result->set_url(request_.query() + std::string(" Result"));


    responder_.Finish(reply_, Status::OK, this);
}