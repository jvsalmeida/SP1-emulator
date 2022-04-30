#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../lib/main.h"


OctoB carry (short num) {
	if (num > 255 || num < 0)
        // binario - segundo bit
		return 2; 
	return 0;
}

// porque 255*255 da um numero maior do q "short" suporta
OctoB overflow (int num) { 
	if (num > 255 || num < 0)
        // binario - terceiro bit
		return 4; 
	return 0;
}

OctoB zeroAcc (short num) {
	if (num == 0)
        // binario - primeiro bit
		return 1; 
	return 0;
}


int main(int argc, const char *argv[]) {
	OctoB acc = 0;
	OctoB pc = 0;
	OctoB STAT = 0;

	instrucao numero[256];
	OctoB memory[256];
	OctoB buffer[2];
	
	FILE *fAssembly = fopen(argv[1], "rb");
	if (fAssembly == NULL) {
		printf("ERROR! opening file\n");
        return 1;
    }

    while (fread(buffer, sizeof(OctoB), 2, fAssembly)) {
		numero[pc].opcode = buffer[0];
		numero[pc].operando = buffer[1];
		++pc;
	}
	
	fclose(fAssembly);

	pc = 0;

	bool state = true;

	while (state) {
        short auxCarry = 0;
        int auxOverflow = 0;
        short zeroacc = 0;

        OctoB opcode = numero[pc].opcode;
		OctoB operando = numero[pc].operando;

		++pc;
		
		switch (opcode) {

            // LOAD MEMÓRIA
			case 0:
				STAT = 0;
				zeroacc = memory[operando];
				acc = memory[operando];
				STAT = zeroAcc(zeroacc);
				
				break;
            
            // LOAD VALOR
			case 1: 
				STAT = 0;
				zeroacc = operando;
				acc = operando;
				STAT = zeroAcc(zeroacc);

				break;
			
            // STORE
			case 2: 
				STAT = 0;
				zeroacc = acc;
				memory[operando] = acc;
				STAT = zeroAcc(zeroacc);

				break;
			
            // ADD                              // carry
			case 3: 
				STAT = 0;
				auxCarry = acc + memory[operando];
				acc = acc + memory[operando];
				STAT = carry(auxCarry) + STAT;
				STAT = zeroAcc(auxCarry) + STAT;

				break;
			
            // SUB                              // carry
			case 4: 
				STAT = 0;
				auxCarry = acc - memory[operando];
				acc = acc - memory[operando];
				STAT = carry(auxCarry) + STAT;
				STAT = zeroAcc(auxCarry) + STAT;

				break;
			
            // MUL                              // overflow
			case 5: 
				STAT = 0;
				auxOverflow = acc * memory[operando];                              
				acc = acc * memory[operando];
				STAT = overflow(auxOverflow) + STAT;
				STAT = zeroAcc(auxOverflow) + STAT;

				break;
			
            // DIV                              // overflow
			case 6: 
				STAT = 0;
				auxOverflow = acc / memory[operando];
				acc = acc / memory[operando];
				STAT = overflow(auxOverflow) + STAT;
				STAT = zeroAcc(auxOverflow) + STAT;

				break;
			
            // INC                              // carry
			case 7: 
				STAT = 0;
				auxCarry = acc + 1;
				acc = acc + 1;
				STAT = carry(auxCarry) + STAT;
				STAT = zeroAcc(auxCarry) + STAT;

				break;
			
            // DEC                              // carry
			case 8: 
				STAT = 0;
				auxCarry = acc - 1; 
				acc = acc - 1;
				STAT = carry(auxCarry) + STAT;
				STAT = zeroAcc(auxCarry) + STAT;

				break;
			
            // AND
			case 9: 
				STAT = 0;
				zeroacc = acc & memory[operando];
				acc = acc & memory[operando];
				STAT = zeroAcc(zeroacc);

				break;
			
            // OR
			case 10: 
				STAT = 0;
				zeroacc = acc | memory[operando]; 
				acc = acc | memory[operando];
				STAT = zeroAcc(zeroacc);

				break;

			// NOT
			case 11: 
				STAT = 0;
				zeroacc = ~acc;
				acc = ~acc;
				STAT = zeroAcc(zeroacc);

				break;
			
            // JMP
			case 12: 
				pc = (operando)/2;
				
				break;
			
            // JZ
			case 13: 
				if (STAT == 1 || STAT == 3 || STAT == 5 || STAT == 7)
					pc = (operando)/2;
			
				break;
			
            // JNZ
			case 14: 
				if (STAT != 1 && STAT != 3 && STAT != 5 && STAT != 7)
					pc = (operando)/2;
				
				break;
			
            // HLT
			case 15: 
				state = false;

				break;
																		
			default:
                printf("Unknown command\n");
				state = false;
                
                break;
        }
	}

    printf("ACC:  %03d\n", acc);
    printf("STAT: %03d\n", STAT);

	return 0;
}