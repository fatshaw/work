PROJECT(sqlmessagetest)
SET(SRC_LIST main.cpp SqlSingleQuery.pb.cc SqlSingleResult.pb.cc)
INCLUDE_DIRECTORIES(/usr/local/include)
LINK_DIRECTORIES(/usr/local/lib)
ADD_EXECUTABLE(sqlmessagetest ${SRC_LIST})
TARGET_LINK_LIBRARIES( sqlmessagetest
    protobuf protobuf-lite protoc pthread)
