/*****************************************************************
 * BUAA Fall 2021 Fundamentals of Computer Hardware
 * Project7 Assembler and Linker
 *****************************************************************
 * my_assembler_utils.c
 * Assembler Submission
 *****************************************************************/
#include "my_assembler_utils.h"
#include "assembler-src/assembler_utils.h"
#include "lib/translate_utils.h"

#include <string.h>
#include <stdlib.h>

/*
 * This function reads .data symbol from INPUT and add them to the SYMTBL
 * Note that in order to distinguish in the symbol table whether a symbol
 * comes from the .data segment or the .text segment, we append a % to the
 * symbol name when adding the .data segment symbol. Since only underscores and
 * letters will appear in legal symbols, distinguishing them by adding % will
 * not cause a conflict between the new symbol and the symbol in the assembly file.
 *
 * Return value:
 * Return the number of bytes in the .data segment.
 */
char* IGNORE_CHARS_RDS = " \f\n\r\t\v,():#";
#define lineN 100050
char line[lineN];

int read_data_segment(FILE *input, SymbolTable *symtbl) {	
	printf("Going into read_data_segment\n") ;
	int bytes = 0;
		
	fgets(line, lineN, input);
	if (strcmp(strtok(line, IGNORE_CHARS_RDS), ".data") != 0) {
        return -1;
    }

	char constChar[5] = "%";
	char name[lineN] = "%";
	fgets(line, lineN, input);
	skip_comment(line);
	while( strcmp(line, "\n") != 0 ){
		char *p = strtok(line, IGNORE_CHARS_RDS);
		int findName = 0, getSize = 0;
		int addErr = 0;
		while(p != NULL) {
			if(findName == 1 && getSize == 1)	break;
			
			if(*p >= 48 && *p <= 57 && getSize == 0){
				long int size = 0;
				//puts(p);
				int numErr = translate_num(&size, p, 0, 1e9);
				getSize = 1;
				if(numErr == 0){
					if(addErr == 0)
						bytes += size;
					else
						return -1;
				}
					
				printf("size: %d\n", size); 
			}
			else if(findName == 0){
				strcat(name, p);
				printf("name: %s\n", p);
				addErr = add_to_table(symtbl, name, bytes);
				//printf("addErr: %d\n", addErr);
				findName = 1; 
			}
			p = strtok(NULL, IGNORE_CHARS_RDS);
		}
		strcpy(name, constChar);
		fgets(line, lineN, input);
		skip_comment(line);
	}
	printf("Total bytes:%d\nEnd of read_data_segment\n", bytes);
	if(bytes == 0)
		return -1;
	else
    	return bytes;
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE.
 * ADDR is given as the byte offset from the first instruction. The SymbolTable
 * must be able to resize itself as more elements are added.
 *
 * Note that NAME may point to a temporary array, so it is not safe to simply
 * store the NAME pointer. You must store a copy of the given string.
 *
 * If ADDR is not word-aligned, you should call addr_alignment_incorrect() and
 * return -1. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists
 * in the table, you should call name_already_exists() and return -1. If memory
 * allocation fails, you should call allocation_failed().And alloction_failed()
 * will print error message and exit with error code 1.
 *
 * Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable *table, const char *name, uint32_t addr) {
	if( (table->mode) == SYMTBL_UNIQUE_NAME && get_addr_for_symbol(table, name) != -1){
		name_already_exists(name);
		return -1;
	}

	if(addr % 4 != 0){
		addr_alignment_incorrect();
		return -1;
	}

	char* nameCpy;
	nameCpy = create_copy_of_str(name);
	Symbol tmp;
	tmp.name = nameCpy, tmp.addr = addr;
	if(table->len == table->cap){
		Symbol* newPtr = (Symbol*) realloc(table->tbl, table->cap*sizeof(Symbol)*SCALING_FACTOR);
		if(!newPtr){
			allocation_failed();
			return -1;
		}
		table->cap = SCALING_FACTOR;
		table->tbl = newPtr;
	}
	table->tbl[table->len] = tmp;
	table->len++;
	return 0;
}

/*
 * Convert lui instructions to machine code. Note that for the imm field of lui,
 * it may be an immediate number or a symbol and needs to be handled separately.
 * Output the instruction to the **OUTPUT**.(Consider using write_inst_hex()).
 * 
 * Return value:
 * 0 on success, -1 otherwise.
 * 
 * Arguments:
 * opcode:     op segment in MIPS machine code
 * args:       args[0] is the $rt register, and args[1] can be either an imm field or 
 *             a .data segment label. The other cases are illegal and need not be considered
 * num_args:   length of args array
 * addr:       Address offset of the current instruction in the file
 */
int write_lui(uint8_t opcode, FILE *output, char **args, size_t num_args, uint32_t addr, SymbolTable *reltbl) {
    printf("rt:%s\n", args[0]);
	int regName = 0;
	//判断args[0]是不是合法的寄存器
	if((regName = translate_reg(args[0])) == -1){
		return -1;
	}
	
	uint32_t instruction = 0;
	long imm_16 = 0;
	//判断是加载地址高16位（-1），还是将立即数加载到寄存器的高位
	if(translate_num(&imm_16, args[1], 0, 0x7fffffff) == -1){
		imm_16 = 0;
		char *label = create_copy_of_str(args[1]);
		//label后面的@Hi/Lo用\0截断
		label[strlen(label)-3] = '\0';
		if(is_valid_label(label) == 0){
			raise_label_error(addr, label);
			return -1;
		}
		add_to_table(reltbl, args[1], addr);
	}
	
	instruction += opcode;
	instruction <<= 5;//20-25为0
	instruction <<= 5;
	instruction += regName;
	instruction <<= 16;
	instruction += imm_16;
	write_inst_hex(output, instruction);
	return 0;
}

