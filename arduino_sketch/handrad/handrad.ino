/*
  Created on 21.07.2022 by MathPi 
  https://github.com/MathPi14/Arduino-Estlcam-Handwheel-Communication/edit/main/README.md#used-components      
*/

#include <Wire.h>

#define JOYSTICK_X_GPIO A0              // ADC Pins the Joystick, speed, feed ovveride potis are connected to
#define JOYSTICK_Y_GPIO A1
#define JOYSTICK_Z_GPIO A2
#define SPEED_GPIO A6                  
#define SPEED_GPIO_INVERTED 1          // 1 if the Poti values shall be inverted 
#define FEED_GPIO A7                   
#define FEED_GPIO_INVERTED 1            // see above

#define X_LED_GPIO 2
#define Y_LED_GPIO 3
#define Z_LED_GPIO 4

#define X_SELECT_GPIO 5                 // Digital IO the corresp. buttons are connected to
#define Y_SELECT_GPIO 6
#define Z_SELECT_GPIO 7
#define PROGRAM_START_GPIO 8           
#define SPINDLE_START_GPIO 9           
#define OK_GPIO 10                   
#define STOP_GPIO 11

#define ONBOARD_LED_GPIO 13             // "-" onboard led

#define DEBUG 0

#define I2C_ADRESS 0b0000010            // I2C Slave Adress
#define I2C_CLOCK_FREQUENCY 10000       // I2C Clock Frequency
#define IDENT_STRING "Handrad 0001"     // Response to Mode3: Identify
#define SERIALNO_STRING "ABCD12345678"  // Response to Mode5: Serial Number
#define READ_FRAME_SIZE 9               // Number of incoming Bytes on I2C Receive event
#define SEND_FRAME_SIZE 15              // "-" outgoing Bytes on I2C Request event

const uint8_t CHALLENGE_VAL[] = {0x3B, 0x59, 0xE8, 0x2A, 0xE9, 0xB1, 0xBE, 0xD8, 0x00, 0x00, 0x00, 0x00}; //Response to Mode5: Challenge
volatile uint8_t rcv_frame[9];          // 9 byte buffer for the received frames, needs to be volatile because it is changed on hardware interrupts
uint8_t send_frame[15];                 // 15 byte array for the values to send, must not be volatile because they are only updated in the loop()
volatile uint16_t speed_data;                    // 2 byte value for the analog speed ovveride (on SPEED_GPIO)
volatile uint16_t feed_data;                     // same as above
volatile uint16_t x_data;
volatile int16_t y_data;
volatile uint16_t z_data;
volatile uint8_t crc;                            // 1 byte value for the checksum (byte 15)
volatile bool program_start_btn_pressed;
volatile bool spindle_start_btn_pressed;
volatile bool ok_btn_pressed;
volatile bool stop_btn_pressed;
volatile uint8_t axis_selected;

// setup called once on program start
void setup() {
  Wire.begin(I2C_ADRESS);               // initialize device as peripheral with I2C_ADRESS (specified by Estlcam)
  Wire.setClock(I2C_CLOCK_FREQUENCY);   // set clock frequency to I2C_CLOCK_FREQUENCY (specified by Estlcam)
  Wire.onRequest(RequestEventHandler);  // register RequestEventHandler on Request Events from the Master (Estlcam)
  Wire.onReceive(ReceiveEventHandler);  // register ReceiveEventHandler on Receive Events from the Master (Estlcam)
  pinMode(PROGRAM_START_GPIO, INPUT_PULLUP);                    // initialize Button and Led GPIOs
  pinMode(SPINDLE_START_GPIO, INPUT_PULLUP);
  pinMode(OK_GPIO, INPUT_PULLUP);
  pinMode(STOP_GPIO, INPUT_PULLUP);
  pinMode(X_SELECT_GPIO, INPUT_PULLUP);
  pinMode(Y_SELECT_GPIO, INPUT_PULLUP);
  pinMode(Z_SELECT_GPIO, INPUT_PULLUP);
  pinMode(X_LED_GPIO, OUTPUT);
  pinMode(Y_LED_GPIO, OUTPUT);
  pinMode(Z_LED_GPIO, OUTPUT);
  pinMode(ONBOARD_LED_GPIO, OUTPUT);
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Running");
#endif
}

// loop is called constantly
// handle mode state machine specified by Estlcam
void loop() {
  buffer_data();
  switch (rcv_frame[0]) 
  {
    case 0:
      delay(100);
      break;

    case 1:
      update_data();                                // update mode specific data
      break;

    case 2:
      update_data();                                // update mode specific data
      update_status_led();
      break;

    case 3:
      identify();
      break;

    case 4:
      challenge();
      break;

    case 5:
      serialnumber();
      break;
  }
}

