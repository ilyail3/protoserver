//
// Created by ilya on 6/21/16.
//

#ifndef PROTO_SERVER_SEARCH_SAY_HELLO_H
#define PROTO_SERVER_SEARCH_SAY_HELLO_H


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


class SearchService_SayHello : public SearchService_Function {
public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    SearchService_SayHello(SearchService::AsyncService *service, ServerCompletionQueue *cq) : SearchService_Function(
            service, cq), responder_(&ctx_) {
    }

protected:
    virtual void processRequest() override;

    virtual SearchService_Function* initiate(SearchService::AsyncService *service, ServerCompletionQueue *cq) override {
        return new SearchService_SayHello(service, cq);
    }

    virtual void create(SearchService::AsyncService* service, ServerContext *ctx, ServerCompletionQueue *cq) override {
        service->RequestSayHello(ctx, &request_, &responder_, cq, cq, this);
    }

private:

    // What we get from the client.
    HelloRequest request_;
    // What we send back to the client.
    HelloReply reply_;

    // The means to get back to the client.
    ServerAsyncResponseWriter<HelloReply> responder_;

};


#endif //PROTO_SERVER_SEARCH_SAY_HELLO_H
