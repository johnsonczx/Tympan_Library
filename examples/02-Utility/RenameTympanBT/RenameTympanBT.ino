/*
*   RenameTympanBT
*
*   Created: Chip Audette, OpenAudio, Mar 2018
*   Updated: Chip Audette, Feb 2019 for Tympan RevD
*   Purpose: Change the name of the Bluetooth module on the Tympan to make it 
*       easier for us fellow humans to recognize when connecting via PC, phone, 
*       or other device.
*       
*   Assumptions:  This code assumes that you already know how to reprogram your Tympan
*       Using any of the example programs that came with the Tympan_Library.  If so,
*       continue.
*       
*   How to Use:
*       1) Know whether you have a Tympan RevC or RevD:
*           * Tympan RevC has two circuit boards stacked on top of each other.  BT is RN42 module.
*           * Tympan RevD has one circuit board.  BT is BC127 module
*       2) Plug your Tympan into your PC via USB, like you're going to reprogram it
*          from here within the Arduino IDE
*       3) Turn on your Tympan
*       4) Open the Serial Monitor (you might first need to go Tools->Port to choose your Teensy)
*       5) Compile and Upload this sketch
*       6) Once the code uploads to the Tympan, watch the text on the Serial Monitor.
*
*   Works with Tympan Rev C or Tympan Rev D
*       * Rev C uses RN42:  https://cdn.sparkfun.com/datasheets/Wireless/Bluetooth/bluetooth_cr_UG-v1.0r.pdf
*       * Rev D uses BC127: https://learn.sparkfun.com/tutorials/understanding-the-bc127-bluetooth-module/bc127-basics
*
*   MIT License.  use at your own risk.
*/

#include <Tympan_Library.h>

#if 1
  //Use this if you have a Tympan RevD
  TympanPins tympPins(TYMPAN_REV_D);     //set to TYMPAN_REV_D, or be more specific to your version of D
  TympanBase audioHardware(tympPins);
  String     new_BT_name = "TympanD";    //change this to whatever you'd like
#else
  //Use this if you have a Tympan RevC
  TympanPins tympPins(TYMPAN_REV_C);
  TympanBase audioHardware(tympPins);
  String     new_BT_name = "TympanC";    //change this to whatever you'd like
#endif

usb_serial_class *USB_Serial;
HardwareSerial *BT_Serial;
String response, given_BT_name, BT_hex;

void echoIncomingUSBSerial(void) {
  while (USB_Serial->available()) {
    BT_Serial->write(USB_Serial->read()); //echo messages from USB Serial over to BT Serial
  }
}

void echoIncomingBTSerial(void) {
  while (BT_Serial->available()) {
    USB_Serial->write(BT_Serial->read());//echo messages from BT serial over to USB Serial
  }
}

void readBTResponse(void) {
   response = BT_Serial->readString();
}

#include "renameBT_RevC.h"
#include "renameBT_RevD.h"

// Arduino setup() function, which is run once when the device starts
void setup() {
  // Start the serial ports on the Tympan
  audioHardware.beginBothSerial(); delay(2000);
  USB_Serial = audioHardware.getUSBSerial();
  BT_Serial = audioHardware.getBTSerial();
  USB_Serial->println("*** RenameTypmanBT.  Starting...");

  // Issue the commands to change the BT name (and set other BT settings)
  if (tympPins.tympanRev == TYMPAN_REV_C) {
    renameBT_RevC();
  } else {
    renameBT_RevD(); 
  }
  
  //transition to Serial echo mode to allow playing around, if desired
  USB_Serial->println("*** Now entering BT <-> USB Echo mode...feel free to send messages back and forth.");
}

// Arduino loop() function, which is run over and over for the life of the device
void loop() {
  echoIncomingUSBSerial();     //echo messages from USB Serial over to BT Serial
  echoIncomingBTSerial();  //echo messages from BT serial over to USB Serial
} //end loop();


