# IOT Home-School Dashboard

### What is it?
* This Dashboard consists of two main parts: an [ESP8266](https://en.wikipedia.org/wiki/ESP8266) _(Microcontroller with Wifi)_ & a [Raspberry Pi](https://www.raspberrypi.org/help/what-%20is-a-raspberry-pi/) _(a low powered altoid tin can sized computer)_
* The ESP8266 is used as an alarm clock to connect to your home network
* The Pi (Raspberry Pi) is beinng used as a home server to host a webapp to track a child's performance and to set different alarms for the ESP8266
* Both devices will have to be connected to your home network (wifi) to interact and function as intended

### Hiccups    
* To program the ESP8266 using the [Arduino IDE](https://docs.arduino.cc/software/ide-v2) you will need to install the appropriate driver [HERE](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads) to succssfully detect the Micrcontrollr when it's connected to the computer/laptop via USB.
* For the Arduino IDE to find the ESP8366 in the list of boards, please do the following steps below in the IDE window:
  1. Go to File > Preferences.
  2. Enter _"http://arduino.esp8266.com/stable/package_esp8266com_index.json"_ into the “Additional Board Manager URLs” field as shown in the figure below. Then, click the “OK” button

     ![image](https://github.com/NerdboyQ/IOT_HomeSchoolDashboard/assets/20153392/b8d3d04c-e55d-4e6e-96b3-b3d7ef1a13f8)
  3. Go to Tools > Board > Boards Manager…  
     ![image](https://github.com/NerdboyQ/IOT_HomeSchoolDashboard/assets/20153392/8d1b92d1-cda3-47ac-8350-a57adae9be3e)

  4. Scroll down, select the ESP8266 board menu and install “esp8266 by ESP8266 Community”, as shown in the figure below.
     ![image](https://github.com/NerdboyQ/IOT_HomeSchoolDashboard/assets/20153392/d2220e19-5335-497b-94e1-876a3e49e98c)

  5. Go to Tools > Board and choose your ESP8266 board. Then, re-open your Arduino IDE.

### Parts 
* Laptop/Desktop
* Raspberry Pi (Preferably a model with wifi capabilities)
   * Power supply 
   * Monitor (or a display with an hdmi port)
   * HDMI cable
   * Keyboard _(a mouse isn't required but it may make things easier)_
   * Mico SD Card
* ESP8266
   * Micro USB cable for programming
   * Small Speaker
   * Wires
   * 2 x OLED Display Module 128 x 64 OLED Display I2C 0.96inch
   * Soldering Iron & solder
   * 1m NeoPixel LED strip

   > NOTE: I 3d printed the alarm clock to look like a bus, but the componenets can be placed in any design you'd prefer.
   
### HTTP Requests   
* Commands can be sent to the ESP8266 via "HTTP requests", which it how in internet essentially works.
* When you go to a website, e.g. Google or Youtube, you the address most likely starts with _"http://"_
> NOTE :: In most modern browsers like Chrome or Safari, you won't see this unless you click on the address bar, copy the address, and then paste it in any text editer (e.g. Word, Docs, Notepad, etc).
* When those addresses/websites are typed into a browser it is makeing a "request" to get the website and display it in your browser window.
* If you ever watch the address bar when you click on different youtube timestamps in a video, you'll notice the end of the address will change. This is how you request for specific information from that website, and it then responds with some action i.e. a new point in the video.
* The "address" is seen as text like "Youtube.com", but it actually associated with an IP Address for where on the internet this site is being ran or "served", i.e. the address to the computer that's the "server" of the site you've requested.
> NOTE :: When an address/website isn't found you will receive a "404 Error", meaning you're requesting the address of a computer that was not found on the internet to "serve" the website you entered 
* Every device connected to an internet (the internet everyone uses around the world, or a home/business private internet) has an "IP Address" : Internet Protocol Address
* The ESP8266, in this project, is serving an address that will respond to these http requests sent to it by the different devices on your home internet connection. Below are a few examples of requests that will work in our project to interact with our IOT BUS:   
  > NOTE :: _The IP Address I am using in this request below, are just for example. Your IP Address will most likely be different, as it is set by your internet router (although you can force it to be a specific address)._
  
  `http://192.168.1.121/current_led_state`   
  this will return information stating if the LED's are ON or OFF along with the current color setting at the time of the request   
  `http://192.168.1.121/led?state=1&irgb=ff00ff`    
  this will set the led's on as well as setting the led color using RGB Hex code.
