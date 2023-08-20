/*****************************************************************
 * BUAA Fall 2021 Fundamentals of Computer Hardware
 * Project7 Assembler and Linker
 *****************************************************************
 * my_linker_utils.c
 * Linker Submission
 *****************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "lib/tables.h"
#include "linker-src/linker_utils.h"

/*
 * Builds the symbol table and relocation data for a single file.
 * Read the .data, .text, .symbol, .relocation segments in that order.
 * The size of the .data and .text segments are read and saved in the
 * relocation table of the current file. For the .symbol and .relocation
 * segments, save the symbols in them in the corresponding locations.
 *
 * Return value:
 * 0 if no errors, -1 if error.
 *
 * Arguments:
 * input:            file pointer.
 * symtbl:           symbol table.
 * reldt:            pointer to a Relocdata struct.
 * base_text_offset: base text offset.
 * base_data_offset: base data offset.
 */
char* IGNORE_CHARS_FD = " \f\n\r\t\v,():";
#define N 100050
int fill_data(FILE *input, SymbolTable *symtbl, RelocData *reldt, uint32_t base_text_offset, uint32_t base_data_offset) {
    int dataSize = 0;
    int textSize = 0;
    printf("Going into fill_data\n");
    char line[N];
    //char* s ;
    //int mode = 0;//moving into which section 0：data/text 1：symbol 2：relocation
    while(fgets(line, N, input))
    {
        printf("getting line content: %s\n", line);
        if(strcmp(line, ".data\n") == 0){
            printf("Get .DATA section\n");
            dataSize = calc_data_size(input);
            printf("dataSize:%d\n", dataSize);
            reldt->data_size = dataSize;
        }
        else if(strcmp(line, ".text\n") == 0){
            printf("Get .TEXT section\n");
            textSize = calc_text_size(input);
            printf("textSize:%d\n", textSize); 
            reldt->text_size = textSize;
        }
        else if(strcmp(line, ".symbol\n") == 0){
            add_to_symbol_table(input, symtbl, base_text_offset, base_data_offset);
            puts("end of symbol");
        }  
        else if(strcmp(line, ".relocation\n") == 0){
            add_to_symbol_table(input, reldt->table, 0, 0);
        }
    }
    puts("End of fill_data");
    return 0;
}

/*
 * Detect whether the given instruction with offset OFFSET needs relocation.
 *
 * Return value:
 * 1 if the instruction needs relocation, 0 otherwise.
 *
 * Arguments:
 * offset: Address offset of the instruction in the current file.
 * reltbl: relocation table corresponding to the current file.
 */
int inst_needs_relocation(SymbolTable *reltbl, uint32_t offset) {
    int len = reltbl->len;
    for(int i = 0; i < len; i++){
        if(reltbl->tbl[i].addr == offset)
            return 1;
    }
    return 0;
}
