CC     = gcc
CXX    = g++
MKDIR = @mkdir -p
RM = @rm -rf

TARGET	   = phpfcgiagentd

OBJ_DIR	   = .obj

VPATH = src

# 下面加入各个组件
SVR_CPP_SRC = $(foreach dir,$(subst :, ,$(VPATH)),$(wildcard $(dir)/*.cpp))
SVR_C_SRC = $(foreach dir,$(subst :, ,$(VPATH)),$(wildcard $(dir)/*.c))
SVR_CPP_OBJ =  $(addprefix $(OBJ_DIR)/,$(subst .cpp,.o, ${SVR_CPP_SRC}))
SVR_C_OBJ =  $(addprefix $(OBJ_DIR)/,$(subst .c,.o, ${SVR_C_SRC}))

SVR_OBJ = $(SVR_C_OBJ) $(SVR_CPP_OBJ)

SVR_PATH =  $(addprefix $(OBJ_DIR)/,$(subst :, ,$(VPATH)))
INC_PATH =  $(addprefix -I,$(subst :, ,$(VPATH))) 

INCLUDE_HOME = .

LDFLAGS	= -l rt -L/usr/lib64/mysql -lmysqlclient -L./lib -lnet_manager.64k -L../../thirdparty/utils/src -lutils -lpthread
#LDFLAGS	= -L/usr/lib/mysql -lmysqlclient -L../../thirdparty/net_manager/src/ -lnet_manager.64k -lpthread
INC	=  -I. -I${INCLUDE_HOME}/include -I/usr/include/mysql -I../../thirdparty/ -I../../thirdparty/utils/src ${INC_PATH}

CPPFLAGS   = -g -Wall $(INC) -Wno-invalid-offsetof -DMAX_PACKET_LENGTH=102400 -DMAX_NET_EVENT_COUNT=102400 -DMAX_NET_SEND_TASK_COUNT=102400
#CPPFLAGS   = -g -Wall $(INC) -DMAX_PACKET_LENGTH=100000 -DMAX_NET_EVENT_COUNT=10240 -DMAX_NET_SEND_TASK_COUNT=10240 -DTEST

all : objpath $(TARGET)
	
objpath : FORCE
	${MKDIR} $(SVR_PATH)

test : $(SVR_OBJ) 
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET) : $(SVR_OBJ) 
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o : %.c
	$(CC) $(CPPFLAGS) -c $< -o $@

clean: 
	${RM} $(SVR_OBJ) $(TARGET)

FORCE:
	
install: $(TARGET)
	mv $(TARGET) /opt/app/server/server/phpfcgiagentd/bin/

build: $(TARGET)
	mv $(TARGET) ./bin/$(TARGET)
	cp -a ./bin/* ../build/bin/

toolinstall:    
	cp -a ./tool/* ../build/tool/
