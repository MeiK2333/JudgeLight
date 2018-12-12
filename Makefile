default:	build


link = g++
gcc11 = g++ -std=c++11 -c -O2 -W -Wall


ALL_INCS = -I src/core \
	-I src/middleware


CORE_DEPS = src/core/judge_light.h \
	src/core/jl_core.h \
	src/core/jl_cycle.h \
	src/core/jl_data.h \
	src/middleware/jlm_file.h \
	src/middleware/jlm_limit.h \
	src/middleware/jlm_parser.h \
	src/middleware/jlm_result.h


build: judge_light


judge_light: src/core/judge_light.o \
	src/core/jl_core.o \
	src/core/jl_cycle.o \
	src/core/jl_data.o \
	src/middleware/jlm_file.o \
	src/middleware/jlm_limit.o \
	src/middleware/jlm_parser.o \
	src/middleware/jlm_result.o

	$(link) -o judge_light \
	src/core/judge_light.o \
	src/core/jl_core.o \
	src/core/jl_cycle.o \
	src/core/jl_data.o \
	src/middleware/jlm_file.o \
	src/middleware/jlm_limit.o \
	src/middleware/jlm_parser.o \
	src/middleware/jlm_result.o


src/core/judge_light.o: $(CORE_DEPS) \
	src/core/judge_light.cc
	$(gcc11) $(ALL_INCS) \
	-o src/core/judge_light.o \
	src/core/judge_light.cc


src/core/jl_core.o: $(CORE_DEPS) \
	src/core/jl_core.cc
	$(gcc11) $(ALL_INCS) \
	-o src/core/jl_core.o \
	src/core/jl_core.cc


src/core/jl_cycle.o: $(CORE_DEPS) \
	src/core/jl_cycle.cc
	$(gcc11) $(ALL_INCS) \
	-o src/core/jl_cycle.o \
	src/core/jl_cycle.cc

src/core/jl_data.o: $(CORE_DEPS) \
	src/core/jl_data.cc
	$(gcc11) $(ALL_INCS) \
	-o src/core/jl_data.o \
	src/core/jl_data.cc

src/middleware/jlm_limit.o: $(CORE_DEPS) \
	src/middleware/jlm_limit.cc
	$(gcc11) $(ALL_INCS) \
	-o src/middleware/jlm_limit.o \
	src/middleware/jlm_limit.cc

src/middleware/jlm_parser.o: $(CORE_DEPS) \
	src/middleware/jlm_parser.cc
	$(gcc11) $(ALL_INCS) \
	-o src/middleware/jlm_parser.o \
	src/middleware/jlm_parser.cc

src/middleware/jlm_file.o: $(CORE_DEPS) \
	src/middleware/jlm_file.cc
	$(gcc11) $(ALL_INCS) \
	-o src/middleware/jlm_file.o \
	src/middleware/jlm_file.cc

src/middleware/jlm_result.o: $(CORE_DEPS) \
	src/middleware/jlm_result.cc
	$(gcc11) $(ALL_INCS) \
	-o src/middleware/jlm_result.o \
	src/middleware/jlm_result.cc

clean:
	rm -rf src/core/*.o
	rm -rf src/middleware/*.o
