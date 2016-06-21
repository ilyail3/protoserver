
from grpc.beta import implementations
from gen import query_pb2
from os import path

_TIMEOUT_SECONDS = 30

DIR=path.dirname(path.dirname(path.abspath(__file__)))


creds = implementations.ssl_channel_credentials(
    open(path.join(DIR, "ssl/domain.crt")).read(),
    open(path.join(DIR, "ssl/client.pem")).read(),
    open(path.join(DIR, "ssl/client.crt")).read()
)
channel = implementations.secure_channel('localhost', 50051, creds)
stub = query_pb2.beta_create_SearchService_stub(channel)

search_request = query_pb2.SearchRequest()
search_request.query = "search"
search_request.page_number = 1
search_request.result_per_page = 30


print(stub.search(search_request, _TIMEOUT_SECONDS))
