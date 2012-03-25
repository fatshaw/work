CC     = gcc
CXX    = g++
MKDIR = @mkdir -p
RM = @rm -rf

TARGET	   = apipoold_sh

OBJ_DIR	   = .obj

VPATH = src

# 下面加入各个组件
SVR_CPP_SRC = $(foreach dir,$(subst :, ,$(VPATH)),$(wildcard $(dir)/*.cpp))
SVR_CC_SRC=$(foreach dir,$(subst :, ,$(VPATH)),$(wildcard $(dir)/*.cc))
SVR_C_SRC = $(foreach dir,$(subst :, ,$(VPATH)),$(wildcard $(dir)/*.c))
SVR_CPP_OBJ =  $(addprefix $(OBJ_DIR)/,$(subst .cpp,.o, ${SVR_CPP_SRC}))
SVR_CC_OBJ =  $(addprefix $(OBJ_DIR)/,$(subst .cc,.o, ${SVR_CC_SRC}))
SVR_C_OBJ =  $(addprefix $(OBJ_DIR)/,$(subst .c,.o, ${SVR_C_SRC}))

SVR_OBJ = $(SVR_C_OBJ) $(SVR_CPP_OBJ) $(SVR_CC_OBJ)

SVR_PATH =  $(addprefix $(OBJ_DIR)/,$(subst :, ,$(VPATH)))
INC_PATH =  $(addprefix -I,$(subst :, ,$(VPATH))) 

INCLUDE_HOME = .

LDFLAGS	= -L./lib -lnet_manager.64k -L../../thirdparty/utils/src -L/usr/lib64 -L/usr/local/lib -lutils -lpthread -ljson -lprotobuf -lprotobuf -lprotobuf-lite -lprotoc
#LDFLAGS	= -L./lib -lnet_manager.64k -L../../thirdparty/utils/src -lutils -lpthread 
INC	=  -I. -I${INCLUDE_HOME}/include -I/usr/include/mysql -I../../thirdparty/net_manager/src -I../../thirdparty/utils/src -I../../thirdparty/jsoncpp/include ${INC_PATH} -I/usr/local/include

CPPFLAGS   = -g -Wall -Wno-invalid-offsetof $(INC)  -DLIST_TEST -DDISABLE_MEMPOOL -DDEBUG \
    -DMAX_PACKET_LENGTH=102400 -DMAX_NET_EVENT_COUNT=102400 -DMAX_NET_SEND_TASK_COUNT=102400
#-DMAX_PACKET_LENGTH=10000 -DMAX_NET_EVENT_COUNT=10000 -DMAX_NET_SEND_TASK_COUNT=10000
#CPPFLAGS   = -g -Wall -Wno-invalid-offsetof $(INC) -DLIST_TEST -DDISABLE_MEMPOOL
#CPPFLAGS   = -g -Wall -Wno-invalid-offsetof $(INC) -DMAX_PACKET_LENGTH=10000 -DMAX_NET_EVENT_COUNT=10000 -DMAX_NET_SEND_TASK_COUNT=10000 -DLIST_TEST -DDISABLE_MEMPOOL

all : objpath $(TARGET)
	
objpath : FORCE
	${MKDIR} $(SVR_PATH)

test : $(SVR_OBJ) 
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET) : $(SVR_OBJ) 
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o : %.cc
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o : %.c
	$(CC) $(CPPFLAGS) -c $< -o $@

clean: 
	${RM} $(SVR_OBJ) $(TARGET)

FORCE:
	
install: $(TARGET)
	mv $(TARGET) ./bin/$(TARGET)

build: $(TARGET)
	mv $(TARGET) ./bin/$(TARGET)
	cp -a ./bin/* ../build/trunk/apipoold/bin/

toolinstall:    
	cp -a ./tool/* ../build/trunk/apipoold/tool/

