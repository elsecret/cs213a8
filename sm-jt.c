#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int           reg[8];
unsigned char mem[10000 * 4];
int           pc = -1;
unsigned char insOpCode;
unsigned char insOp0;
unsigned char insOp1;
unsigned char insOp2;
char          insOpImm;
int           insOpExt;



void fetch() {
	insOpCode = mem[pc] >> 4;
	insOp0 = mem[pc] & 0xf;
	insOp1 = mem[pc + 1] >> 4;
	insOp2 = mem[pc + 1] & 0xf;
	insOpImm = mem[pc + 1];
	pc += 2;
	switch (insOpCode) {
	case 0x0:
	case 0xb:
		insOpExt = mem[pc] << 24 | mem[pc + 1] << 16 | mem[pc + 2] << 8 | mem[pc + 3];
		pc += 4;
		break;
	default:
		;
	}
}

int exec() {
	int	PRINT_IT = 0;
	static const void* opCodeJumpTable[16] =
	{
		&&OC_0,
		&&OC_1,
		&&OC_2,
		&&OC_3,
		&&OC_4,
		&&DEFAULT,
		&&OC_6,
		&&OC_7,
		&&OC_8,
		&&OC_9,
		&&OC_A,
		&&OC_B,
		&&OC_C,
		&&OC_D,
		&&OC_E,
		&&OC_F
	};

	static const void* AluJumpTable[9] = {
		&&ALU_0,
		&&ALU_1,
		&&ALU_2,
		&&ALU_3,
		&&ALU_4,
		&&ALU_5,
		&&ALU_6,
		&&ALU_7,
	};

	int cont = 1;
	int addr, val;
	goto *opCodeJumpTable[insOpCode];

	//switch (insOpCode) {
	//case 0x0: // ld $i, d .............. 0d-- iiii iiii
OC_0:
	if (PRINT_IT) printf("OpCode 0 \n");
	reg[insOp0] = insOpExt;
	//break;
	goto CONT;
	//case 0x1: // ld o(rs), rd .......... 1osd
OC_1:
	if (PRINT_IT) printf("OpCode 1 \n");
	addr = (insOp0 << 2) + reg[insOp1];
	reg[insOp2] = mem[addr] << 24 | mem[addr + 1] << 16 | mem[addr + 2] << 8 | mem[addr + 3];
	//break;
	goto CONT;
	//case 0x2: // ld (rs, ri, 2), rd .... 2sid
OC_2:
	if (PRINT_IT) printf("OpCode 2 \n");
	addr = reg[insOp0] + (reg[insOp1] << 2);
	reg[insOp2] = mem[addr] << 24 | mem[addr + 1] << 16 | mem[addr + 2] << 8 | mem[addr + 3];
	//break;
	goto CONT;
	//case 0x3: // st rs, o(rd) .......... 3sod
OC_3:
	if (PRINT_IT) printf("OpCode 3 \n");
	addr = (insOp1 << 2) + reg[insOp2];
	val = reg[insOp0];
	mem[addr] = val >> 24 & 0xff;
	mem[addr + 1] = val >> 16 & 0xff;
	mem[addr + 2] = val >> 8 & 0xff;
	mem[addr + 3] = val & 0xff;
	//break;
	goto CONT;
	//case 0x4: // st rs, (rd, ri, 4) .... 4sdi
OC_4:
	if (PRINT_IT) printf("OpCode 4 \n");
	addr = reg[insOp1] + (reg[insOp2] << 2);
	val = reg[insOp0];
	mem[addr] = val >> 24 & 0xff;
	mem[addr + 1] = val >> 16 & 0xff;
	mem[addr + 2] = val >> 8 & 0xff;
	mem[addr + 3] = val & 0xff;
	//break;
	goto CONT;

	//case 0x6: // ALU ................... 6-sd
OC_6:
	if (PRINT_IT) printf("OpCode 6 \n");
	// ------------------ START OF ALU INNER SWITCH STATEMENT ---------------------
	if (insOp0 == 0xf) goto ALU_F;
	//switch (insOp0) {
	if (insOp0 > 8) goto ALU_DEFAULT;
	goto *AluJumpTable[insOp0];
	//case 0x0: // mov rs, rd ........ 60sd
ALU_0:
	if (PRINT_IT) printf("ALU Op0 0 \n");
	reg[insOp2] = reg[insOp1];
	//break;
	goto ALU_CONT;
	//case 0x1: // add rs, rd ........ 61sd
ALU_1:
	if (PRINT_IT) printf("ALU Op0 1 \n");
	reg[insOp2] = reg[insOp1] + reg[insOp2];
	//break;
	goto ALU_CONT;
	//case 0x2: // and rs, rd ........ 62sd
ALU_2:
	if (PRINT_IT) printf("ALU Op0 2 \n");
	reg[insOp2] = reg[insOp1] & reg[insOp2];
	//break;
	goto ALU_CONT;
	//case 0x3: // inc rr ............ 63-r
ALU_3:
	if (PRINT_IT) printf("ALU Op0 3 \n");
	reg[insOp2] = reg[insOp2] + 1;
	//break;
	goto ALU_CONT;
	//case 0x4: // inca rr ........... 64-r
ALU_4:
	if (PRINT_IT) printf("ALU Op0 4 \n");
	reg[insOp2] = reg[insOp2] + 4;
	//break;
	goto ALU_CONT;
	//case 0x5: // dec rr ............ 65-r
ALU_5:
	if (PRINT_IT) printf("ALU Op0 5 \n");
	reg[insOp2] = reg[insOp2] - 1;
	//break;
	goto ALU_CONT;
	//case 0x6: // deca rr ........... 66-r
ALU_6:
	if (PRINT_IT) printf("ALU Op0 6 \n");
	reg[insOp2] = reg[insOp2] - 4;
	//break;
	goto ALU_CONT;
	//case 0x7: // not ............... 67-r
ALU_7:
	if (PRINT_IT) printf("ALU Op0 7 \n");
	reg[insOp2] = ~reg[insOp2];
	//break;
	goto ALU_CONT;
	//case 0xf: // gpc ............... 6f-r
ALU_F:
	if (PRINT_IT) printf("ALU Op0 F \n");
	reg[insOp2] = pc + (insOp1 << 1);
	//break;
	goto ALU_CONT;
	//default:
ALU_DEFAULT:
	printf("Illegal ALU instruction: pc=0x%x fun=0x%x\n", pc, insOp0);

ALU_CONT:
	//break;
	goto CONT;

	// ------------ END OF ALU INNER SWITCH STATEMENT ---------------------
	// ------------ OPCODE SWITCH STATEMENT CONTINUTED --------------------

OC_7:
	if (PRINT_IT) printf("OpCode 7 \n");
	//case 0x7: // sh? $i,rd ............. 7dii
	if (insOpImm > 0)
		reg[insOp0] = reg[insOp0] << insOpImm;
	else
		reg[insOp0] = reg[insOp0] >> -insOpImm;
	//break;
	goto CONT;
	//case 0x8: // br o .................. 8-oo
OC_8:
	if (PRINT_IT) printf("OpCode 8 \n");
	pc += insOpImm << 1;
	//break;
	goto CONT;
	//case 0x9: // beq rs, o ............. 9roo
OC_9:
	if (PRINT_IT) printf("OpCode 9 \n");
	if (reg[insOp0] == 0)
		pc += insOpImm << 1;
	//break;
	goto CONT;
	//case 0xa: // bgt rs, o .............. aroo
OC_A:
	if (PRINT_IT) printf("OpCode A \n");
	if (reg[insOp0] > 0)
		pc += insOpImm << 1;
	//break;
	goto CONT;
	//case 0xb: // j i ................... b--- iiii iiii
OC_B:
	if (PRINT_IT) printf("OpCode B \n");
	pc = insOpExt;
	//break;
	goto CONT;
	//case 0xc: // j o(rr) ............... croo
OC_C:
	if (PRINT_IT) printf("OpCode C \n");
	pc = (((unsigned short)insOpImm) << 1) + reg[insOp0];
	//break;
	goto CONT;
	//case 0xd: // j *o(rr) .............. droo
OC_D:
	if (PRINT_IT) printf("OpCode D \n");
	// TODO (done)
	addr = reg[insOp0] + (insOpImm << 2);
	//printf("The address is: ");
	//printf("%d\n", addr);
	pc = mem[addr] << 24 | mem[addr + 1] << 16 | mem[addr + 2] << 8 | mem[addr + 3];
	//break;
	goto CONT;
	//case 0xe: // j*(rr,ri,4) ............. eri-
OC_E:
	if (PRINT_IT) printf("OpCode E \n");
	// TODO (done)
	addr = reg[insOp0] + (reg[insOp1] << 2);
	pc = mem[addr] << 24 | mem[addr + 1] << 16 | mem[addr + 2] << 8 | mem[addr + 3];
	//break;
	goto CONT;
	//case 0xf: // halt or nop ............. f?--
OC_F:
	if (PRINT_IT) printf("OpCode F \n");
	if (insOp0 == 0)
		cont = 0;
	//break;
	goto CONT;
	//default:
DEFAULT:
	printf("Illegal  instruction: pc=0x%x opCode=0x%x\n", pc, insOpCode);
CONT:
	return cont;
}

