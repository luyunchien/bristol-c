#in my codes, I had the interpreter code mixed-in with the parser code.
#This additional code is protected by #ifdef INTERP / #endif clauses.
#This code is only "seen" by the compiler if you have #defined INTERP
#This #define would be annoying to change inside nlab.c/nlab.h so inside it is
#set in the gcc/clang statement using -DINTERP
#in this way ./parse & ./interp can both be build form the same source file.

CFLAGS := -Wall -Wextra -Wpedantic -Wfloat-equal -Wvla -std=c99
SANI := -fsanitize=address -fsanitize=undefined
NLBS := $(wildcard *.nlb)
RESULTS := $(NLBS:.nlb=.result)

all : parse parse_s parse_v interp interp_s interp_v extension

interp: nlab.c
		gcc nlab.c $(CFLAGS) -O2 -DINTERP -o interp -lm

interp_s: nlab.c
		gcc nlab.c $(CFLAGS) $(SANI) -g3 -DINTERP -o interp_s -lm

interp_v: nlab.c
		gcc nlab.c $(CFLAGS) -g3 -DINTERP -o interp_v -lm

parse: nlab.c
		gcc nlab.c $(CFLAGS) -O2 -o parse -lm

parse_s: nlab.c
		gcc nlab.c $(CFLAGS) $(SANI) -g3 -o parse_s -lm

parse_v: nlab.c
		gcc nlab.c $(CFLAGS) -g3 -o parse_v -lm

extension: extension.c
		gcc extension.c $(CFLAGS) -O2 -DINTERP -o extension -lm

# For all .nlb files, run them and output result to a .result file
runall : $(RESULTS)

%.result:
		./interp $*.nlb > $*.results

clean:
		rm -f parse parse_s parse_v interp interp_s interp_v $(RESULTS)
