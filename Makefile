default:	build


link = g++
gcc11 = g++ -std=c++11 -c -O2 -W -Wall


ALL_INCS = -I src/core src/middleware


CORE_DEPS = src/core/judge_light.h \
	src/core/jl_core.h \
	src/core/jl_cycle.h \
	src/middleware/jl_limit.h


build: judge_light


judge_light: src/core/judge_light.o \
	src/core/jl_cycle.o \
	src/middleware/jl_limit.o

	$(link) -o judge_light \
	src/core/judge_light.o \
	src/core/jl_cycle.o \
	src/middleware/jl_limit.o


src/core/judge_light.o: $(CORE_DEPS) \
	src/core/judge_light.cc
	$(gcc11) $(ALL_INCS) \
	-o src/core/judge_light.o \
	src/core/judge_light.cc


src/core/jl_cycle.o: $(CORE_DEPS) \
	src/core/jl_cycle.cc
	$(gcc11) $(ALL_INCS) \
	-o src/core/jl_cycle.o \
	src/core/jl_cycle.cc

src/middleware/jl_limit.o: $(CORE_DEPS) \
	src/middleware/jl_limit.cc
	$(gcc11) $(ALL_INCS) \
	-o src/middleware/jl_limit.o \
	src/middleware/jl_limit.cc

clean:
	rm -rf src/core/*.o
	rm -rf src/middleware/*.o
