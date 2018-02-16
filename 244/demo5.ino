/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example LED Remote
 *
 * This is an example of how to use the RF24 class to control a remote
 * bank of LED's using buttons on a remote control.
 *
 * On the 'remote', connect any number of buttons or switches from
 * an arduino pin to ground.  Update 'button_pins' to reflect the
 * pins used.
 *
 * On the 'led' board, connect the same number of LED's from an
 * arduino pin to a resistor to ground.  Update 'led_pins' to reflect
 * the pins used.  Also connect a separate pin to ground and change
 * the 'role_pin'.  This tells the sketch it's running on the LED board.
 *
 * Every time the buttons change on the remote, the entire state of
 * buttons is send to the led board, which displays the state.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 (CE & CS)

RF24 radio(9,10);

// sets the role of this unit in hardware.  Connect to GND to be the 'led' board receiver
// Leave open to be the 'remote' transmitter
const int role_pin = A4;

// Pins on the remote for buttons
const uint8_t button_pins[] = { 2,3,4,5,6 };
const uint8_t num_button_pins = sizeof(button_pins);

// Pins on the LED board for LED's
const uint8_t led_pins[] = { 2,3,4,5,6};
const uint8_t num_led_pins = sizeof(led_pins);

//
// Topology
//

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe_tem = 0xE8E8F0F0E1EE;
const uint64_t pipe_light = 0xE8E8F0F0E1CC;

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes in this
// system.  Doing so greatly simplifies testing.  The hardware itself specifies
// which node it is.
//
// This is done through the role_pin
//

// The various roles supported by this sketch
typedef enum { role_remote = 1, role_led } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Remote", "LED Board"};

// The role of the current running sketch
role_e role;

//
// Payload
//

uint8_t button_states[num_button_pins];
uint8_t led_states[num_led_pins];

//
// Setup
//

float temperature;

void setup(void)
{
  //
  // Role
  //
  pinMode(0,OUTPUT);

  // set up the role pin
 // pinMode(role_pin, INPUT);
 // digitalWrite(role_pin,HIGH);
  delay(20); // Just to get a solid reading on the role pin

  // read the address pin, establish our role
// // if ( digitalRead(role_pin) )
//    role = role_remote;
// // else
// //   role = role_led;

  //
  // Print preamble
  //

  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/led_remote/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);

  //
  // Setup and configure rf radio
  //

  radio.begin();

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens a single pipes for these two nodes to communicate
  // back and forth.  One listens on it, the other talks to it.

//  if ( role == role_remote )
//  {
    radio.openWritingPipe(pipe_light);
    radio.openReadingPipe(1,pipe_tem);
//  }
//  else
//  {
//    radio.openReadingPipe(1,pipe);
//  }

  //
  // Start listening
  //

//  if ( role == role_led )
  //  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

//  radio.printDetails();

  //
  // Set up buttons / LED's
  //

  // Set pull-up resistors for all buttons
//  if ( role == role_remote )
//  {
//    radio.startListening();
//    int i = num_button_pins;
//    while(i--)
//    {
//      pinMode(button_pins[i],INPUT);
//      digitalWrite(button_pins[i],HIGH);
//    }
//  }

  // Turn LED's ON until we start getting keys
//  if ( role == role_led )
//  {
//    int i = num_led_pins;
//    while(i--)
//    {
//      pinMode(led_pins[i],OUTPUT);
//      led_states[i] = HIGH;
//      digitalWrite(led_pins[i],led_states[i]);
//    }
//  }

}

//
// Loop
//

void loop(void)
{

    int light = analogRead(0);
    Serial.print("light: ");
    Serial.println(light);
    delay(1000);
    radio.stopListening();
      //printf("Now sending...");
      bool ok = radio.write(&light,sizeof(int));

    radio.startListening();


    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      while (radio.available())
      {
        // Fetch the payload, and see if this was the last one.
        radio.read( &temperature, sizeof(float) );

        // Spew it
        //printf("Got buttons\n\r");

        // For each button, if the button now on, then toggle the LED
        float tem = temperature;
        Serial.print("temperature: ");
        Serial.println(tem);
      }
    }
  //}
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
