BUILD_DIR := gnu_build
GEN_DIR := ${BUILD_DIR}/gen

CC := @gcc
CFLAGS := -Iinclude -I${GEN_DIR} -g

SRCS := $(shell find src/ -name "*.c")
OBJS := $(addprefix $(BUILD_DIR)/, ${SRCS:.c=.o}) 
INCLUDE_FILES := $(shell find include -name "*.h")

GEN_LEX := $(GEN_DIR)/miniC.lex.c
GEN_YACC := $(GEN_DIR)/miniC.yacc.c
CPRE_BUILD := $(GEN_LEX) $(GEN_YACC) $(INCLUDE_FILES) 

ECHOS := @echo "\033[32m
ECHOE := \033[0m"

.PHONY : all test clean build_dir parser zip test

all : $(BUILD_DIR)/tinyC 
	cp $< a.out

$(BUILD_DIR)/tinyC : $(OBJS) $(CPRE_BUILD)
	$(ECHOS)Building tinyC... $(ECHOE)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
	$(ECHOS)Success! $(ECHOE)

build_dir : 
	$(ECHOS)Build Project on $(BUILD_DIR)$(ECHOE)
	@mkdir -p $(GEN_DIR) 
	@mkdir -p $(BUILD_DIR)/src/

$(BUILD_DIR)/%.o: %.c $(CPRE_BUILD)
	$(ECHOS)CXX $< $(ECHOE)
	$(CC) $(CFLAGS) $< -o $@ -c 

$(GEN_DIR)/%.lex.c : lex/%.l | build_dir
	$(ECHOS)FLEX $< $(ECHOE)
	flex -o $@ $<

$(GEN_DIR)/%.yacc.c : lex/%.y | build_dir 
	$(ECHOS)YACC $< $(ECHOE)
	yacc -o $@ $<

clean :
	@rm -rf $(BUILD_DIR)
	$(ECHOS)Done$(ECHOE)
	
parser : $(CPRE_BUILD)

zip : 
	git archive --format zip --output ../1400012705.zip master 

test : $(BUILD_DIR)/tinyC
	./$< < test/test.cpp 
