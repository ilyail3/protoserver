cmake_minimum_required(VERSION 3.5)
project(proto_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

link_libraries(grpc grpc++ protobuf)

file(GLOB GENERATED gen/*.h gen/*.cc)

set(SOURCE_FILES main.cpp ${GENERATED} searchservice_sayhello.cpp searchservice_sayhello.h searchservice_search.cpp searchservice_search.h searchservice_function.h)

# Copy credentials key and certificate
configure_file(ssl/domain.key domain.key COPYONLY)
configure_file(ssl/domain.crt domain.crt COPYONLY)

add_executable(proto_server ${SOURCE_FILES})