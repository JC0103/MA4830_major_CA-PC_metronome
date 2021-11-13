#include "PCI_waves.h"
#include "pass_arg.h"


void pci_setup(){													

// printf("\fDemonstration Routine for PCI-DAS 1602\n\n");

// memset(&info,0,sizeof(info));
// if(pci_attach(0)<0) {
//    perror("pci_attach");
//    exit(EXIT_FAILURE);
//    }

//  																		/* Vendor and Device ID */
// info.VendorId=0x1307;
// info.DeviceId=0x01;

// if ((hdl=pci_attach_device(0, PCI_SHARE|PCI_INIT_ALL, 0, &info))==0) {
//   perror("pci_attach_device");
//   exit(EXIT_FAILURE);
//   }
//    																		// Determine assigned BADRn IO addresses for PCI-DAS1602			

// printf("\nDAS 1602 Base addresses:\n\n");
// for(i=0;i<5;i++) {
//   badr[i]=PCI_IO_ADDR(info.CpuBaseAddress[i]);
//   printf("Badr[%d] : %x\n", i, badr[i]);
//   }
 
// printf("\nReconfirm Iobase:\n");  						// map I/O base address to user space						
// for(i=0;i<5;i++) {												// expect CpuBaseAddress to be the same as iobase for PC
//   iobase[i]=mmap_device_io(0x0f,badr[i]);	
//   printf("Index %d : Address : %x ", i,badr[i]);
//   printf("IOBASE  : %x \n",iobase[i]);
//   }													
//  																		// Modify thread control privity
// if(ThreadCtl(_NTO_TCTL_IO,0)==-1) {
//   perror("Thread Control");
//   exit(1);
//   }											
}

void generate_wave(int waveforms, float freq, float amplitude){
amplitude = amplitude/100.0 * 0xFFFF;
intercept = amplitude * 2;

//Setup waveforms array
  switch (waveforms){
    case 1:
    //Sine
    delta=(2.0*3.142)/STEP;
    for(i=0;i<STEP;i++) {
      dummy= ((sinf((float)(i*delta))) + 1.0) * amplitude / 2 ;   // A * sin(Bx) A = amplitude, B = frequency
      data[i]= (unsigned) dummy;			// add offset +  scale
      // printf("data%d: %d\n", i, data[i]);
    }
    break;

    case 2:
    //Square
    for (i=0;i<STEP;i++){
      dummy = (i< STEP / 2) ? 0 : 1 * amplitude;
      data[i]= (unsigned) dummy;
    }
    break;

    case 3:
    //Triangular
    delta = 1 / STEP;
    for(i=0;i<STEP;i++){
      linear_func = i * delta * amplitude * 2;
      dummy = (i < STEP / 2) ? linear_func : - linear_func + intercept;
      data[i]= (unsigned) dummy;
    }
    break;

    case 4:
    //Sawtooth
    delta = 1 / STEP;
    for(i=0;i<STEP;i++){
      dummy = i * delta * amplitude;
      data[i]= (unsigned) dummy;
    }
    break;

    default: break; //TODO: pass exception
  }

  // for(i=0;i<STEP;i++) {
  //   out16(DA_CTLREG,0x0a23);			// DA Enable, #0, #1, SW 5V unipolar		2/6 (pg.23)
  //   out16(DA_FIFOCLR, 0);					// Clear DA FIFO  buffer  
  //   out16(DA_Data,(short) data[i]);																																		
  //   out16(DA_CTLREG,0x0a43);			// DA Enable, #1, #1, SW 5V unipolar		2/6 (pg.23)
  //   out16(DA_FIFOCLR, 0);					// Clear DA FIFO  buffer
  //   out16(DA_Data,(short) data[i]);
  //   delay(1000/(freq * (float)STEP));   // Min delay: 1 ms , max freq: 10Hz
  // }
}							

void shutdown_pci(){
//**********************************************************************************************
// Reset DAC to default : 5v
//**********************************************************************************************

// out16(DA_CTLREG,(short)0x0a23);	
// out16(DA_FIFOCLR,(short) 0);			
// out16(DA_Data, 0x8fff);						// Mid range - Unipolar																											
  
// out16(DA_CTLREG,(short)0x0a43);	
// out16(DA_FIFOCLR,(short) 0);			
// out16(DA_Data, 0x8fff);				
																																						
// pci_detach_device(hdl);
// printf("\n\nPCI detached\n");

//**********************************************************************************************
}

