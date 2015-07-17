/*
 * FirstTransition.h
 *
 *  Created on: Jul 14, 2015
 *      Author: student
 */

#ifndef FIRSTTRANSITION_H_
#define FIRSTTRANSITION_H_

#include "Types.h"
#include <stdio.h>
#include <stdbool.h>

void execute_first_transition(FILE* pFile, char* file_name);

void process_line(line_info* info, unsigned int* ic, unsigned int* dc);

void process_data(line_info* info, unsigned int* dc, char* word, char* type);
void process_operation(line_info* info,unsigned int* ic, char* word);
void process_extern(line_info* info);

bool is_operation_name(char* symbol);
void get_word(line_info* info, char** word);
void get_data(line_info* info, char* type, unsigned int* dc);

#endif /* FIRSTTRANSITION_H_ */