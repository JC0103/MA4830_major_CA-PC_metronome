// 09September 2005
//******************************************************************************************************
// Performs basic I/O for the Omega PCI-DAS1602 
//
// Demonstration routine to demonstrate pci hardware programming
// Demonstrated the most basic DIO and ADC and DAC functions
// - excludes FIFO and strobed operations 
//
// Note : Single ADC - Disable Burst - Single write to DAC - only individual write seems to work.
//         : Need to check & verify DAC range  output/calibration
//
// G.Seet - 26 August 2005
//******************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <hw/pci.h>
#include <hw/inout.h>
#include <sys/neutrino.h>
#include <sys/mman.h>
																
#define	INTERRUPT		iobase[1] + 0				// Badr1 + 0 : also ADC register
#define	MUXCHAN			iobase[1] + 2				// Badr1 + 2
#define	TRIGGER			iobase[1] + 4				// Badr1 + 4
#define	AUTOCAL			iobase[1] + 6				// Badr1 + 6
#define DA_CTLREG		iobase[1] + 8				// Badr1 + 8

#define AD_DATA			iobase[2] + 0				// Badr2 + 0
#define AD_FIFOCLR		iobase[2] + 2				// Badr2 + 2

#define	TIMER0				iobase[3] + 0				// Badr3 + 0
#define	TIMER1				iobase[3] + 1				// Badr3 + 1
#define	TIMER2				iobase[3] + 2				// Badr3 + 2
#define	COUNTCTL		iobase[3] + 3				// Badr3 + 3
#define	DIO_PORTA		iobase[3] + 4				// Badr3 + 4
#define	DIO_PORTB		iobase[3] + 5				// Badr3 + 5
#define	DIO_PORTC		iobase[3] + 6				// Badr3 + 6
#define	DIO_CTLREG		iobase[3] + 7				// Badr3 + 7
#define	PACER1				iobase[3] + 8				// Badr3 + 8
#define	PACER2				iobase[3] + 9				// Badr3 + 9
#define	PACER3				iobase[3] + a				// Badr3 + a
#define	PACERCTL			iobase[3] + b				// Badr3 + b

#define DA_Data			iobase[4] + 0				// Badr4 + 0
#define DA_FIFOCLR		iobase[4] + 2				// Badr4 + 2

#define	DEBUG						1
 	
