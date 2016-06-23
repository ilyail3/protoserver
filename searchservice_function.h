//
// Created by ilya on 6/23/16.
//

#pragma once

#include <iostream>
#include <grpc++/grpc++.h>
#include "gen/query.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;



class SearchService_Function {
public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    SearchService_Function(SearchService::AsyncService* service, ServerCompletionQueue* cq) :
        service_(service), cq_(cq), status_(CREATE) {
            // Invoke the serving logic right away.

    }

    void Proceed() {
        if (status_ == CREATE) {
            // Make this instance progress to the PROCESS state.
            status_ = PROCESS;

            // As part of the initial CREATE state, we *request* that the system
            // start processing SayHello requests. In this request, "this" acts are
            // the tag uniquely identifying the request (so that different CallData
            // instances can serve different requests concurrently), in this case
            // the memory address of this CallData instance.
            create(service_, &ctx_, cq_);
        } else if (status_ == PROCESS) {
            // Spawn a new CallData instance to serve new clients while we process
            // the one for this CallData. The instance will deallocate itself as
            // part of its FINISH state.
            initiate(service_, cq_)->Proceed();

            // And we are done! Let the gRPC runtime know we've finished, using the
            // memory address of this instance as the uniquely identifying tag for
            // the event.
            status_ = FINISH;

            processRequest();
        } else {
            GPR_ASSERT(status_ == FINISH);
            // Once in the FINISH state, deallocate ourselves (CallData).
            delete this;
        }
    }

protected:
    virtual void processRequest() = 0;

    virtual SearchService_Function* initiate(SearchService::AsyncService* service, ServerCompletionQueue* cq) = 0;
    virtual void create(SearchService::AsyncService* service, ServerContext* ctx, ServerCompletionQueue* cq) = 0;

    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    ServerContext ctx_;

private:
    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    SearchService::AsyncService* service_;
    // The producer-consumer queue where for asynchronous server notifications.
    ServerCompletionQueue* cq_;

    // Let's implement a tiny state machine with the following states.
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;  // The current serving state.

};
