# CS327EvaBlake - Solitaire

Computer Science 327 Spring 2020 repository for Eva Kohl and Blake Dunn's group project

The group project for the Computer Science 327 spring 2020 semester is Solitaire.  

## Game Overview
* The tableau is the main playing area, comprising seven columns of cards. In the project specs, these
will be numbered from left to right as columns 1 through 7. Each column contains zero or more covered
cards (face down), followed by zero or more face up cards that must form a “pile”, where cards in a
tableau pile are subject to the rule: a card may only be placed on a card with rank one higher, and
opposite color. Additionally, if there are any covered cards, then they must be followed by at least one
uncovered card. Kings may be played on any empty column. 

* There are four foundations, one for each suit, that are built up in rank order starting from Aces (rank
1).

* The stock contains cards which may be played. These are turned over, either one at a time or three at
a time (depending on the variation of Klondike being played).

* The waste contains turned over cards from the stock. The top of the waste may be played onto the
tableau or one of the foundations, if such a move is legal.

For a more detailed overview of Klondike Solitaire, see the wikipedia page at https://en.wikipedia.org/wiki/Klondike_(solitaire).

## About This Implementation

## Part 1
The make executable has the ability to read a game setup from an input file and process that data accordingly.  The program can check for the validity of the input file and outputs whether the game setup file is valid.

## Part 2
The advance executable checks for the validity of an input file and then runs the moves specified in the file.  The number of moves that are to be run can be set with the -m switch, the output can be in the exchange format which allows it to be read into the check executable with the x switch, and the output can be sent to a specific file with the o switch.  Invalid moves are detected and are shown in the output.

## Part 3
Part 3 of the project allows a user to check if a game is winnable or not.  The winnable executable reads in an input file or from standard in.  The moves section of either input method is ignored.  After processing the data, whether or not the game can be won is printed to standard out.  The winnable executable has four swtiches: v, m, c, and f.  The m switch is followed with a number and that is that max number of moves in which the game should be won.  The v switch operates the executable in verbose mode which prints off more diagnostic data and therefore is a good tool to help with debugging.  The c switch uses a hash table as a cache to reduce execution time.  The f switch forces moves to the foundation and therefore can also reduce the time it takes the executable to win but it may also increase the number of moves to get to a winning state.

## Part 4
The 4th part of the project implement a text-based user interface for playing solataire.  The is implemented in the game executable.  The begining game state can either be read from a file in the exchange format (the move section is ignored) or it can be randomly generated using a seed value.  To run the game with a file, use the f switch followed by a filename.  To use the randomly generated game, use the s switch followed by a seed value and either the 1 or the 3 switch.  The seed game can optionally use the l switch followed by a number that indicates the number of times the stock can be reset.


Visit the DEVELOPERS file for more information about running the program.