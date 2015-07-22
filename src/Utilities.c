/*
 * Utilities.c
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Types.h"
#include "Consts.h"
#include "Utilities.h"

operation_node_ptr p_operation_head = NULL;

void print_compiler_error(char* message, line_info* info) {
	fprintf(stderr, "Error: %s, File %s Line %d \n", message, info->file_name, info->line_number);
}

void skip_all_spaces(line_info* info) {
	int index = info->current_index;

	while ((index < info->line_length) && isspace(info->line_str[index])) {
		index++;
	}

	info->current_index = index;
}

void get_next_word(line_info* info, char** word, bool skip_spaces)
{
	int i, word_end_index, word_start_index, word_length;

	if (*word != NULL) {
		free(*word);
	}

	if (skip_spaces)
		skip_all_spaces(info);

	word_end_index  = word_start_index = i = info->current_index;

	for (;i < info->line_length; i++) {
		if (!isspace(info->line_str[i]) && (info->line_str[i] != LABEL_END_TOKEN)) {
			word_end_index = i;
		}
		else
		{
			break;
		}
	}

	word_length = word_end_index - word_start_index + 1;

	*word = (char*)malloc(sizeof(char) * (word_length + 1));

	if (*word == NULL) {
		/* TODO: bad alloc */
	} else {
		strncpy(*word, info->line_str + word_start_index, word_length);
		(*word)[word_length] = '\0';

		info->current_index = word_end_index + 1;
	}
}

line_info* create_line_info(char* file_name, int line_number, char* line_str) {
	line_info* info = (line_info*)malloc(sizeof(line_info));

	if (info == NULL) {
		/* TODO: bad alloc */
	} else {
		info->current_index = 0;
		info->file_name = file_name;
		info->line_number = line_number;
		info->line_str = line_str;
		info->line_length = strlen(line_str);
	}

	return info;
}

void get_operation(char* word, char** operation, int* counter) {
	int i = 0;
	int word_length = strlen(word);

	while ((i < word_length) && isalpha(word[i])) {
		i++;
	}

	*operation = (char*)malloc(sizeof(char) * (i + 1));

	if (*operation == NULL) {
		/* TODO: bad alloc */
	} else {
		strncpy(*operation, word, i);
		(*operation)[i] = '\0';

		*counter = atoi(word + i);
	}
}

operation* get_operation_info(char* operation) {
	operation_node_ptr p_current;

	if (p_operation_head == NULL) {
		init_operation_list();
	}

	p_current = p_operation_head;

	while (p_current != NULL) {
		if (strcmp(p_current->data.name, operation) == 0) {
			return &(p_current->data);
		}

		p_current = p_current->next;
	}

	return NULL;
}

void init_operation_list() {
	int op_code = 0;

	add_operation_to_list(MOV_OPERATION, op_code++, 2);
	add_operation_to_list(CMP_OPERATION, op_code++, 2);
	add_operation_to_list(ADD_OPERATION, op_code++, 2);
	add_operation_to_list(SUB_OPERATION, op_code++, 2);
	add_operation_to_list(NOT_OPERATION, op_code++, 1);
	add_operation_to_list(CLR_OPERATION, op_code++, 1);
	add_operation_to_list(LEA_OPERATION, op_code++, 2);
	add_operation_to_list(INC_OPERATION, op_code++, 1);
	add_operation_to_list(DEC_OPERATION, op_code++, 1);
	add_operation_to_list(JMP_OPERATION, op_code++, 1);
	add_operation_to_list(BNE_OPERATION, op_code++, 1);
	add_operation_to_list(RED_OPERATION, op_code++, 1);
	add_operation_to_list(PRN_OPERATION, op_code++, 1);
	add_operation_to_list(JSR_OPERATION, op_code++, 1);
	add_operation_to_list(RTS_OPERATION, op_code++, 0);
	add_operation_to_list(STOP_OPERATION, op_code++, 0);
}

void add_operation_to_list(char* name, unsigned int code, int operands) {
	operation_node_ptr p_new = (operation_node_ptr)malloc(sizeof(operation_node));

	if (p_new == NULL) {
		/* Todo: bad alloc */
	} else {
		p_new->data.name = name;
		p_new->data.code = code;
		p_new->data.operands_number = operands;
		p_new->next = NULL;

		if (p_operation_head == NULL) {
			p_operation_head = p_new;
		} else {
			p_new->next = p_operation_head;
			p_operation_head = p_new;
		}
	}
}

char* get_label(line_info* info) {
	char* label = NULL;
	int i = 1;

	/*
	 * A label must start on the first index of the line.
	 * The first token of the label must be a valid alpha letter
	 */
	if (isalpha(info->line_str[0])) {
		while (isalnum(info->line_str[i])) {
			i++;
		}

		if (i > MAX_LINE_LENGTH) {
			print_compiler_error("Invalid label length", info);
		} else if (info->line_str[i] != LABEL_END_TOKEN) {
			print_compiler_error("A label must end with a colon", info);
		} else {
			label = (char*)malloc(sizeof(char) * (i + 1));

			if (label == NULL) {
				/* TODO: bad alloc */
			} else {
				strncpy(label, info->line_str, i);
				label[i] = '\0';

				info->current_index = i + 1;
			}
		}
	}

	return label;
}

bool is_empty_or_comment(char* line) {
	int length = strlen(line);

	if (line[0] == COMMENT) {
		return true;
	} else {
		int i;

		for (i = 0; i < length; i++) {
			if (!isspace(line[i])) {
				return false;
			}
		}

		return true;
	}
}

char* get_next_operand(line_info* info) {
	char* operand = NULL;
	int i, operand_end_index, operand_start_index, operand_length;

	skip_all_spaces(info);

	operand_end_index  = operand_start_index = i = info->current_index;

	for (;i < info->line_length; i++) {
		if (!isspace(info->line_str[i]) && (info->line_str[i] != OPERAND_SEPERATOR)) {
			operand_end_index = i;
		}
		else
		{
			break;
		}
	}

	operand_length = operand_end_index - operand_start_index + 1;

	operand = (char*)malloc(sizeof(char) * (operand_length + 1));

	if (operand == NULL) {
		/* TODO: bad alloc */
	} else {
		strncpy(operand, info->line_str + operand_start_index, operand_length);
		operand[operand_length] = '\0';

		info->current_index = operand_end_index + 1;
	}

	return operand;
}

ADDRESS_METHOD get_operand_method(char* operand) {
	int operand_length = strlen(operand);

	if (operand_length > 0) {
		if (operand[0] == IMMEDIATE_TOKEN) {
			return IMMEDIATE;
		} else if (strcmp(operand, COPY_PERVIOUS_STR) == 0) {
			return COPY_PREVIOUS;
		} else if (is_register(operand, operand_length)) {
			return DIRECT_REGISTER;
		} else {
			return DIRECT;
		}
	}

	return DIRECT;
}

bool is_register(char* operand, int length) {
	int digit;

	if ((length != 2) || (operand[0] != REGISTER_FIRST_TOKEN) || (!isdigit(operand[2]))) {
		return false;
	}

	digit = atoi(operand + 1);

	return (digit < REGISTERS_COUNT) && (digit >= 0);
}
