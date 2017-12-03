BUILD_DIR := build
GEN_DIR := ${BUILD_DIR}/gen

CC := @gcc
CXX := @g++ -std=c++11
CFLAGS := -Iinclude -I${GEN_DIR} -Iinclude/BuildEeyore -g

C_SRCS := $(shell find src/ -name "*.c")
CXX_SRCS := $(shell find src/ -name "*.cpp")
C_OBJS := $(addprefix $(BUILD_DIR)/c/, ${C_SRCS:.c=.o}) 
CXX_OBJS := $(addprefix $(BUILD_DIR)/cpp/, ${CXX_SRCS:.cpp=.o})
OBJS := $(C_OBJS) $(CXX_OBJS)
C_INCLUDE_FILES := $(shell find include -name "*.h")
CXX_INCLUDE_FILES := $(shell find include -name "*.hpp")

GEN_CXX := $(GEN_DIR)/Eeyore.lex.cpp $(GEN_DIR)/Eeyore.yacc.cpp
GEN_LEX := $(GEN_DIR)/miniC.lex.c 
GEN_YACC := $(GEN_DIR)/miniC.yacc.c 
CPRE_BUILD := $(GEN_LEX) $(GEN_YACC) $(C_INCLUDE_FILES) 
CXX_PRE_BUILD := $(CXX_INCLUDE_FILES) $(GEN_CXX)

ECHOS := @echo "\033[32m
ECHOE := \033[0m"

.PHONY : all test clean build_dir parser zip test

all : $(BUILD_DIR)/CEeyore $(BUILD_DIR)/EeyoreTigger scripts/minicc.sh
	$(ECHOS)LINK a.out $(ECHOE)
	@cp $< a.out
	$(ECHOS)BUILD minicc $(ECHOE)
	@cat scripts/minicc.sh > minicc
	$(ECHOS)CHMOD minicc $(ECHOE)
	@chmod +x minicc

minicc : all

a.out : all

$(BUILD_DIR)/CEeyore : $(C_OBJS) $(CPRE_BUILD)
	$(ECHOS)Building CEeyore... $(ECHOE)
	$(CC) $(CFLAGS) -o $@ $(C_OBJS)
	$(ECHOS)Success! $(ECHOE)

$(BUILD_DIR)/EeyoreTigger : $(CXX_OBJS) $(CXX_PRE_BUILD)
	$(ECHOS)Building EeyoreTigger... $(ECHOE)
	$(CXX) $(CFLAGS) -o $@ $(CXX_OBJS) 
	$(ECHOS)Success! $(ECHOE)

build_dir : 
	$(ECHOS)Build Project on $(BUILD_DIR)$(ECHOE)
	@mkdir -p $(GEN_DIR) 
	@dirname $(OBJS) | xargs mkdir -p

$(BUILD_DIR)/cpp/%/main.o: %/main.cpp $(CXX_PRE_BUILD)
	$(ECHOS)CXX $< $(ECHOE)
	$(CXX) $(CFLAGS) $< -o $@ -c 

$(BUILD_DIR)/cpp/%.o: %.cpp $(CXX_INCLUDE_FILES)
	$(ECHOS)CXX $< $(ECHOE)
	$(CXX) $(CFLAGS) $< -o $@ -c 

$(BUILD_DIR)/c/%.o: %.c $(CPRE_BUILD)
	$(ECHOS)CC $< $(ECHOE)
	$(CC) $(CFLAGS) $< -o $@ -c 

#$(BUILD_DIR)/gen/%.o: $(GEN_DIR)/%.c $(CXX_CPRE_BUILD)
#	$(ECHOS)CC $< $(ECHOE)
#	$(CC) $(CFLAGS) $< -o $@ -c 

$(GEN_DIR)/%.lex.c : lex/%.l | build_dir
	$(ECHOS)FLEX $< $(ECHOE)
	flex -o $@ $<

$(GEN_DIR)/%.lex.cpp : lex/%.l | build_dir
	$(ECHOS)FLEX $< $(ECHOE)
	flex -o $@ $<



$(GEN_DIR)/%.yacc.c : lex/%.y | build_dir 
	$(ECHOS)YACC $< $(ECHOE)
	yacc -o $@ $<

$(GEN_DIR)/%.yacc.cpp : lex/%.y | build_dir 
	$(ECHOS)YACC $< $(ECHOE)
	bison -o $@ $< -d #-L c++


clean :
	@rm -rf $(BUILD_DIR)
	$(ECHOS)Done$(ECHOE)
	
parser : $(CPRE_BUILD)

tar : 
	git archive --format tar --output ../1400012705.tar master 

testC : ./build/CEeyore 
	$(ECHOS)check Eeyore$(ECHOE)
	python scripts/score.py --task 1

testE : ./build/EeyoreTigger  
	$(ECHOS)check Tigger$(ECHOE)
	python scripts/score.py --task 2
