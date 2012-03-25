cmake_minimum_required(VERSION 2.6)

SET(DEV_COMMON_HOME /home/xcj/server/framework)

SET(GINX_VERSION 0.0.1)


SET(LIBRARY_OUTPUT_PATH lib)
SET(CMAKE_AR ar)

FIND_PATH(ACBASE_HOME ac/util/fixedstring.h ${DEV_COMMON_HOME}/base /usr/local/include NO_DEFAULT_PATH)
FIND_PATH(ACBASE_INC ac/network/reactor.h ${ACBASE_HOME} /usr/local/include NO_DEFAULT_PATH)

FIND_PATH(ACRESEARCH_HOME ac_actor/actor.h ${DEV_COMMON_HOME}/research /usr/local/include NO_DEFAULT_PATH)
FIND_PATH(ACRESEARCH_INC ac_actor/actor.h ${ACRESEARCH_HOME} /usr/local/include NO_DEFAULT_PATH)

FIND_PATH(BDB_INC db.h /usr/include /usr/local/BerkeleyDB.4.5/include /usr/local/include/bdb ${DEV_COMMON_HOME}/BerkeleyDB.4.5/include NO_DEFAULT_PATH)
FIND_PATH(EVENT_INC event.h /usr/include /usr/local/include/event ${DEV_COMMON_HOME}/libevent-1.3e/include NO_DEFAULT_PATH)

FIND_LIBRARY(AC_RESEARCH_LIB ac_research ${ACRESEARCH_HOME}/lib  NO_DEFAULT_PATH)
FIND_LIBRARY(ACBASE_LIB ac ${ACBASE_HOME}/lib  NO_DEFAULT_PATH)

FIND_LIBRARY(CONFIG_LIB config  ${ACBASE_HOME}/lib NO_DEFAULT_PATH)
SET(LIBCONFIG_LIB ${CONFIG_LIB})
message(STATUS ${CONFIG_LIB})
FIND_LIBRARY(BDB_LIB db /usr/lib /usr/local/lib /usr/local/lib/bdb /app/dev/common/BerkeleyDB.4.5/lib NO_DEFAULT_PATH)
FIND_LIBRARY(PTHREAD_LIB pthread /usr/lib /usr/local/lib NO_DEFAULT_PATH)
FIND_LIBRARY(RUNTIME_LIB rt /usr/lib  /usr/local/lib NO_DEFAULT_PATH)
FIND_LIBRARY(LIBEVENT_LIB event  /usr/lib /usr/local/lib  NO_DEFAULT_PATH)
FIND_LIBRARY(BZ2_LIB bz2 /usr/lib /usr/local/lib NO_DEFAULT_PATH)
LINK_DIRECTORIES(/home/xcj/mongo-driver /usr/local/lib /usr/lib64 /home/xcj/server/thirdparty/utils/src /home/xcj/server/apipoold/trunk/lib)
INCLUDE_DIRECTORIES(. ${ACRESEARCH_INC} ${ACBASE_INC} ${BDB_INC} ${EVENT_INC} /home/xcj/mongo-driver/mongo/client /home/xcj/server/thirdparty/utils/src  /home/xcj/server/thirdparty/jsoncpp/include )


ADD_DEFINITIONS(
 -O3 -g -W -Wall
 -Wunused-variable -Wunused-parameter -Wunused-function -Wunused
 -Wno-deprecated -Woverloaded-virtual -Wwrite-strings
 -D_REENTRANT -DCORO_UCONTEXT
 -DAC_HAS_INFO -DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DAC_HAS_STATISTICIAN
)

ADD_DEFINITIONS(-DAC_HAS_DEBUG -D_AC_HAS_DEBUG )

SUBDIRS(src test unittest)
