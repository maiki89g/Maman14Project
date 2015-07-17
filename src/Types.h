/*
 * Types.h
 *
 *  Created on: Jul 9, 2015
 *      Author: student
 */

#ifndef TYPES_H_
#define TYPES_H_

/*
 * A symbol in the code.
 * Can be an instruction or an operation.
 */
typedef struct
{
	char* name;
	int is_instruction;
	int is_external;
	unsigned int address;
} symbol;

typedef struct symbol_node* symbol_node_ptr;

/*
 * A symbol node in the symbol table.
 */
typedef struct symbol_node
{
	symbol data;
	symbol_node_ptr next;
} symbol_node;

/*
 * An instruction. The instruction is built out of 12 bits;
 */
typedef struct
{
	unsigned int era : 2;
	unsigned int target_operand_address_method : 2;
	unsigned int source_operand_address_method : 2;
	unsigned int op_code : 4;
	unsigned int group : 2;
} instruction;

typedef struct data {
	char value;
	unsigned int address;
} data;

typedef struct data_node* data_node_ptr;

typedef struct data_node {
	data value;
	data_node_ptr next;
} data_node;

typedef struct line_info {
	char* line_str;
	int line_length;
	int current_index;
	int line_number;
	char* file_name;
} line_info;

#endif /* TYPES_H_ */