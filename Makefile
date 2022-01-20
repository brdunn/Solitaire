all: check advance winnable game


game: game.opp readInputFile.opp singlyLinkedList.opp
	g++ game.opp readInputFile.opp singlyLinkedList.opp /usr/lib/libtermbox.a -o game

winnable: winnable.opp readInputFile.opp singlyLinkedList.opp movesSLL.opp cSwitchHashTable.opp
	g++ winnable.opp readInputFile.opp singlyLinkedList.opp movesSLL.opp cSwitchHashTable.opp -o winnable

advance: advance.opp readInputFile.opp singlyLinkedList.opp
	g++ advance.opp readInputFile.opp singlyLinkedList.opp -o advance

check: check.opp readInputFile.opp singlyLinkedList.opp
	g++ check.opp readInputFile.opp singlyLinkedList.opp -o check



readInputFile.opp: readInputFile.c readInputFile.h singlyLinkedList.h
	g++ -c readInputFile.c -o readInputFile.opp

check.opp: check.c readInputFile.h
	g++ -c check.c -o check.opp

advance.opp: advance.c advance.h readInputFile.h singlyLinkedList.h
	g++ -c advance.c -o advance.opp

singlyLinkedList.opp: singlyLinkedList.c singlyLinkedList.h
	g++ -c singlyLinkedList.c -o singlyLinkedList.opp

winnable.opp: winnable.cpp winnable.hpp singlyLinkedList.h readInputFile.h movesSLL.hpp
	g++ -c winnable.cpp -o winnable.opp

cSwitchHashTable.opp: cSwitchHashTable.cpp cSwitchHashTable.hpp singlyLinkedList.h readInputFile.h
	g++ -c cSwitchHashTable.cpp -o cSwitchHashTable.opp

movesSLL.opp: movesSLL.cpp movesSLL.hpp
	g++ -c movesSLL.cpp -o movesSLL.opp

game.opp: game.cpp game.hpp readInputFile.h
	g++ -I/share/cs327/include -c game.cpp -o game.opp

	

clean:
	rm *.opp
	make
