# [GERMAN VERSION](https://github.com/MathPi14/Arduino-Estlcam-Handwheel-Communication/blob/main/README_DE.md)

# Table of contents
* [General Description](#general-description)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
* [Used Components](#used-components)

## General Description
This is my implementation of the Estlcam handwheel i2c communication protocol (https://www.estlcam.de/DIN_Detail.php) for Arduino Boards. 
Its made for users of the original Estlcam Hardware.

The Code supports all the features the originial handwheel provides:
* 3/4 - axis joystick for rough positioning 
* encoder (handwheel) which allows very fine positioning
* spindle, program start and enter (probing) buttons 
* feed and speed override potentiometers

the following 'hidden' features are implemented as well:
* 3 led which indicate which of the axis is currently active to move on the handwheel
* 3 buttons to change the currently active axis 
* emergency stop (triggers program pause)

![top](https://user-images.githubusercontent.com/69339442/213932798-1af69cf4-892e-43d9-bc12-5184e7064199.png)
![inside](https://user-images.githubusercontent.com/69339442/213932805-338ffd80-9654-4688-8ebe-09921d74ee7b.png)
![inside_2](https://user-images.githubusercontent.com/69339442/213932818-617348a7-9fe8-44db-abf7-a462b88048d6.png)
![pinout](https://user-images.githubusercontent.com/69339442/213933733-bbf50029-dfe7-4c3b-87aa-6fddc10b4ca6.png)


Tested with Estlcam V11.244. The hidden features may not be supported by other Estlcam Versions.
	
## Prerequisites
To get started, you need the following:
* an original Estlcam board with a mini-DIN port
* mini-DIN cable (6 Pin, male connector). 2-3 m length recommended, depending on the size of your cnc. Super long cables (>5 m) might cause EMC issues
* an arduino (original or compatible) board with 5 V operating voltage, 7 ADC Pins including SDA and SCL for the i2c communication functionality and 9 digital Pins like the Arduino Nano 
* analog joystick (3 or 4 axis, 10kohm)
* handwheel encoder
* 2x 10kohm potentiometers
* 6x push buttons. No toggle switches!
* 3x 5 V LED (or 12 V with appropiate resistor)
* emergency stop button
* housing (see attached .stl files)
* a small cable gland (M16/ PG9 if you use my 3d printed housing)
* 4 M4 brass inserts and short screws (if you use my 3d printed housing)

You also need basic soldering equipment, cables, shrink tubing, hot glue etc.
	
## Installation
### Housing/ Electronics
* start with installing the led, push-buttons, the encoder and potentiometers (not the joystick, arduino, cable gland and emergency stop for now). Carefully solder (not too short) cables to the components. It's recommended to use the screw terminals of the handwheel encoder as bus for the components and the DIN Cable as well. This way, the DIN cable remains detatchable
* secure the cables with hot glue
* solder the cables to the arduino according the attached photo of the pinout. Use two short cables for the SDA and SCL so that we can connect them to the DIN cable with Wago clamps later (the cable remains detachable, see #1)
* attach the arduino to the insert and sort the cables with the cable comb
* insert the DIN cable, connect enc + and - as well as VCC and GND to the encoder (handwheel) and SDA/ SCL to the arduino


### Software (Arduino)
Flash the Arduino via the IDE. The onboard TX LED blinks when the program is running. 
If you use any other arduino board or if your pin assignment differs from mine, you may need to change the following defines 

```
#define JOYSTICK_X_GPIO A0            
#define JOYSTICK_Y_GPIO A1
#define JOYSTICK_Z_GPIO A2
#define SPEED_GPIO A6                  
#define SPEED_GPIO_INVERTED 1          
#define FEED_GPIO A7                   
#define FEED_GPIO_INVERTED 1            


#define X_LED_GPIO 2
#define Y_LED_GPIO 3
#define Z_LED_GPIO 4

#define X_SELECT_GPIO 5
#define Y_SELECT_GPIO 6
#define Z_SELECT_GPIO 7          
#define SPINDLE_START_GPIO 8    
#define PROGRAM_START_GPIO 9
#define OK_GPIO 10                   
#define STOP_GPIO 11         

#define ONBOARD_LED_GPIO 13           
```

### Software (Estlcam)
After the electrical work is done and the Code is running on your Arduino, you can connect the mini-DIN cable to the Estlcam Board and reprogram the latter. You know that everything worked out, if there's a 'Handrad 0001 ...' line appearing in the log inside Estlcam. If it didn't, there is most likely something wrong with the SDA/ SCL Pins.

If it works, you can go to the controls tab inside Estlcam and test the components. If the Joystick ADC are inverted, you can simply change it in Estlcam directly. If the Buttons, LED or feed/ speed potentiometers are inverted or not assigned correctly, go back to the previous step and change the defines

## Used Components
* joystick (https://de.aliexpress.com/item/4001270851701.html?spm=a2g0o.order_list.order_list_main.28.3ab85c5fBNVLpg&gatewayAdapt=glo2deu)
* encoder (5V 4Pin, 60 mm) (https://de.aliexpress.com/item/32949618549.html?spm=a2g0o.order_list.order_list_main.23.3ab85c5fBNVLpg&gatewayAdapt=glo2deu)
* buttons (https://de.aliexpress.com/item/1005003302861259.html?spm=a2g0o.order_list.order_list_main.15.3ab85c5fBNVLpg&gatewayAdapt=glo2deu)
* 5V led (https://de.aliexpress.com/item/1005003751324053.html?spm=a2g0o.order_list.order_list_main.16.3ab85c5fBNVLpg&gatewayAdapt=glo2deu) 
* 10 kOhm potentiometers (https://de.aliexpress.com/item/4000971762879.html?spm=a2g0o.order_list.order_list_main.33.3ab85c5fBNVLpg&gatewayAdapt=glo2deu)
* emergency stop (16 mm diameter) (https://de.aliexpress.com/item/33014058927.html?spm=a2g0o.order_list.order_list_main.17.77675c5fkKyHby&gatewayAdapt=glo2deu)
* Arduino Nano (https://de.aliexpress.com/item/1005002867053794.html?spm=a2g0o.productlist.main.1.133a77caLz0lR6&algo_pvid=95d24bee-27b9-4d5b-8cd0-02c4f537d310&aem_p4p_detail=202301110712423199476066881400004107065&algo_exp_id=95d24bee-27b9-4d5b-8cd0-02c4f537d310-0&pdp_ext_f=%7B%22sku_id%22%3A%2212000022573065020%22%7D&pdp_npi=2%40dis%21EUR%215.46%214.31%21%21%21%21%21%402100b18f16734499623697280d070b%2112000022573065020%21sea&curPageLogUid=7E5HYHNNMxJW&ad_pvid=202301110712423199476066881400004107065_1&ad_pvid=202301110712423199476066881400004107065_1)