int loadFile(char* filename) {
	char*  line = NULL;
	size_t len = 0;
	FILE* f = fopen(filename, "r");
	if (f == NULL)
		return 0;
	while (getline(&line, &len, f) > 0) {
		char* sep = line;
		while (*sep != ':')
			sep++;
		*sep = 0;
		int addr = strtol(line, 0, 16);
		if (pc < 0)
			pc = addr;
		line = sep + 1;
		char v[3] = { 0,0,0 };
		while (1) {
			while (*line && *line == ' ')
				line++;
			if (*line == 0 || *(line + 1) == 0)
				break;
			v[0] = line[0];
			v[1] = line[1];
			mem[addr] = strtol(v, 0, 16);
			line += 2;
			addr += 1;
		}
		line = NULL;
	}
	fclose(f);
	return 1;
}

void showMem(int start, int count) {
	int i;
	for (i = 0; i < count * 4; i += 4)
		printf("0x%08x: %02x %02x %02x %02x\n", start + i, mem[start + i], mem[start + i + 1], mem[start + i + 2], mem[start + i + 3]);
}

void compute() {
	int cont = 0;
	do {
		fetch();
		cont = exec();
	} while (cont);
}

struct showMemList {
	int  len;
	int* addr;
	int* count;
} showMemList;

