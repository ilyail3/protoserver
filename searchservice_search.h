//
// Created by ilya on 6/21/16.
//

#ifndef PROTO_SERVER_SEARCH_SEARCH_H
#define PROTO_SERVER_SEARCH_SEARCH_H


#include <iostream>
#include <grpc++/grpc++.h>
#include "gen/query.grpc.pb.h"
#include "searchservice_function.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;


class SearchService_Search : public SearchService_Function {
public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    SearchService_Search(SearchService::AsyncService *service, ServerCompletionQueue *cq) : SearchService_Function(
            service, cq), responder_(&ctx_) {
    }

protected:
    virtual void processRequest() override;

    virtual SearchService_Function* initiate(SearchService::AsyncService *service, ServerCompletionQueue *cq) override {
        return new SearchService_Search(service, cq);
    }

    virtual void create(SearchService::AsyncService* service, ServerContext *ctx, ServerCompletionQueue *cq) override {
        service->RequestSearch(ctx, &request_, &responder_, cq, cq, this);
    }

private:

    // What we get from the client.
    SearchRequest request_;
    // What we send back to the client.
    SearchResponse reply_;

    // The means to get back to the client.
    ServerAsyncResponseWriter<SearchResponse> responder_;

};


#endif // PROTO_SERVER_SEARCH_SEARCH_H
