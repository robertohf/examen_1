TARGET=examen_1
all: ${TARGET}

${TARGET}: ast.o ${TARGET}_parser.cpp ${TARGET}_lexer.o main.o
	g++ --std=c++11 -g -o $@ $^

ast.o: ast.cpp ast.h
	g++ -std=c++11 -g -c -o $@ $<

main.o: main.cpp
	g++ --std=c++11 -g -c -o $@ $<

${TARGET}_lexer.o: ${TARGET}_lexer.cpp
	g++ --std=c++11 -g -c -o $@ $<

${TARGET}_lexer.cpp: ${TARGET}.l
	flex -d -o $@ $<

${TARGET}_parser.o: ${TARGET}_parser.cpp
	g++ --std=c++11 -g -c -o $@ $<

${TARGET}_parser.cpp: ${TARGET}.y
	bison --defines=tokens.h --debug -t -Wcounterexamples -o $@ $<

clean:
	rm -f *.o
	rm -rf ./${TARGET}.dSYM
	rm -f tokens.h
	rm -f ${TARGET}
	rm ${TARGET}_lexer.cpp
	rm ${TARGET}_parser.cpp