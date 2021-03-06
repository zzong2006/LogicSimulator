#pragma once
#define MAX(a,b) (a) >= (b) ? (a) : (b)
#define MIN(a,b) (a) <= (b) ? (a) : (b)

#define AND_GATE					20000
#define OR_GATE					20001
#define NOT_GATE					20002
#define NAND_GATE					20003
#define NOR_GATE					20004
#define XOR_GATE					20005

#define PIN							20010
#define CLOCK						20011
#define OUTPIN						20012
#define SEG7						20013

#define D_FF						20030
#define JK_FF						20031
#define T_FF						20032

#define EAST						777
#define SOUTH						778
#define WEST						779
#define NORTH						780

#define LIB							11100
#define BOX							11000
#define MAIN_LIB					12000
#define SUB_LIB					12010

#define GATE_TYPE					30000
#define WIRING_TYPE				30001
#define FLIPFLOP_TYPE			30002

#define OFF_SIGNAL				0
#define ON_SIGNAL					1

#define LINE						100
#define OBJECT						101
#define ALL							111

#define MAX_UNDO					10
#define MAX_REDO					10

#define VERTICAL					0
#define HORIZONTAL				1

#define NEW							0
#define REMOVE						1
#define MOVE						2
#define COPY						3
#define PASTE						4