XX = g++
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = watan
OBJECTS = goal.o main.o load.o criteria.o position.o util.o board.o subject.o observer.o tile.o dice.o factory.o textdisplay.o student.o loaded.o fair.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
.PHONY: clean
