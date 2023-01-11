# Table of contents
* [General Description](#general-description)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
* [Used Components](#used-components)

## General Description
This is an implementation of the Estlcam handwheel i2c Communication Protocol (https://www.estlcam.de/DIN_Detail.php) for Arduino Boards. 
It supports all the functionality of the originial handwheel:
* 3/4 - axis joystick for rough positioning 
* encoder (handwheel) which allows very fine positioning
* spindle, program start and enter (probing) buttons 
* feed and speed override potentiometers

the following 'hidden' features are implemented as well:
* 3 led which indicate which of the axis is currently active to move on the handwheel
* 3 buttons to change the currently active axis 

Tested with Estlcam V11.244
	
## Prerequisites
To get started, you need the following:
* an original Estlcam Board with a mini-DIN Port
* mini-Din cable (6 Pin, male connector). 2-3 m length recommended, depending on the size of your cnc. Super long cables (>5 m) might cause EMC issues
* an arduino (original or compatible) board with 5 V operating voltage, 7 ADC Pins including SDA and SCL for the i2c communication functionality and 9 digital Pins like the Arduino Nano 
* analog Joystick (3 or 4 axis, 10kohm)
* handwheel encoder
* 2x 10kohm potentiometers
* 6x push buttons. No toggle switches!
* 3x 5 V LED (or 12 V with appropiate resistor)
* plastic/ esd safe housing
* a small cable gland (M16/ PG9 if you use my 3d printed housing)
* 2 M4 brass inserts and short screws (if you use my 3d printed housing)

You also need basic soldering equipment, cables, shrink tubing, hot glue etc.
	
## Installation
### Housing/ Electronics
* You need to design the housing of your handwheel first. I will update this repo with my components/ printable .stl files/ installation pictures soon after i solved some teething problems
* start with installing all the components (apart from the arduino board for now). Carefully solder cables to the buttons', potentiometers' etc. VCC (+) and GND (-) Pins. It's recommended to either daisy chain them from the DIN - Cable to all the components or to use wago connection clamps
* follow up with the DIN-Cable. First connect the dedicated wires to the encoder +- and the VCC(+), GND(-), SDA/ SCL to the arduino (check https://www.estlcam.de/DIN_Detail.php & the pinout of your Arduino Board. SDA/ SCL Pins are specific ADC Pins)
* solder cables to the components' data pins and start connecting them to the arduino:
* begin with the LED output pins. Its recommended to use digital Pins 1-3 to keep things organized as we need to configure them as 'output' later in software
* the 6 buttons can be connected to GPIO 4-10
* finally connect the potentiometer and joystick output to any ADC pin left

### Software (Arduino)
After soldering everything together, you need to adjust the software and download it to your arduino. After you copy and pasted the code to your Arduino IDE, go through the defines and change the pin configuration according to your hardware installation

```
#define SPEED_GPIO A0                   
#define SPEED_GPIO_INVERTED 1          
#define FEED_GPIO A1                   
#define FEED_GPIO_INVERTED 1            
#define JOYSTICK_X_GPIO A2              
#define JOYSTICK_Y_GPIO A3
#define JOYSTICK_Z_GPIO A6

#define X_LED_GPIO 2
#define Y_LED_GPIO 3
#define Z_LED_GPIO 4

#define PROGRAM_START_GPIO 9           // GPIO the program start button is connected
#define SPINDLE_START_GPIO 10           // "-" spindle start button
#define OK_GPIO 11                   // "-" ok button 
#define X_SELECT_GPIO 5
#define Y_SELECT_GPIO 6
#define Z_SELECT_GPIO 7
#define STOP_GPIO 8
#define ONBOARD_LED_GPIO 13             // "-" onboard led
```

Then upload the software to your arduino. 

### Software (Estlcam)
After the electrical work is done and the Code is running on your Arduino, you can connect the mini-Din cable to the Estlcam Board and reprogram the latter. You know that everything worked out, if there's a 'Handrad 0001 ...' line appearing in the log inside Estlcam. If it didn't, there is most likely something wrong with the SDA/ SCL Pins.

If it works, you can go to the controls tab inside Estlcam and test if everything works as intended. If the Joystick ADC are inverted, you can simply change it in Estlcam directly. If the Buttons, LED or feed/ speed potentiometers are inverted or not assigned correctly, go back to the previous step 

## Used Components
* joystick (https://de.aliexpress.com/item/4001270851701.html?spm=a2g0o.order_list.order_list_main.28.3ab85c5fBNVLpg&gatewayAdapt=glo2deu)
* encoder (5V 4Pin, 60 mm) (https://de.aliexpress.com/item/32949618549.html?spm=a2g0o.order_list.order_list_main.23.3ab85c5fBNVLpg&gatewayAdapt=glo2deu)
* buttons (https://de.aliexpress.com/item/1005003302861259.html?spm=a2g0o.order_list.order_list_main.15.3ab85c5fBNVLpg&gatewayAdapt=glo2deu)
* 5V led (https://de.aliexpress.com/item/1005003751324053.html?spm=a2g0o.order_list.order_list_main.16.3ab85c5fBNVLpg&gatewayAdapt=glo2deu) 
* 10 kOhm potentiometers (https://de.aliexpress.com/item/4000971762879.html?spm=a2g0o.order_list.order_list_main.33.3ab85c5fBNVLpg&gatewayAdapt=glo2deu)
* Arduino Nano (https://de.aliexpress.com/item/1005002867053794.html?spm=a2g0o.productlist.main.1.133a77caLz0lR6&algo_pvid=95d24bee-27b9-4d5b-8cd0-02c4f537d310&aem_p4p_detail=202301110712423199476066881400004107065&algo_exp_id=95d24bee-27b9-4d5b-8cd0-02c4f537d310-0&pdp_ext_f=%7B%22sku_id%22%3A%2212000022573065020%22%7D&pdp_npi=2%40dis%21EUR%215.46%214.31%21%21%21%21%21%402100b18f16734499623697280d070b%2112000022573065020%21sea&curPageLogUid=7E5HYHNNMxJW&ad_pvid=202301110712423199476066881400004107065_1&ad_pvid=202301110712423199476066881400004107065_1)
