link_libraries(${AC_RESEARCH_LIB} ${ACBASE_LIB} ${LIBCONFIG_LIB} ${CONFIG_LIB}
${LIBEVENT_LIB} protobuf pthread protobuf protobuf-lite protoc
boost_thread  boost_filesystem  boost_program_options json utils mongoclient.a  memcached)
SET(EXECUTABLE_OUTPUT_PATH ../bin)
add_definitions(-DGINX_VERSION=0.1.1) 
add_executable(stateserver
main.cpp
memcachepool.cpp
mongo.cpp
memcachedst.cpp
stateserverservice.cpp
acceptormanager.cpp
msg/stateServerMsg.cpp
msg/pmsg/tokenMessage.pb.cc
msganalyser/stateServerAnalyser.cpp
msganalyser/sessionAnalyser.cpp
msganalyser/msgAnalyserUtil.cpp
msganalyser/mongoUtil.cpp
msganalyser/memcachedUtil.cpp
ginxutil.cpp)
