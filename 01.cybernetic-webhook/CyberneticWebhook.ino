#include <Wire.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <Adafruit_PN532.h>
#include <string.h>
#include "secrets.h" 

#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)
#define WEBHOOK_HOST "maker.ifttt.com"
#define WEBHOOK_PORT 80
#define WEBHOOK_EVENT "implant_scanned" 

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

char ssid[] = SECRET_SSID;
char psk[] = SECRET_PSK;
int status = WL_IDLE_STATUS;

WiFiClient client;

void setup()
{
    Serial.begin(9600);

    while (!Serial)
    {
        ; // Wait for the serial port to connect.
    }

    Serial.println("Toryn's Haxxor Cybernetic Webhook");
    Serial.println("");

    // Check for the Arduino WiFi module.
    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println("Unable to find the Arduino WiFi module!");
        Serial.println("");
        while (true); // Halt
    }

    // Try to connect to the WiFi network.
    while (status != WL_CONNECTED)
    {
        Serial.println("Attempting to connect to WiFi...");
        Serial.println("");
        status = WiFi.begin(ssid, psk);
        delay(5000);
    }

    // A network connection has been established.
    Serial.println("WiFi connection established!");
    Serial.println("");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Subnet Mask: ");
    Serial.println((IPAddress)WiFi.subnetMask());
    Serial.print("Default Gateway: ");
    Serial.println((IPAddress)WiFi.gatewayIP());
    Serial.println();

    nfc.begin();

    Serial.println("Checking for PN53x RFID board...");
    Serial.println("");
    uint32_t versiondata = nfc.getFirmwareVersion();

    // Check for the Adafruit PN532 board. 
    if (! versiondata) 
    {
        Serial.print("Unable to find the PN53x board!");
        while (true); // Halt
    }

    // A PN53x RFID board has been found.
    Serial.println("PN53x RFID board found!");
    Serial.println();
    Serial.print("Model: PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
    Serial.print("Firmware version: "); Serial.print((versiondata>>16) & 0xFF, DEC); 
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
    Serial.println();

    // Configure the board to read RFID tags.
    nfc.SAMConfig();
  
    Serial.println("Scan your cybernetic implant...");
    Serial.println();
}

void loop()
{
    uint8_t success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the UID of the scanned tag.
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on the type of tag).
    
    // Wait for an NFC tag to be scanned. 
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

    if (success) 
    {
        String uidHexStr = "";

        for (int i = 0; i < uidLength; i++) 
        {
            uidHexStr = uidHexStr + decToHex(uid[i]);
        }

        if (uidHexStr == SECRET_UID)
        {
            Serial.println("Cybernetic implant deteted!");
            Serial.println();
            Serial.println("Greetings Toryn.");
            Serial.println();

            // Trigger the IFTTT event by sending an HTTP POST request to the webhook.
            if (client.connect(WEBHOOK_HOST, (uint16_t)WEBHOOK_PORT))
            {
                Serial.println("HTTP connection established!");
                Serial.println();

                Serial.println("Sending POST request to webhook...");
                Serial.println();

                client.println("POST /trigger/" + (String)WEBHOOK_EVENT + "/with/key/" + (String)SECRET_WEBHOOK_KEY + " HTTP/1.1");
                client.println("Host: " + (String)WEBHOOK_HOST);
                client.println("Connection: close");
                client.println();

                Serial.println("HTTP connection closed.");
                Serial.println();
            }
            
        } 
        else
        {
            Serial.println("Invalid NFC tag detected!");
            Serial.println();
        }

        // Wait three seconds before prompting to scan again.
        delay(3000);

        Serial.println("Scan your cybernetic implant...");
        Serial.println();

    }
}

// Converts an integer from decimal to hexadecimal.
String decToHex(int num) 
{
    // Append a zero to decimal values lower 0 through 9.
    if (num >= 0 && num < 10)
    {
        return "0" + String(num);
    }

    char arr[100];
    int i = 0;

    while(num!=0) 
    {
        int remainder = 0;
        remainder = num % 16;

        if(remainder < 10) 
        {
            arr[i] = remainder + 48;
            i++;
        } 
        else 
        {
            arr[i] = remainder + 55;
            i++;
        }

        num = num/16;
    }

    String hex = "";

    for(int j=i-1; j>=0; j--)
    {
        hex = hex + arr[j];
    }

    return hex;
}