//
void update_status_led() {
  switch (rcv_frame[1]) {
    case 0:
      digitalWrite(X_LED_GPIO, 0);
      digitalWrite(Y_LED_GPIO, 0);
      digitalWrite(Z_LED_GPIO, 0);
      break;

    case 1:
      digitalWrite(X_LED_GPIO, 1);
      digitalWrite(Y_LED_GPIO, 0);
      digitalWrite(Z_LED_GPIO, 0);
      break;

    case 2:
      digitalWrite(X_LED_GPIO, 0);
      digitalWrite(Y_LED_GPIO, 1);
      digitalWrite(Z_LED_GPIO, 0);
      break;

    case 4:
      digitalWrite(X_LED_GPIO, 0);
      digitalWrite(Y_LED_GPIO, 0);
      digitalWrite(Z_LED_GPIO, 1);
      break;
  }
}
// called on a i2c Request from the Master (Estlcam)
// interrupts the loop() thus needs to be as fast as possible
// increment Frame number and checksum according to Specification from Estlcam
void RequestEventHandler() {
  if (rcv_frame[0] != 0) {
    send_frame[12] = rcv_frame[0];                // set 12 bit to match selected mode
    send_frame[13] += 1;                          // increment frame on 13 byte
    send_frame[14] = crc_calc();                  // calculate checksum on 14 byte
    Wire.write(send_frame, sizeof(send_frame));   // Write new frame to the Master
  }
#ifdef DEBUG
  Serial.print("received : ");
  for (int i = 0; i < READ_FRAME_SIZE; i++) {
    if ((int)rcv_frame[i] < 20) {
      Serial.print("0");
    }
    Serial.print(rcv_frame[i], HEX);
    Serial.print(" ");
  }
  Serial.print("; sending: ");
  for (int i = 0; i < SEND_FRAME_SIZE; i++) {
    if ((int)send_frame[i] < 20) {
      Serial.print("0");
    }
    Serial.print(send_frame[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
#endif
}

// called on a i2c Request from the Master (Estlcam)
// interrupts the loop() thus needs to be as fast as possible and needs a volatile buffer to write data to
// save byte 1 of the received frame (specifies the  mode used in the loop() state machine)
void ReceiveEventHandler(int rcv_frame_size) {
  if (rcv_frame_size == READ_FRAME_SIZE) {        // check if we receive 9 bytes (according to specification from Estlcam)
    for (int i = 0; i < READ_FRAME_SIZE; i++) {   // save received data byte-by-byte to the volatile buffer variable
      rcv_frame[i] = Wire.read();
    }
  }
#ifdef DEBUG
  Serial.println("Receive Request");
#endif
}

void update_data() {
  send_frame[0] = 0xFF;                     //byte 1 = 255 according to spec
  if (axis_selected > 0) {
    send_frame[0] &= ~(1 << axis_selected - 1) | (0 << axis_selected - 1);
  }

  send_frame[1] = 0xFF;                     //set bits 2, 4,5,6,7 of byte 2 to 1 beforehand
  if (!program_start_btn_pressed) {
    send_frame[1] &= ~(1 << 0) | (0 << 0); //flip bit 0 if PROGRAM_START_GPIO is high
  }
  if (!spindle_start_btn_pressed) {
    send_frame[1] &= ~(1 << 1) | (0 << 1); //flip bit 1 if SPINDLE_START_GPIO is high
  }
  if (!ok_btn_pressed) {
    send_frame[1] &= ~(1 << 3) | (0 << 3); //flip bit 3 if OK_GPIO is high
  }
  if (stop_btn_pressed) {
    send_frame[1] &= ~(1 << 2) | (0 << 2); // stop
  }

  send_frame[2] = lowByte(speed_data);
  send_frame[3] = highByte(speed_data);
  send_frame[4] = lowByte(feed_data);
  send_frame[5] = highByte(feed_data);
  send_frame[6] = lowByte(x_data);
  send_frame[7] = highByte(x_data);
  send_frame[8] = lowByte(y_data);
  send_frame[9] = highByte(y_data);
  send_frame[10] = lowByte(z_data);
  send_frame[11] = highByte(z_data);
}

// called once after a Request Event fired and we are in mode 1
// update the data to write out to the master
void buffer_data() {
  program_start_btn_pressed = digitalRead(PROGRAM_START_GPIO);
  spindle_start_btn_pressed = digitalRead(SPINDLE_START_GPIO);
  ok_btn_pressed = digitalRead(OK_GPIO);
  stop_btn_pressed = digitalRead(STOP_GPIO);
  if (digitalRead(X_SELECT_GPIO) == 0) {
    axis_selected = 1;
  }
  else if (digitalRead(Y_SELECT_GPIO) == 0) {
    axis_selected = 2;
  }
  else if (digitalRead(Z_SELECT_GPIO) == 0) {
    axis_selected = 3;
  }
  else {
    axis_selected = 0;
  }
  speed_data = SPEED_GPIO_INVERTED ? 65535 - (analogRead(SPEED_GPIO) * 64) : analogRead(SPEED_GPIO) * 64;
  feed_data = FEED_GPIO_INVERTED ? 65535 - (analogRead(FEED_GPIO) * 64) : analogRead(FEED_GPIO) * 64;
  x_data = analogRead(JOYSTICK_X_GPIO) * 64;// convert the returning 10 bit uint from analogRead() to a 16 bit uint 
  y_data = analogRead(JOYSTICK_Y_GPIO) * 64;
  z_data = analogRead(JOYSTICK_Z_GPIO) * 64;
}

// called once after a Request Event fired and we are in mode 3
// update the data to write out to the master
void identify() {
  for (int i = 0; i < 12; i++) {
    send_frame[i] = IDENT_STRING[i];
  }
};

// called once after a Request Event fired and we are in mode 4
// update the data to write out to the master
void challenge() {
  for (int i = 0; i < 12; i++) {
    send_frame[i] = CHALLENGE_VAL[i];
  }
}

// called once after a Request Event fired and we are in mode 5
// update the data to write out to the master
void serialnumber() {
  for (int i = 0; i < 12; i++) {
    send_frame[i] = SERIALNO_STRING[i];
  }
}

// calculate the checksum according to specification from Estlcam
uint8_t crc_calc() {
  crc = 0;                                  //clear checksum
  for (int i = 0; i < 14; i++) {            //for bytes 1-14: XOR next byte with current checksum, then increment the latter
    crc ^= send_frame[i];
    crc++;
  }
  return crc;
}
