CPPFLAGS=-std=c++11 -Wall -O2 -l pthread

all: maxcut revmax_nm preproc er ba

maxcov: src/main.cpp src/mygraph.cpp src/algs.cpp src/obj.cpp
	g++ src/main.cpp -o maxcov  ${CPPFLAGS} -DMAXCOV
maxcut: src/main.cpp src/mygraph.cpp src/algs.cpp src/obj.cpp
	g++ src/main.cpp -o maxcut  ${CPPFLAGS} -DMAXCUT
revmax: src/main.cpp src/mygraph.cpp src/algs.cpp
	g++ src/main.cpp -o revmax  ${CPPFLAGS} -DREVMAX_MON
revmax_nm: src/main.cpp src/mygraph.cpp src/algs.cpp
	g++ src/main.cpp -o revmax_nm  ${CPPFLAGS} -DREVMAX_NM
debug: src/main.cpp src/mygraph.cpp src/algs.cpp
	g++ src/main.cpp -o maxcov_debug -std=c++11 -Wall -Og -g -DMAXCOV
maxcut_debug: src/main.cpp src/mygraph.cpp src/algs.cpp src/obj.cpp
	g++ src/main.cpp -o maxcut_debug -std=c++11 -Wall -Og -g -DMAXCUT -l pthread
revmax-debug: src/main.cpp src/mygraph.cpp src/algs.cpp
	g++-4.9 src/main.cpp -o revmax-debug -std=c++11 -Wall -Og -g  -DREVMAX_MON
preproc: src/preprocess.cpp src/mygraph.cpp
	g++ src/preprocess.cpp -o preproc  ${CPPFLAGS}
er: src/gen_er.cpp src/mygraph.cpp
	g++ -std=c++11 src/gen_er.cpp -o er
ba: src/gen_ba.cpp src/mygraph.cpp
	g++ -std=c++11 src/gen_ba.cpp -o ba
clean:
	rm -f preproc
	rm -f maxcov
	rm -f revmax
single-pass: maxcut preproc
	cd exp/$@; make DATA=er BIN=maxcut; make DATA=er BIN=revmax; make DATA=ba BIN=maxcut; make DATA=ba BIN=revmax; make DATA=fb BIN=maxcut; make DATA=fb BIN=revmax; make DATA=slashdot BIN=maxcut; make DATA=slashdot BIN=revmax; make DATA=pokec BIN=maxcut; make DATA=pokec BIN=revmax
