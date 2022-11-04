# Solo Blaster
I made a Han Solo replica blaster complete with an led to act as a blaster shot and a web interface for control through an access point mode or over wifi.

![image](https://github.com/scatmanspace/SoloBlaster/blob/master/Model/Render1.png)

## Description
For this project i used an existing model by [PortedtoReality](https://www.thingiverse.com/thing:1915956) on thingiverse. I used the Wemos D1 Mini to control the leds and detect trigger pulls along with running the accesss point and webserver.

### Model:
I modified the original model using Fusion 360 for various purposes:

* Redesigned Trigger mechanism to implement a button to activate leds on trigger pull.
* Replaced pegs with screwsto avoid glue and improve joints for ease of assembly and dissasembly.
* Made more space for electronics and added magazine system for ease of assembly.
* Redesigned barrell to hold leds and barrel detail to difuse leds.

All the parts were 3D printed in ABS and painted using spray paints and acrylic paints.

### Electronics:
These were the main components used:

* Wemos D1 Mini
* Wemos battery shield
* Generic 3.7v LiPo Battery
* WS 2812b Neopixel LEDs

### Software:
The software i wrote has many features i found necessary along with things that make the user experience better:

* Both access point and WiFi modes
* LittleFS file system for flash storage of the main HTML webpage
* Live battery status percentage and voltage monitoring using anolog input and voltage divider
* Power consumption limits for leds to increase battery life
* mDNS for easy connectivity though Blaster.local
* OTA updates for easy tweaking of the code

### Usage:
The blaster puts out its own WiFi Access point along with connecting to the wifi network that is given in its code. By default it is set to a normal red blaster and you can pull the trigger to fire it and once you let go it fades the color away.

If you hold the trigger for 8 seconds it switches between normal color mode and rainbow mode. 

To access the web interface you connect to its access point or connect to the same WiFi network it it connected to. Then you can access the web page by going to Blaster.local on the web browser of your choice.

The webpage is super simple, Lightside for preset Red, Darkside for Green, color picker to set any color you want and then press change color to update the color on the blaster. Rainbow button toggles rainbow mode, Fire Blaster does exactly as it says, and battery status takes you to the battery status page with the voltage and percentage displayed.

   
#### Homepage
<p align="center">
    <img width="400" src="https://user-images.githubusercontent.com/33579118/199863876-6709977a-383f-43aa-8d8b-a9a02196f4ff.png">
</p>

## Final Results
After a few weeks of work printing, reprinting, tweaking code, and painting the blaster was complete in time for halloween and these were the final results.

<p align="center">
    <img width="400" src="https://user-images.githubusercontent.com/33579118/199864760-6da95887-49ba-46c7-b1c9-57f72a6b8f58.png">
    <img width="400" src="https://user-images.githubusercontent.com/33579118/199864752-7b1be22f-a263-4f81-98a6-ac4342c91fba.png">
    <img width="450" src="https://user-images.githubusercontent.com/33579118/199866909-44fc1523-a59e-4e19-91f6-ceef32717054.JPEG">
</p>





