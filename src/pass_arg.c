#include "pass_arg.h"


float string2float(char *str)
{
    return strtod(str,NULL);
}

// Check if the input is in numbers or dot
int check_input(char *str)
{
	int i = 0; int point_count = 0;
	while(str[i] != '\0' && str[i] != 0){
		if (!((str[i]>=48 && str[i]<=57)||str[i]==46)){
			return 0;
		}
		if (str[i]==46) point_count++;
		if (point_count >= 2) return 0;
		i++;
	}
	return 1;
}

// Function that parse argument which handling exception
int parse_arg(int argc, char **argv)
{

	int i = 0;
	int valid = 1;
	// No argument parse
	if(argc <= 2){
		printf("No argument input\n");
		return 0;
	}
	while(argv[2*i+1] != NULL){
		// Handle invalid option (must have '-' as first element, not "\0" as second element, and "\0" as third element)
		if(argv[2*i+1][1] == '\0' || argv[2*i+1][0] != '-' || argv[2*i+1][2] != '\0'){
			printf("Invalid option\n");
			return -1;
		}
	
		switch(argv[2*i+1][1]){
			// Waveforms
			case 'w': {
				if(!strcmp(argv[2*i+2],"sine")){
					printf("Sine wave was chosen\n"); 
					waveforms = 1; 	break;
				}

				else if(!strcmp(argv[2*i+2],"square")){
					printf("Square wave was was chosen\n");
					waveforms = 2; break;

				}

				else if(!strcmp(argv[2*i+2],"triangular")){
					printf("Triangular wave was chosen\n");
					waveforms = 3; break;
				}

				else if(!strcmp(argv[2*i+2],"sawtooth")){
					printf("Sawtooth wave was chosen\n");
					waveforms = 4; break;
				}
				// Print instructions to input correct parameters for waveforms
				else{
					printf("Invalid waveform input\n");
					printf("Usage: ./main -w [waveform]\n");
					printf("waveform:\n\n");
					printf("\tsine\t\tgenerate sine wave\n");
					printf("\tsquare\t\tgenerate square wave\n");
					printf("\ttriangular\tgenerate triangular wave\n");
					printf("\tsawtooth\tgenerate sawtooth wave\n\n");
					valid = 0;
					break;
				}

			}
			// Frequency 
			case 'f':{
				// Check if input is empty
				if (argv[2*i+2] == NULL) return -1;
				// Check if inputs are numbers or dot
				if(check_input(argv[2*i+2])){
					// Change string data type to float
					freq = string2float(argv[2*i+2]);
					// Set limit for frequency input
					if(freq>=0.1 && freq<=10){
						printf("Frequency = %.1f Hz\n", freq);
						break;
					}
					// Handle out of bound inputs
					else{
						printf("Frequency value out of range, valid range: (0.1 <= f <= 10)\n\n");valid = 0;break;
					}
				}
				// Handle invalid input (not numbers)
				else{
					printf("Invalid frequency value\n");						
					return -1;
				}
			}
			// Amplitude
			case 'a':{
				// Check if input is empty
				if (argv[2*i+2] == NULL) return -1;
				// Check if inputs are numbers or dot
				if(check_input(argv[2*i+2])){
					// Change string data type to float
					amp = string2float(argv[2*i+2]);
					// Set limit for amplitude input 
					if(amp>=0 && amp<=100){
						printf("Amplitude = %.1f percent \n", amp);
						break;
					}
					//Handle out of bound inputs
					else{
						printf("Amplitude value out of range, valid range: (1 <= a <= 100)\n\n");valid = 0;break;

					}
				}
				// Handle invalid input (not numbers)
				else{
					printf("Invalid amplitude value\n");
					return -1;
				}
			}
			// Handle argument that is not w, f or a	
			default: printf("Invalid option\n");return -1;
		}
		i++;
	}
	if(valid == 0)
		return -1;
	return 1;
}

// Parse default setting prameters from setting.txt to main program
void parse_arg_default(){
	if (waveforms == 0){
		char temp[20];	int r;
		FILE *fp;
   		fp = fopen("setting.txt", "r");   //read file
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
   		fp = fopen("setting.txt", "r");   //read file
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
   		fp = fopen("setting.txt", "r");   //read file
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

// Save the current setting parameters as txt file
void save_setting(){
    char buffer [100];
    FILE *fp;
    fp = fopen("setting.txt", "w+");  // create file and write or overwrite if file existed
	// Write the txt file with latest parameters
    sprintf(buffer, "Waveforms: %d\nFrequency(Hz): %f\nAmplitude: %f", waveforms, freq, amp);
    fputs(buffer, fp);
	// Close file
    fclose(fp);
}

// Ask user to use default setting parameters from the 'setting.txt' file or not
int ask_for_default_setting(){
	char c, a;
	printf("Use default setting? (Y/N)");
	while(1){
		c = toupper(getchar());
		if ( c == 'Y') return 1;
		else if (c == 'N'){
			return -1;
		}
		printf("Invalid input. Only input Y/N: ");
		while((a = getchar()) != '\n' &&  a != EOF);
	}
}

void print_instruction(){
	printf("\nUsage:  ./main [OPTIONS] INPUT\n\n");
	printf("Options:\n\n");
	printf("\t-w\tinput one of the waveform: sine, square, triangular, sawtooth\n");
	printf("\t-f\tinput frequency in Hz: 0.1 - 10\n\t-a\tinput amplitude in percentage: 1 - 100\n\n");
	printf("Sample: ./main -w [waveform] -a [value] -f [value]\n\n");
}
