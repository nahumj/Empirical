# Flags to use regardless of compiler
CFLAGS_all := -Wall -Wno-unused-function -std=c++11 -I../../

# Emscripten compiler information
CXX_web := emcc
OFLAGS_web := -g4 -Werror -pedantic -Wno-dollar-in-identifier-extension -s TOTAL_MEMORY=67108864
#OFLAGS_web := -Werror -DNDEBUG -s TOTAL_MEMORY=67108864
#OFLAGS_web := -Oz -Werror -DNDEBUG -s TOTAL_MEMORY=67108864 -s ASSERTIONS=1
#OFLAGS_web := -O3 -Werror -DNDEBUG -s TOTAL_MEMORY=67108864


CFLAGS_web := $(CFLAGS_all) $(OFLAGS_web) --js-library ../../emtools/library_emp.js -s EXPORTED_FUNCTIONS="['_main', '_empCppCallback']" -s DISABLE_EXCEPTION_CATCHING=1 -s COMPILER_ASSERTIONS=1 -s NO_EXIT_RUNTIME=1
default: all
all: html5_events.js JSWrap.js

JSWrap.js: JSWrap.cc
	$(CXX_web) $(CFLAGS_web) JSWrap.cc -o JSWrap.js

html5_events.js: html5_events.cc
	$(CXX_web) $(CFLAGS_web) html5_events.cc -o html5_events.js

clean:
	rm -f html5_events.js JSWrap.js *.js.map *~

# Debugging information
#print-%: ; @echo $*=$($*)
print-%: ; @echo '$(subst ','\'',$*=$($*))'
