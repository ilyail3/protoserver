#include <iostream>

#include <grpc++/grpc++.h>
#include <fstream>

#include "gen/query.grpc.pb.h"
#include "searchservice_sayhello.h"
#include "searchservice_search.h"


using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;


class ServerImpl final {
public:
    ~ServerImpl() {
        server_->Shutdown();
        // Always shutdown the completion queue after the server.
        cq_->Shutdown();
    }

    // There is no shutdown handling in this code.
    void Run() {
        std::string server_address("0.0.0.0:50051");

        ServerBuilder builder;




        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(server_address, sslCred());
        // Register "service_" as the instance through which we'll communicate with
        // clients. In this case it corresponds to an *asynchronous* service.
        builder.RegisterService(&service_);
        // Get hold of the completion queue used for the asynchronous communication
        // with the gRPC runtime.
        cq_ = builder.AddCompletionQueue();
        // Finally assemble the server.
        server_ = builder.BuildAndStart();
        std::cout << "Server listening on " << server_address << std::endl;

        // Proceed to the server's main loop.
        HandleRpcs();
    }

private:
    std::string get_content(const char* filename) {
        std::ifstream ifs(filename);
        std::string content(
                (std::istreambuf_iterator<char>(ifs)),
                            (std::istreambuf_iterator<char>())
        );

        return content;
    }

    std::shared_ptr<grpc::ServerCredentials> sslCred(){
        grpc::SslServerCredentialsOptions::PemKeyCertPair pkcp ={
                get_content("domain.key"),
                get_content("domain.crt")
        };

        grpc::SslServerCredentialsOptions ssl_opts;
        ssl_opts.pem_root_certs="";
        ssl_opts.pem_key_cert_pairs.push_back(pkcp);
        ssl_opts.force_client_auth = true;
        return grpc::SslServerCredentials(ssl_opts);
    };

    // This can be run in multiple threads if needed.
    void HandleRpcs() {
        // Spawn a new CallData instance to serve new clients.
        (new SearchService_SayHello(&service_, cq_.get()))->Proceed();
        (new SearchService_Search(&service_, cq_.get()))->Proceed();

        void* tag;  // uniquely identifies a request.
        bool ok;
        while (true) {
            // Block waiting to read the next event from the completion queue. The
            // event is uniquely identified by its tag, which in this case is the
            // memory address of a CallData instance.
            // The return value of Next should always be checked. This return value
            // tells us whether there is any kind of event or cq_ is shutting down.
            GPR_ASSERT(cq_->Next(&tag, &ok));
            GPR_ASSERT(ok);
            static_cast<SearchService_Function*>(tag)->Proceed();
        }
    }


    std::unique_ptr<ServerCompletionQueue> cq_;
    SearchService::AsyncService service_;
    std::unique_ptr<Server> server_;
};

int main(int argc, char** argv) {
    ServerImpl server;
    server.Run();

    return 0;
}