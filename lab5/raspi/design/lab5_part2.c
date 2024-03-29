/*Xingyu Zhou
 * xzhou45@ucsc.edu
 * Lab5 part2
 * raspi code
 * this code constantly read and send data
 * through USB
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <errno.h>
#include <libusb.h>


int main (int argc, char * argv[]){

	libusb_device_handle* dev; // Pointer to data structure representing USB device

    char rx_data[64]; // Receive data block
    int sent_bytes; // Bytes transmitted
    int rcvd_bytes; // Bytes received
    int return_val;

    libusb_init(NULL); // Initialize the LIBUSB library

    // Open the USB device (the Cypress device has
    // Vendor ID = 0x04B4 and Product ID = 0x8051)
    dev = libusb_open_device_with_vid_pid(NULL, 0x04B4, 0x8051);

    if (dev == NULL){
        perror("device not found\n");
    }
  
    // Reset the USB device.
    // This step is not always needed, but clears any residual state from
    // previous invocations of the program.
    if (libusb_reset_device(dev) != 0){
        perror("Device reset failed\n");
    } 

    // Set configuration of USB device
    if (libusb_set_configuration(dev, 1) != 0){
        perror("Set configuration failed\n");
    } 


    // Claim the interface.  This step is needed before any I/Os can be
    // issued to the USB device.
    if (libusb_claim_interface(dev, 0) !=0){
          perror("Cannot claim interface");
      }

    // Clear receive buffer
    for (int i=0; i<64; i++) rx_data[i] = 0;
  
    for(;;){
		// Perform the IN transfer (from device to host).
        // This will read the data back from the device.
		return_val=libusb_bulk_transfer(dev, // Handle for the USB device
				    (0x01 | 0x80), // Address of the Endpoint in USB device
				    // MS bit nust be 1 for IN transfers
				    rx_data, // address of receive data buffer
				    64, // Size of data buffer
				    &rcvd_bytes, // Number of bytes actually received
				    0 // Timeout in milliseconds (0 to disable timeout)
				    );
		if(return_val != 0){
			perror("DEAD");
			return -1;
			}
		return_val=libusb_bulk_transfer(dev, // Handle for the USB device
				    0x02, // Address of the Endpoint in USB device
				    rx_data, // address of data block to transmit
				    64, // Size of data block
				    &sent_bytes, // Number of bytes actually sent
				    0 // Timeout in milliseconds (0 to disable timeout)
				    );
		if(return_val != 0){
			perror("DEAD");
			return -1;
			}
	}
	libusb_close(dev);
}
	
