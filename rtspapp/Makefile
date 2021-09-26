# export LD_LIBRARY_PATH=/usr/local/lib
CROSS_COMPILE	=
CXX   			= $(CROSS_COMPILE)g++
CC 				= $(CROSS_COMPILE)gcc

# PRJ_PATH 	= $(shell pwd)
PRJ_PATH = .
OBJS_PATH = $(PRJ_PATH)/objs
DEPS_PATH = $(OBJS_PATH)




C_FLAGS 	= 	-g $(I_FLAGS)


CXX_FLAGS 	= 	-g $(I_FLAGS)

LD_FLAGS 	= -lm 


EXE = main
# =====================
# 在这里添加源文件的文件夹
SRCS_PATH 	= 	$(PRJ_PATH) 
# $(PRJ_PATH)/testPrj

# =====================
# 下面不动
C_SOURCES 		= $(foreach dir,$(SRCS_PATH), $(wildcard $(dir)/*.c))
CXX_SOURCES 	= $(foreach dir,$(SRCS_PATH), $(wildcard $(dir)/*.cpp))
C_OBJECTS 		:= $(C_SOURCES:%.c=%.o)
C_OBJECTS 		:= $(addprefix $(OBJS_PATH)/,$(C_OBJECTS))
CXX_OBJECTS 	:= $(CXX_SOURCES:%.cpp=%.o)
CXX_OBJECTS 	:= $(addprefix $(OBJS_PATH)/,$(CXX_OBJECTS))
OBJS 			:= $(C_OBJECTS) $(CXX_OBJECTS)
DEPS 			:= $(OBJS:%.o=%.d)
MKDIR = @mkdir -p $(dir $@)

all: CREATE_DIR $(EXE)

CREATE_DIR : 
	@mkdir -p $(OBJS_PATH)
	@mkdir -p $(DEPS_PATH)


$(EXE): $(OBJS)
	$(CXX) $(OBJS) -o $(EXE) $(LD_FLAGS) $(CXX_FLAGS)

# 编译规则，不动
$(OBJS_PATH)/%.o : %.c
	$(MKDIR)
	@echo -e "\033[35m	CC	$(notdir $@)\033[0m"
	@$(CC) -c $(C_FLAGS) $< -o $@
$(OBJS_PATH)/%.o : %.cpp
	$(MKDIR)
	@echo -e "\033[35m	CXX	$(notdir $@)\033[0m"
	@$(CXX) -c $(CXX_FLAGS) $< -o $@

$(DEPS_PATH)/%.d: %.c
	$(MKDIR)
	@echo -e "\033[32m	CREATE	$(notdir $@)\033[0m"
	@rm -f $@; \
	 $(CC) -MM $(C_FLAGS) $< > $@.$$$$; \
     sed 's,\($(notdir $*)\)\.o[ :]*,$(OBJS_PATH)/$*\.o $@ : ,g' < $@.$$$$ > $@; \
     rm -f $@.$$$$
$(DEPS_PATH)/%.d: %.cpp
	$(MKDIR)
	@echo -e "\033[32m	CREATE	$(notdir $@)\033[0m"
	@rm -f $@; \
	 $(CXX) -MM $(CXX_FLAGS) $< > $@.$$$$; \
     sed 's,\($(notdir $*)\)\.o[ :]*,$(OBJS_PATH)/$*\.o $@ : ,g' < $@.$$$$ > $@; \
     rm -f $@.$$$$

ifeq ($(MAKECMDGOALS), clean)
else ifeq ($(MAKECMDGOALS), debug)
else
sinclude $(DEPS)
endif

.PHONY:clean
clean:
	rm -rf  $(OBJS_PATH) $(DEPS_PATH) $(EXE)

debug:
	@echo "============== C_SOURCES"
	@echo $(C_SOURCES)
	@echo "============== C_SOURCES"
	@echo  $(SRCS_PATH)/%.c