int badr[5];								// PCI 2.2 assigns 6 IO base addresses

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
int main() {
struct pci_dev_info info;
void *hdl;

uintptr_t iobase[6];
uintptr_t dio_in;
uint16_t adc_in;
	
unsigned int i,count;
unsigned short chan;

printf("\fDemonstration Routine for PCI-DAS 1602\n\n");

memset(&info,0,sizeof(info));
if(pci_attach(0)<0) {
  perror("pci_attach");
  exit(EXIT_FAILURE);
  }

																		/* Vendor and Device ID */
info.VendorId=0x1307;
info.DeviceId=0x01;

if ((hdl=pci_attach_device(0, PCI_SHARE|PCI_INIT_ALL, 0, &info))==0) {
  perror("pci_attach_device");
  exit(EXIT_FAILURE);
  }
  
  for(i=0;i<6;i++) {							// Another printf BUG ? - Break printf to two statements
    if(info.BaseAddressSize[i]>0) {
      printf("Aperture %d  Base 0x%x Length %d Type %s\n", i, 
        PCI_IS_MEM(info.CpuBaseAddress[i]) ?  (int)PCI_MEM_ADDR(info.CpuBaseAddress[i]) : 
        (int)PCI_IO_ADDR(info.CpuBaseAddress[i]),info.BaseAddressSize[i], 
        PCI_IS_MEM(info.CpuBaseAddress[i]) ? "MEM" : "IO");
      }
  }  
    														
printf("IRQ %d\n",info.Irq); 		
														// Assign BADRn IO addresses for PCI-DAS1602			
if(DEBUG) {
printf("\nDAS 1602 Base addresses:\n\n");
for(i=0;i<5;i++) {
  badr[i]=PCI_IO_ADDR(info.CpuBaseAddress[i]);
  if(DEBUG) printf("Badr[%d] : %x\n", i, badr[i]);
  }
 
	printf("\nReconfirm Iobase:\n");  			// map I/O base address to user space						
for(i=0;i<5;i++) {								// expect CpuBaseAddress to be the same as iobase for PC
  iobase[i]=mmap_device_io(0x0f,badr[i]);	
  printf("Index %d : Address : %x ", i,badr[i]);
  printf("IOBASE  : %x \n",iobase[i]);
  }													
}
														// Modify thread control privity
if(ThreadCtl(_NTO_TCTL_IO,0)==-1) {
  perror("Thread Control");
  exit(1);
  }																											


while(1)
{
														//*****************************************************************************
														//Digital Port Functions
														//*****************************************************************************																																	

printf("\nDIO Functions\n");													  										
out8(DIO_CTLREG,0x90);					// Port A : Input,  Port B : Output,  Port C (upper | lower) : Output | Output			

dio_in=in8(DIO_PORTA); 					// Read Port A	
printf("Port A : %02x\n", dio_in);																												
																						
out8(DIO_PORTB, dio_in);					// output Port A value -> write to Port B 		



 														//******************************************************************************
														// ADC Port Functions
														//******************************************************************************
														// Initialise Board								
out16(INTERRUPT,0x60c0);				// sets interrupts	 - Clears			
out16(TRIGGER,0x2081);					// sets trigger control: 10MHz, clear, Burst off,SW trig. default:20a0
out16(AUTOCAL,0x007f);					// sets automatic calibration : default

out16(AD_FIFOCLR,0); 						// clear ADC buffer
out16(MUXCHAN,0x0D00);				// Write to MUX register - SW trigger, UP, SE, 5v, ch 0-0 	
														// x x 0 0 | 1  0  0 1  | 0x 7   0 | Diff - 8 channels
														// SW trig |Diff-Uni 5v| scan 0-7| Single - 16 channels
printf("\n\nRead multiple ADC\n");
count=0x00;
while(count <0x02) {
  chan= ((count & 0x0f)<<4) | (0x0f & count);
  out16(MUXCHAN,0x0D00|chan);		// Set channel	 - burst mode off.
  delay(1);											// allow mux to settle
  out16(AD_DATA,0); 							// start ADC 
  while(!(in16(MUXCHAN) & 0x4000));
  adc_in=in16(AD_DATA);   
  printf("ADC Chan: %02x Data [%3d]: %4x \n", chan, (int)count, (unsigned int)adc_in);		// print ADC													
  fflush( stdout );
  count++;
  delay(5);											// Write to MUX register - SW trigger, UP, DE, 5v, ch 0-7 	
  }

 														//******************************************************************************
														//D/A Port Functions
														//******************************************************************************
	
printf("\n\nRead multiple DAC\n");																						
for(i=0x8fff;i<0xffff;i=i+0x80) {
	out16(DA_CTLREG,0x0a23);			// DA Enable, #0, #1, SW 5V unipolar		2/6
   	out16(DA_FIFOCLR, 0);					// Clear DA FIFO  buffer
   	out16(DA_Data,(short) i);																																		
   	out16(DA_CTLREG,0x0a43);			// DA Enable, #1, #1, SW 5V unipolar		2/6
  	out16(DA_FIFOCLR, 0);					// Clear DA FIFO  buffer
	out16(DA_Data,(short) i);																																		

  	printf("DAC Data [%4x]: %4x\r", i, i);		// print DAC													
  	fflush( stdout ); 
  	delay(2);
  	}
  	 
														// Reset DAC to 5v
out16(DA_CTLREG,(short)0x0a23);	
out16(DA_FIFOCLR,(short) 0);			
out16(DA_Data, 0x8fff);						// Mid range - Unipolar																											
  
out16(DA_CTLREG,(short)0x0a43);	
out16(DA_FIFOCLR,(short) 0);			
out16(DA_Data, 0x8fff);		
																												
printf("\n\nExit Demo Program\n");

//*****************************************************************************************************************

//pci_detach_device(hdl);

//return(0);
}

}