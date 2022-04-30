#ifndef MAIN_H
# define MAIN_H

// 8 bits == 1 byte
typedef unsigned char OctoB; 

typedef struct instrucao {
	OctoB opcode;
	OctoB operando;
}instrucao;

#endif