//
// Created by ilya on 6/21/16.
//

#include "searchservice_search.h"

Status SearchService_Search::process_search_request() {
    // The actual processing.
    std::string prefix("Hello ");
    //reply_.set_message(prefix + request_.query());


    Result *result = reply_.add_results();
    result->set_title(prefix + request_.query());
    result->set_url(prefix + request_.query());

    return Status::OK;
}