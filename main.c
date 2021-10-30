#include <stdlib.h>
#include <stdio.h>
#include <string.h>

float string2float(char *str)
{
    return strtod(str,NULL);
}



int parse_arg(int argc, char **argv)
{
	char **p_to_arg = &argv[1];
	float freq;
	float amp;
	int i = 0;

	if(argc < 2){
		printf("Input required\nUsage:  ./main [OPTIONS] INPUT\n\n");
		printf("Options:\n\n");
		printf("\t-w\tinput one of the waveform: sine, square, triangular, sawtooth\n");
		printf("\t-f\tinput frequency in Hz\n\t-a\tinput amplitude\n\n");
		printf("Sample: ./main -w [waveform] -a [value] -f [value]\n\n");
		return -1;
	}

	while(argv[2*i+1] != NULL){
		if(argv[2*i+1][1] == '\0'){
			printf("Invalid option\n");
			return -1;
		}

		switch(argv[2*i+1][1]){
			case 'w': {
				if(!strcmp(argv[2*i+2],"sine")){
					printf("Generating Sine wave...\n");break;
					//return 1;
				}

				else if(!strcmp(argv[2*i+2],"square")){
					printf("Generating Square wave...\n");break;
					//return 2;
				}

				else if(!strcmp(argv[2*i+2],"triangular")){
					printf("Generating Triangular wave...\n");break;
					//return 3;
				}

				else if(!strcmp(argv[2*i+2],"sawtooth")){
					printf("Generating Sawtooth wave...\n");break;
					//return 4;
				}

				else{
					printf("Invalid input\n");
					printf("Usage: ./main -w [waveform]\n");
					printf("waveform:\n\n");
					printf("\tsine\t\tgenerate sine wave\n");
					printf("\tsquare\t\tgenerate square wave\n");
					printf("\ttriangular\tgenerate triangular wave\n");
					printf("\tsawtooth\tgenerate sawtooth wave\n");
					return -1;
				}

			}

			case 'f':freq = string2float(argv[2*i+2]);printf("Frequency = %.3f Hz\n", freq);break;

			case 'a':amp = string2float(argv[2*i+2]);printf("Amplitude = %.3f \n", amp);break;

			default: printf("Invalid option\n");return -1;
		}
		i++;
	}

}

int main(int argc, char **argv)
{
	parse_arg(argc, argv);
}