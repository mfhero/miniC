BUILD_DIR := build
GEN_DIR := ${BUILD_DIR}/gen

CC := @gcc
CXX := @g++ -std=c++11
CFLAGS := -Iinclude -I${GEN_DIR} -Iinclude/BuildEeyore -g

BE_SRCS := $(shell find src/BuildEeyore -name "*.c")
BE_OBJS := $(addprefix $(BUILD_DIR)/c/, ${BE_SRCS:.c=.o}) 
RA_SRCS := $(shell find src/RegAllocate/ -name "*.cpp")
RA_OBJS := $(addprefix $(BUILD_DIR)/cpp/, ${RA_SRCS:.cpp=.o})
RV_SRCS := $(shell find src/Riscv/ -name "*.cpp")
RV_OBJS := $(addprefix $(BUILD_DIR)/cpp/, ${RV_SRCS:.cpp=.o})
OBJS := $(BE_OBJS) $(RA_OBJS) $(RV_OBJS)

C_INCLUDE_FILES := $(shell find include -name "*.h")
CXX_INCLUDE_FILES := $(shell find include -name "*.hpp")

GEN_CXX := $(GEN_DIR)/Eeyore.lex.cpp $(GEN_DIR)/Eeyore.yacc.cpp $(GEN_DIR)/tigger.lex.cpp $(GEN_DIR)/tigger.yacc.cpp 
GEN_C := $(GEN_DIR)/miniC.lex.c $(GEN_DIR)/miniC.yacc.c 
CPRE_BUILD := $(C_INCLUDE_FILES) $(GEN_C)
CXX_PRE_BUILD := $(CXX_INCLUDE_FILES) $(GEN_CXX)

ECHOS := @echo "\033[32m
ECHOE := \033[0m"

.PHONY : all test clean build_dir parser zip test

all : $(BUILD_DIR)/CEeyore $(BUILD_DIR)/EeyoreTigger $(BUILD_DIR)/TiggerRiscv scripts/minicc.sh
	$(ECHOS)LINK a.out $(ECHOE)
	@cp $< a.out
	$(ECHOS)BUILD minicc $(ECHOE)
	@cat scripts/minicc.sh > minicc
	$(ECHOS)CHMOD minicc $(ECHOE)
	@chmod +x minicc

minicc : all

a.out : all

$(BUILD_DIR)/CEeyore : $(BE_OBJS) $(CPRE_BUILD)
	$(ECHOS)Building CEeyore... $(ECHOE)
	$(CC) $(CFLAGS) -o $@ $(BE_OBJS)
	$(ECHOS)Success! $(ECHOE)

$(BUILD_DIR)/EeyoreTigger : $(RA_OBJS) $(CXX_PRE_BUILD)
	$(ECHOS)Building EeyoreTigger... $(ECHOE)
	$(CXX) $(CFLAGS) -o $@ $(RA_OBJS) 
	$(ECHOS)Success! $(ECHOE)

$(BUILD_DIR)/TiggerRiscv : $(RV_OBJS) $(CXX_PRE_BUILD)
	$(ECHOS)Building TiggerRiscv... $(ECHOE)
	$(CXX) $(CFLAGS) -o $@ $(RV_OBJS) 
	$(ECHOS)Success! $(ECHOE)

build_dir: 
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

$(GEN_DIR)/%.lex.c : lex/%.l | build_dir
	$(ECHOS)FLEX $< $(ECHOE)
	@flex -o $@ $<

$(GEN_DIR)/%.lex.cpp : lex/%.l | build_dir
	$(ECHOS)FLEX $< $(ECHOE)
	@flex -o $@ $<



$(GEN_DIR)/%.yacc.c : lex/%.y | build_dir 
	$(ECHOS)YACC $< $(ECHOE)
	@yacc -o $@ $<

$(GEN_DIR)/%.yacc.cpp : lex/%.y | build_dir 
	$(ECHOS)YACC $< $(ECHOE)
	@bison -o $@ $< -d #-L c++


clean :
	@rm -rf $(BUILD_DIR)
	@rm a.out
	@rm minicc
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

testT : ./build/TiggerRiscv 
	$(ECHOS)check Riscv$(ECHOE)
	python scripts/score.py --task 3
