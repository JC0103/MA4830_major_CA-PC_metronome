#ifndef PASS_ARG_H
#define PASS_ARG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define FLOAT_EPSILON 1e-6


float freq;         // 0.1-10.0 Hz
float amp;          // 1 - 100%

int waveforms;      // 1 = Sine Wave
                    // 2 = Square Wave
                    // 3 = Triangular Wave
                    // 4 = Sawtooth Wave



/**
 * A function to convert string number to float value.
 * @param arg contains a number value in string.
 * @return float number value.
 */
float string2float(char *str);


/**
 * A function to check if the input is within 0-9 or "."
 * @param arg contains a string input from user.
 * @return 1 for valid input or 0 for invalid input.
 */
int check_input(char *str);


/**
 * A function to parse argument at the beginning of program.
 * @param arg contains the number of input and the input string.
 * @return 1 for valid input, 0 for no input and -1 for invalid input.
 */
int parse_arg(int argc, char **argv);


/**
 * A function to read default value from txt file and assign the values to the variables. 
 */
void parse_arg_default();


/**
 * A function to save all current values of frequency, amplitude and waveform into a txt file. 
 */
void save_setting();


/**
 * A function to prompt user to use default value or exit the program. 
 * @return 1 if user choose to use default value, -1 if user choose to exit the program.
 */
int ask_for_default_setting();

/**
 * Print instruction on how to parse argument and its limit. 
 */
void print_instruction();

#endif
