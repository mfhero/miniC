BUILD_DIR := build
GEN_DIR := ${BUILD_DIR}/gen

CC := @gcc
CXX := @g++
CFLAGS := -Iinclude -I${GEN_DIR} -g

SRCS := $(shell find src/ -name "*.c")
C_OBJS := $(addprefix $(BUILD_DIR)/c/, ${SRCS:.c=.o}) 
CXX_OBJS := $(addprefix $(BUILD_DIR)/cpp/, ${SRCS:.cpp=.o})
OBJS := $(C_OBJS) $(CXX_OBJS)
INCLUDE_FILES := $(shell find include -name "*.h" -name "*.hpp")

GEN_LEX := $(GEN_DIR)/miniC.lex.c
GEN_YACC := $(GEN_DIR)/miniC.yacc.c
CPRE_BUILD := $(GEN_LEX) $(GEN_YACC) $(INCLUDE_FILES) 

ECHOS := @echo "\033[32m
ECHOE := \033[0m"

.PHONY : all test clean build_dir parser zip test

all : $(BUILD_DIR)/CEeyore 
	cp $< a.out

a.out : all

$(BUILD_DIR)/CEeyore : $(C_OBJS) $(CPRE_BUILD)
	$(ECHOS)Building tinyC... $(ECHOE)
	$(CC) $(CFLAGS) -o $@ $(C_OBJS)
	$(ECHOS)Success! $(ECHOE)

build_dir : 
	$(ECHOS)Build Project on $(BUILD_DIR)$(ECHOE)
	@mkdir -p $(GEN_DIR) 
	@dirname $(OBJS) | xargs mkdir -p

$(BUILD_DIR)/cpp/%.o: %.cpp $(CPRE_BUILD)
	$(ECHOS)CXX $< $(ECHOE)
	$(CXX) $(CFLAGS) $< -o $@ -c 

$(BUILD_DIR)/c/%.o: %.c $(CPRE_BUILD)
	$(ECHOS)CC $< $(ECHOE)
	$(CC) $(CFLAGS) $< -o $@ -c 


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
	bison -o $@ $< -d -L c++


clean :
	@rm -rf $(BUILD_DIR)
	$(ECHOS)Done$(ECHOE)
	
parser : $(CPRE_BUILD)

tar : 
	git archive --format tar --output ../1400012705.tar master 

test : a.out
	python test/score.py