int showReg;

int parseArgs(int argc, char** argv) {
	if (argc < 2)
		return 0;
	for (int i = 1; i < argc; i++)
		if (argv[i][0] == '-') {
			char* op = argv[i] + 1;
			if (strcmp(op, "p") == 0) {
				i += 1;
				pc = strtol(argv[i], 0, 16);
			}
			else if (strcmp(op, "m") == 0) {
				i += 1;
				char* sep = argv[i];
				while (*sep != ':')
					sep++;
				*sep = 0;
				showMemList.len++;
				showMemList.addr = realloc(showMemList.addr, showMemList.len * sizeof(*showMemList.addr));
				showMemList.addr[showMemList.len - 1] = strtol(argv[i], 0, 16);
				showMemList.count = realloc(showMemList.count, showMemList.len * sizeof(*showMemList.count));
				showMemList.count[showMemList.len - 1] = strtol(sep + 1, 0, 16);
			}
			else if (strcmp(op, "r") == 0)
				showReg = 1;
			else
				return 0;
		}
		else if (i != argc - 1)
			return 0;
	return 1;
}

int main(int argc, char** argv) {
	if (!parseArgs(argc, argv)) {
		fprintf(stderr, "usage: sm [-p startPC] [-m addr:count]* [-r] filename\n");
		return EXIT_FAILURE;
	}
	int ok = loadFile(argv[argc - 1]);
	if (!ok) {
		fprintf(stderr, "error reading input file\n");
		return EXIT_FAILURE;
	}
	compute();
	for (int i = 0; i < showMemList.len; i++)
		showMem(showMemList.addr[i], showMemList.count[i]);
	if (showReg)
		for (int i = 0; i < 8; i++)
			printf("r%d: 0x%08x (%d)\n", i, reg[i], reg[i]);
}
