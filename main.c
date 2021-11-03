#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define FLOAT_EPSILON 1e-6

float freq = 0.0;
float amp = 0.0;
int waveforms = 0;

float string2float(char *str)
{
    return strtod(str,NULL);
}

int check_input(char *str)
{
	int i = 0;
	while(str[i] != '\0'){
		if (!((str[i]>=48 && str[i]<=57)||str[i]==46)){
			return 0;
		}
		i++;
	}
	return 1;
}

int parse_arg(int argc, char **argv)
{

	int i = 0;

	if(argc <= 2){
		printf("No argument input\n");
		return 0;
	}
	while(argv[2*i+1] != NULL){
		if(argv[2*i+1][1] == '\0'){
			printf("Invalid option\n");
			return -1;
		}
	
		switch(argv[2*i+1][1]){
			case 'w': {
				if(!strcmp(argv[2*i+2],"sine")){
					printf("Generating Sine wave...\n"); 
					waveforms = 1; 	break;
				}

				else if(!strcmp(argv[2*i+2],"square")){
					printf("Generating Square wave...\n");
					waveforms = 2; break;

				}

				else if(!strcmp(argv[2*i+2],"triangular")){
					printf("Generating Triangular wave...\n");
					waveforms = 3; break;
				}

				else if(!strcmp(argv[2*i+2],"sawtooth")){
					printf("Generating Sawtooth wave...\n");
					waveforms = 4; break;
				}

				else{
					printf("Invalid waveform input\n");
					printf("Usage: ./main -w [waveform]\n");
					printf("waveform:\n\n");
					printf("\tsine\t\tgenerate sine wave\n");
					printf("\tsquare\t\tgenerate square wave\n");
					printf("\ttriangular\tgenerate triangular wave\n");
					printf("\tsawtooth\tgenerate sawtooth wave\n");
					return -1;
				}

			}

			case 'f':{
				if(check_input(argv[2*i+2])){
					freq = string2float(argv[2*i+2]);
					printf("Frequency = %.3f Hz\n", freq);
					break;
				}
				else{
					printf("Invalid frequency value\n");
					return -1;
				}
			}

			case 'a':{
				if(check_input(argv[2*i+2])){
					amp = string2float(argv[2*i+2]);
					printf("Amplitude = %.3f \n", amp);
					break;
				}
				else{
					printf("Invalid amplitude value\n");
					return -1;
				}
			}	
			default: printf("Invalid option\n");return -1;
		}
		i++;
	}
	return 1;
}

void parse_arg_default(){
	if (waveforms == 0){
		char temp[20];	int r;
		FILE *fp;
   		fp = fopen("setting.txt", "r");   //read
		while(1){
			r = fscanf(fp, "%s", temp);
			if (temp[0] == 'W'){
	    		fscanf(fp, "%d", &waveforms);
				printf("Use default waveforms\n");	
				break;
			}
			else if (r == EOF) break;
		}
		fclose(fp);
	}

	if (freq < FLOAT_EPSILON){
		char temp[20];	int r;
		FILE *fp;
   		fp = fopen("setting.txt", "r");   //read
		while(1){
			r = fscanf(fp, "%s", temp);
			if (temp[0] == 'F'){
	    		fscanf(fp, "%f", &freq);
				printf("Use default freqeuncy\n");	
				break;
			}
			else if (r == EOF) break;
		}
		fclose(fp);
	}
	if (amp < FLOAT_EPSILON){
		char temp[20];	int r;
		FILE *fp;
   		fp = fopen("setting.txt", "r");   //read
		while(1){
			r = fscanf(fp, "%s", temp);
			if (temp[0] == 'A'){
	    		fscanf(fp, "%f", &amp);	
				printf("Use default amplitude\n");	
				break;
			}
			else if (r == EOF) break;
		}
		fclose(fp);
	}
}

void save_setting(){
    char buffer [100];
    FILE *fp;
    fp = fopen("setting.txt", "w+");  // create file and write or overwrite if file existed
    sprintf(buffer, "Waveforms: %d\nFrequency(Hz): %f\nAmplitude: %f", waveforms, freq, amp);
    fputs(buffer, fp);
    fclose(fp);
}

int ask_for_default_setting(){
	char c, a;
	printf("Use default setting? (Y/N)");
	while(1){
		c = toupper(getchar());
		if ( c == 'Y') return 1;
		else if (c == 'N'){
			printf("Usage:  ./main [OPTIONS] INPUT\n\n");
			printf("Options:\n\n");
			printf("\t-w\tinput one of the waveform: sine, square, triangular, sawtooth\n");
			printf("\t-f\tinput frequency in Hz\n\t-a\tinput amplitude\n\n");
			printf("Sample: ./main -w [waveform] -a [value] -f [value]\n\n");
			return -1;
		}
		printf("Invalid input. Only input Y/N: ");
		while((a = getchar()) != '\n' &&  a != EOF);
	}
}


int main(int argc, char **argv)
{
	int state;
	state = parse_arg(argc, argv);
	if (state == -1) return 0;
	else if (state == 0){
		state = ask_for_default_setting();
		if (state == -1) return 0;
	}
	else if (state == 1){
		if (freq < FLOAT_EPSILON || amp < FLOAT_EPSILON){
			printf("No input for freqeuncy and/or amplitude\n");
			state = ask_for_default_setting();
		}
	}

	parse_arg_default();
	save_setting();
	printf("Check\n");
	printf("%d %f  %f \n", waveforms, freq, amp);
}