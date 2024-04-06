
# Table of contents
* [Allgemeines](#allgemeines)
* [Anforderungen](#anforderungen)
* [Installation](#installation)
* [Einkaufsliste](#einkaufsliste)

## Allgemeines
Dieser Code implementiert das Estlcam Handrad-Kommunikationsprotokoll (https://www.estlcam.de/DIN_Detail.php) für Arduino Boards.
Damit kann das Handrad für original Estlcam Klemmenadapter nachgebaut werden.

Es werden alle Features des originalen Handrads unterstützt:
* 3 achs joystick für das grobe Positionieren
* Encoder (Handrad) für das feine Antasten
* Spindel, Programm start und Enter (Antasten) Druckknöpfe
* Vorschub und Drehzahl Potis

Folgende Funktionen werden zusätzlich unterstützt:
* 3 LED zur Anzeige, welche Achse gerade mit dem Handrad verfahren wird
* 3 Buttons zur Auswahl der aktuellen Achse
* Not-Halt (triggert den Programm "Pause" Knopf in Estlcam)

![top](https://user-images.githubusercontent.com/69339442/213932798-1af69cf4-892e-43d9-bc12-5184e7064199.png)
![inside](https://user-images.githubusercontent.com/69339442/213932805-338ffd80-9654-4688-8ebe-09921d74ee7b.png)
![inside_2](https://user-images.githubusercontent.com/69339442/213932818-617348a7-9fe8-44db-abf7-a462b88048d6.png)
<img width="1916" alt="Unbenannt" src="https://github.com/MathPi14/Arduino-Estlcam-Handwheel-Communication/assets/69339442/2b1e692b-f871-46e6-a83f-bd957b928ef0">




Getestet mit Estlcam V11.244. Die zusätzlichen Funktionen funktionieren evtl. nicht mit anderen Versionen.
	
## Anforderungen
Zunächst muss die Hardware beschafft werden:
* 1x Estlcam Klemmenadapter mit einer mini-DIN Buchse
* 1x mini-DIN kabel (6 pin Stecker bzw. männlicher Anschluss). Ich empfehle mindestens 2-3 m Länge. Ab 5 m werden evtl. EMV Probleme auftreten.
* 1x Arduino Board z.B. Nano (original oder after market) mit 5 V Nennspannung, 7 ADC Pins (SDA und SCL für i2c inklusive), 9 digitale IO
* 1x Analoger joystick (3 oder 4 axis, 10 kohm)
* 1x Handrad mit Encoder
* 2x 10kohm Potis
* 6x Druckknöpfe (keine Rastfunktion!)
* 3x 5 V LED (or 12 V mit Widerstand)
* 1x Gehäuse z.B. 3D gedruckt (siehe Dateien)
* 1x Kabelverschraubung (M16/ PG9 bei meinem Gehäuse)
* 4x M4 Gewindeeinsatz und kurze M4 Schrauben (bei meinem Gehäuse)

Außerdem wird essentielles Werkzeug zum Löten (Lötstation/ Kolben, Zinn, Kabel, Schrumpfschläuche, Heißklebepistole etc.) benötigt	

## Installation
### Gehäuse
Falls meine 3D Dateien genutzt werden, empfehle ich die folgende Vorgehensweise:
* Druck mit einer 0.4 mm Nozzle, 0.2-0.25 mm Schichthöhe, 2-3 Schleifen, Infill egal
* Am besten startet man mit der Installation der LED, des Handrads, der Knöpfe, Potis und der Kabelverschraubung. Dort kann man gleich (ruhig etwas längere) Kabel anlöten. Die Schraubklemmen am Handrad (Encoder) kann man als Sammelpunkt für GND und Versorgungsspannung nutzen (jew. 1 Kabel zu Arduino/ Komponenten). Dann bleibt das DIN Kabel abnehmbar
* Zur Sicherheit sollten alle Knöpfe einmal mit dem Multimeter auf Funktion überprüft werden
* Danach Kabel an die Kontakte der Joystick- X und Y Potis löten (im ausgebauten Zustand), anschließend einbauen
* Den Not-Halt Dreipunktschalter sollte man zuletzt einbauen
* Bevor der Arudino eingebaut wird, am besten alle Kabel zur Ordnung mit Heißkleber am Gehäuse fixieren
* Anschließend alle Kabel, welche zum Arduino gehen, einzeln ablängen, abisolieren, verdrillen und ensprechend dem angefügten Bild an den Arduino löten
* Dabei an die SDA und SCL Pins (ADC 4 & 5) zunächst nur zwei kurze Kabel löten. Diese werden später mit z.B. Wagoklemmen mit dem DIN Kabel verbunden (was dann abnehmbar bleibt, siehe Punkt 1)
* Das DIN Kabel über die Kabelverschraubung einführen, abisolieren und die Adern durchprüfen & -nummerieren. GND, VCC, Encoder +, Encoder - an den Schraubklemmen am Encoder anschließen. SDA und SCL mit den kurzen Kabeln, welche man an den Arduino gelötet hat, z.B. über Wago Klemmen verbinden.
* Zum Schutz des Innenraums den Arduino zuletzt auf die gedruckte Einsatzplatte clipsen und die einzelnen Kabel in den Kämmen sortieren

### Software (Arduino)
Nachdem alles verbaut, verlötet und getestet ist, kann die Software über die Arduino IDE aufgespielt werden. Falls die Kabel nicht so, wie in dem angefügten Pinout verlötet wurden oder ein anderes Board verwendet wird, müssen folgende Defines ensprechend geändert werden:

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
Nun kann man das Handrad schon an den Klemmenadapter anschließen. Nach dem erneuten Programmieren der Estlcam Steuerung muss die Meldung "Zusatzmodul Handrad 0001 erkannt" erscheinen. Falls dem nicht so ist, stimmt vermutlich etwas mit dem SDA/ SCL Pins nicht.

Vor dem Testen sollte man ein letztes mal verifizieren, dass alle Buttons, LED usw. richtig angeschlossen sind. Dazu in den "Bedienelemente" Tab wechseln und bei laufender Steuerung systematisch alle Knöpfe, Potis etc. drücken/ bewegen und auf die entsprechende Anzeige achten. Falls die Potis invertiert sind oder die Belegung der Knöpfe nicht passt, kann man diese einfach über die Defines aus dem vorherigen Unterkapitel ändern.

## Einkaufsliste
* Joystick (https://de.aliexpress.com/item/4001270851701.html?spm=a2g0o.order_list.order_list_main.28.3ab85c5fBNVLpg&gatewayAdapt=glo2deu)
* Handrad (5V 4Pin, 60 mm) (https://de.aliexpress.com/item/32949618549.html?spm=a2g0o.order_list.order_list_main.23.3ab85c5fBNVLpg&gatewayAdapt=glo2deu)
* Druckknöpfe (https://de.aliexpress.com/item/1005003302861259.html?spm=a2g0o.order_list.order_list_main.15.3ab85c5fBNVLpg&gatewayAdapt=glo2deu)
* 5V led (https://de.aliexpress.com/item/1005003751324053.html?spm=a2g0o.order_list.order_list_main.16.3ab85c5fBNVLpg&gatewayAdapt=glo2deu) 
* 10 kOhm Potis (https://de.aliexpress.com/item/4000971762879.html?spm=a2g0o.order_list.order_list_main.33.3ab85c5fBNVLpg&gatewayAdapt=glo2deu)
* Not Dreipunktschalter (16 mm) (https://de.aliexpress.com/item/33014058927.html?spm=a2g0o.order_list.order_list_main.17.77675c5fkKyHby&gatewayAdapt=glo2deu)
* Arduino Nano (https://de.aliexpress.com/item/1005002867053794.html?spm=a2g0o.productlist.main.1.133a77caLz0lR6&algo_pvid=95d24bee-27b9-4d5b-8cd0-02c4f537d310&aem_p4p_detail=202301110712423199476066881400004107065&algo_exp_id=95d24bee-27b9-4d5b-8cd0-02c4f537d310-0&pdp_ext_f=%7B%22sku_id%22%3A%2212000022573065020%22%7D&pdp_npi=2%40dis%21EUR%215.46%214.31%21%21%21%21%21%402100b18f16734499623697280d070b%2112000022573065020%21sea&curPageLogUid=7E5HYHNNMxJW&ad_pvid=202301110712423199476066881400004107065_1&ad_pvid=202301110712423199476066881400004107065_1)
