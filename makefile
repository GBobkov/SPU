.PHONY: all Release Debug  сlean doxygen

all: main.exe

Release: main.exe

Debug: main.exe

сlean:
	rm $(DIROBJFILES)

OBJFILES_PROCESSOR := processor.o
OBJFILES_MY_STACK := my_stack.o stack_check.o
OBJFILES_SPU := spu.o spu_check.o
OBJFILES_ASSEMBLER := assembler.o
OBJFILES_MAIN := main.o
FLAGS := -Werror -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code  -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE -g

DIROBJFILES_MAIN := $(addprefix obj/, $(OBJFILES_MAIN))
DIROBJFILES_ASSEMBLER := $(addprefix obj/assembler/src/, $(OBJFILES_ASSEMBLER))
DIROBJFILES_SPU := $(addprefix obj/spu/src/, $(OBJFILES_SPU))
DIROBJFILES_MY_STACK := $(addprefix obj/my_stack/src/, $(OBJFILES_MY_STACK))
DIROBJFILES_PROCESSOR := $(addprefix obj/processor/src/, $(OBJFILES_PROCESSOR))
DIROBJFILES := $(DIROBJFILES_MAIN) $(DIROBJFILES_ASSEMBLER) $(DIROBJFILES_SPU) $(DIROBJFILES_MY_STACK) $(DIROBJFILES_PROCESSOR)

doxygen:
	doxygen
	
main.exe : $(DIROBJFILES)
	@g++ $(FLAGS) $(DIROBJFILES) -o $@


obj/%.o : %.cpp
	g++ $< -c  -o $@




