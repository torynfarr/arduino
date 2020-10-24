# Arduino
This repository contains various sample C++ projects designed for the Arduino Uno WiFi Rev2 microcontroller.
<br />
<br />

## 01.[Cybernetic Webhook](https://github.com/torynfarr/arduino/tree/master/01.cybernetic-webhook)

This sample uses the Adafruid PN532 breakout board configured to communicate iva Serial Peripheral Interface (SPI). The PN532 is connected to an Arduino Uno WiFi Rev2. When the sketch runs, the Arduino will attempt to connect to the WiFi network using the SSID and WPA PSK defined in the *secrets* header file. Once connected, it verifies that the PN532 board is present and then prompts you to scan your cybernetic implant (or any NFC tag, if you're not chipped).

If the UID of the scanned tag matches the UID defined in the *secrets* header file, the Arduino will send an HTTP post request to an IFTTT webhook which you've configured to trigger an applet.

- When creating your webhook at IFTTT, give it the event name: *implant_scanned*  (or change the code accordingly)

- Enter your webhook service key from IFTTT in the *secrets* header file

- Instructions showing how to connect the PN532 breakout board to the Arduino can be found [here](https://learn.adafruit.com/adafruit-pn532-rfid-nfc/breakout-wiring)

- The UID read from the NFC tag should be either 4 bytes (likely Mifare Classic) or 7 bytes (likely Mifare Ultralight)

<br />
<img src="https://github.com/torynfarr/arduino/blob/master/docs/images/cybernetic-webhook.gif" width="350">
<br />

## Additional Information

- These samples were created using Visual Studio Code with the Arduino extension