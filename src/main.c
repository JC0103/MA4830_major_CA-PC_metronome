#include "pass_arg.h"
#include "PCI_waves.h"

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

	//Start thread#1 and thread#2

	parse_arg_default();
	printf("Check\n");
	printf("%d %f  %f \n", waveforms, freq, amp);

	pci_setup();
	while(1) generate_wave(waveforms, freq, amplitude);

	// TODO: SIGNINT handler
	save_setting();
	shutdown_pci();

}