#ifndef PASS_ARG_H
#define PASS_ARG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define FLOAT_EPSILON 1e-6



float freq;         // 0.1-10.0 Hz
float amp;          // 1 - 100%  //TODO: Set amp only take input from 0-100	
int waveforms;      // 1: Sine Wave
                    // 2: Square Wave
                    // 3: Triangular Wave
                    // 4: Sawtooth Wave


float string2float(char *str);
int check_input(char *str);
int parse_arg(int argc, char **argv);
void parse_arg_default();
void save_setting();
int ask_for_default_setting();

#endif