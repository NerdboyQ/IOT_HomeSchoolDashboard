#include <Adafruit_NeoPixel.h>

#include "Music_7Octaves.h"
#include "HomeSecurity.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display_1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 display_2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

MusicGenerator music_generator = MusicGenerator();

#define SPEAKER_PIN        D0 // On Trinket or Gemma, suggest changing this to 1
#define LCD_SCL_PIN        D1 // On Trinket or Gemma, suggest changing this to 1
#define LCD_SDA_PIN        D2 // On Trinket or Gemma, suggest changing this to 1

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN1        D4 // On Trinket or Gemma, suggest changing this to 1
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 6 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN1, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 50 // Time (in milliseconds) to pause between pixels


#define BUTTON_PIN        D3 // Interrupt Pin for stopping alarms/changing display info/color
bool isClicked = false;
byte buttonClickCount = 0;
const unsigned long short_press = 500;
const unsigned long long_press = 2000;
unsigned long last_tap = 0;

const uint16_t max_brightness = 200;
const uint16_t max_red = 255;
const uint16_t max_green = 255;
const uint16_t max_blue = 255;
bool LED_STATE = false;
union IRGB {
  uint64_t payload;
  struct{
    uint16_t b : 8;
    uint16_t g : 8;
    uint16_t r : 8;
    uint16_t i : 8;
  } vals;
};

IRGB current_irgb = {0xc8ffffff};
IRGB alarm_irgb = current_irgb;

char* alarm_name;
bool alarm_state_active = false;
bool interrupt_was_clicked = false;
unsigned long last_interrupt_time, time_delta;

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = _SSID;
const char* password = _PASSWORD;

ESP8266WebServer server(80);
String server_response = "";


// ISR - Interrupt Service Routine
ICACHE_RAM_ATTR void handle_button_state_change(){
  if (alarm_state_active){
    alarm_state_active = false;
  } else {
    if (interrupt_was_clicked){
      interrupt_was_clicked = false;
      // handle multiple press conditions
      /*time_delta = millis() - last_interrupt_time;
      if (time_delta < short_press){
        setPixelRGB(max_brightness, 255, 0, 0);
        delay(500);
        setPixelRGB(max_brightness, max_red, max_green, max_blue);
      }
      else if (time_delta > short_press &&  time_delta < long_press){
        setPixelRGB(max_brightness, 0, 255, 0);
        delay(500);
        setPixelRGB(max_brightness, max_red, max_green, max_blue);
      }
      else if (time_delta > long_press){
        setPixelRGB(max_brightness, 0, 0, 255);
        delay(500);
        setPixelRGB(max_brightness, max_red, max_green, max_blue);
      }*/
      
    } else {
      interrupt_was_clicked = true;
      last_interrupt_time = millis();
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  Serial.println();
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");  
      // tone(SPEAKER_PIN,N_C4, 100);      
  }
  Serial.println();

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );

  server.begin();
  Serial.println("NodeMCU Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  // server address functions
  server.on("/alarm", play_alarm);
  server.on("/current_led_state", get_led_state);
  server.on("/led", set_led_state);

  pinMode(BUTTON_PIN,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handle_button_state_change, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  setPixelRGB(25,max_red,max_green,max_blue);
  current_irgb.vals.i = 25;
  current_irgb.vals.r = max_red;
  current_irgb.vals.g = max_green;
  current_irgb.vals.b = max_blue;
  LED_STATE = true;

  if(!display_1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  if(!display_2.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  set_default_displays();
  
  // music_generator.PlayMelody1(music_generator.CScale_Notes, sizeof(music_generator.CScale_Notes)/sizeof(music_generator.CScale_Notes[0]));
  // kmusic_generator.PlayMelody(music_generator.CScale_melody, 1);
}

void loop() {
  if (Serial.available() > 0){
    char in_char = Serial.read();
    Serial.println(in_char);
    if (in_char == 'r'){
      Serial.print("The Note Count: ");
      Serial.println(music_generator.CScale_melody.note_count);
      Serial.print("The first freq: ");
      Serial.println(music_generator.CScale_melody.notes[0].key);
      
      music_generator.PlayMelody1(music_generator.DrankinPatna_Notes, sizeof(music_generator.DrankinPatna_Notes)/sizeof(music_generator.DrankinPatna_Notes[0]));
    }
  }

  /*
  if (digitalRead(BUTTON_PIN) != HIGH && !isClicked){
    isClicked = true;
    buttonClickCount = 1;
    unsigned long start_time = millis();
    if (digitalRead(BUTTON_PIN) == LOW){
      delay(100);
      start_time = millis();
    }
    bool wait = true;
    unsigned long w_time = millis();
    while (wait){
      if (digitalRead(BUTTON_PIN) == LOW){
        w_time = start_time;
        buttonClickCount+=1;
        delay(100);
      }
      if (w_time - start_time >= 50){
        wait = false;
      } 
      w_time = millis();
    }
    Serial.println("Clicked");
    Serial.print("button state1: ");
    Serial.println(digitalRead(BUTTON_PIN));
    Serial.print("button state2: ");
    Serial.println(digitalRead(BUTTON_PIN));
    Serial.print("button state3: ");
    Serial.println(digitalRead(BUTTON_PIN));
    while(digitalRead(BUTTON_PIN) == LOW){
      Serial.println("holding...");
      delay(1);
    }
    
    unsigned long time_delta = millis() - start_time;
    if (time_delta < short_press){
      Serial.println("Tapped");
      setPixelRGB(max_brightness, 255, 0, 0);
      delay(500);
      setPixelRGB(max_brightness, max_red, max_green, max_blue);
      if (buttonClickCount > 1){
        Serial.print("Click count: ");
        Serial.println(buttonClickCount);
        buttonClickCount = 0;
      }
      Serial.print("delta: ");
      Serial.println(start_time - last_tap);
      last_tap = start_time;
    }
    else if (time_delta > short_press &&  time_delta < long_press){
      Serial.println("Short Press");
      setPixelRGB(max_brightness, 0, 255, 0);
      delay(500);
      setPixelRGB(max_brightness, max_red, max_green, max_blue);
    }
    else if (time_delta > long_press){
      Serial.println("Long Press");
      setPixelRGB(max_brightness, 0, 0, 255);
      delay(500);
      setPixelRGB(max_brightness, max_red, max_green, max_blue);
    }
    isClicked = false;
  }*/
  /*
  for(int i = 0; i < 4; i++){
    pixelDemo_Sequence(200);
    eyeAnimationDemo();
  }
  pixelDemo_ColorRange();*/
  
  //demoCscale();
  delay(1);
  server.handleClient();
}



struct new_coord {
  int x;
  int y;
};

struct new_coord getNewCirclePosition(int8_t center_x,int8_t center_y, double angle, int8_t radius){
  struct new_coord out_coord;
  /*Serial.print("center_x: ");
  Serial.print(center_x);
  Serial.print(", center_y: ");
  Serial.println(center_y);
  Serial.print("cos*radius: ");
  Serial.println(cos(angle*PI/180)*radius);*/
  out_coord.x = int(center_x + radius*cos(angle*PI/180));
  out_coord.y = int(center_y - radius*sin(angle*PI/180));
  return out_coord;
}

void set_default_displays(){
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display_1.display();
  display_2.display();
  delay(2000); // Pause for 2 seconds
  display_1.clearDisplay();
  display_2.clearDisplay();

  // Clear the buffer
  display_1.setTextSize(2); // Draw 2X-scale text
  display_1.setTextColor(SSD1306_WHITE);
  display_1.setCursor(0, 0);
  display_1.println(F("Bus Alarm!"));
  display_1.fillCircle(display_2.width()/2,40,20,WHITE);
  display_1.fillCircle(display_2.width()/2,45,3,BLACK);
  display_1.display();      // Show initial text

  // Clear the buffer
  display_2.setTextSize(2); // Draw 2X-scale text
  display_2.setTextColor(SSD1306_WHITE);
  display_2.setCursor(0, 0);
  display_2.println(F("The School"));
  display_2.drawCircle(display_2.width()/2,40,20,WHITE);
  display_2.fillCircle(display_2.width()/2,45,3,WHITE);
  display_2.display();      // Show initial text
  delay(100);
}

void eyeAnimationDemo(){
  byte delay_t = 50;
  int8_t x = int8_t(display_2.width()/2);
  int8_t y = 45;
  for (double a = -90; a < 0; a+=5 ){
    struct new_coord target_coord = getNewCirclePosition(x,42, a, 16);
    /*Serial.print("new x: ");
    Serial.print(target_coord.x);
    Serial.print(", new y: ");
    Serial.println(target_coord.y);*/
    display_1.clearDisplay();
    display_2.clearDisplay();
    // Final State
    display_1.setTextSize(2); // Draw 2X-scale text
    display_1.setTextColor(SSD1306_WHITE);
    display_1.setCursor(0, 0);
    display_1.println(F("Bus Alarm!"));
    display_1.fillCircle(display_2.width()/2,40,20,WHITE);
    display_1.fillCircle(target_coord.x,target_coord.y,3,BLACK);
  
    // Clear the buffer
    display_2.setTextSize(2); // Draw 2X-scale text
    display_2.setTextColor(SSD1306_WHITE);
    display_2.setCursor(0, 0);
    display_2.println(F("The School"));
    display_2.drawCircle(display_2.width()/2,40,20,WHITE);
    display_2.fillCircle(target_coord.x,target_coord.y,3,WHITE);
    display_1.display();      // Show initial text
    display_2.display();      // Show initial text
    delay(delay_t);
  }
  for (double a = 0; a > -90; a-=5 ){
    struct new_coord target_coord = getNewCirclePosition(x,42, a, 16);
    /*Serial.print("new x: ");
    Serial.print(target_coord.x);
    Serial.print(", new y: ");
    Serial.println(target_coord.y);*/
    display_1.clearDisplay();
    display_2.clearDisplay();
    // Final State
    display_1.setTextSize(2); // Draw 2X-scale text
    display_1.setTextColor(SSD1306_WHITE);
    display_1.setCursor(0, 0);
    display_1.println(F("Bus Alarm!"));
    display_1.fillCircle(display_2.width()/2,40,20,WHITE);
    display_1.fillCircle(target_coord.x,target_coord.y,3,BLACK);
  
    // Clear the buffer
    display_2.setTextSize(2); // Draw 2X-scale text
    display_2.setTextColor(SSD1306_WHITE);
    display_2.setCursor(0, 0);
    display_2.println(F("The School"));
    display_2.drawCircle(display_2.width()/2,40,20,WHITE);
    display_2.fillCircle(target_coord.x,target_coord.y,3,WHITE);
    display_1.display();      // Show initial text
    display_2.display();      // Show initial text
    delay(delay_t);
  }

  
  display_1.clearDisplay();
  display_2.clearDisplay();
  // Final State
  display_1.setTextSize(2); // Draw 2X-scale text
  display_1.setTextColor(SSD1306_WHITE);
  display_1.setCursor(0, 0);
  display_1.println(F("Bus Alarm!"));
  display_1.fillCircle(display_2.width()/2,40,20,WHITE);
  display_1.fillCircle(display_2.width()/2,45,3,BLACK);
  display_1.display();      // Show initial text

  // Clear the buffer
  display_2.setTextSize(2); // Draw 2X-scale text
  display_2.setTextColor(SSD1306_WHITE);
  display_2.setCursor(0, 0);
  display_2.println(F("The School"));
  display_2.drawCircle(display_2.width()/2,40,20,WHITE);
  display_2.fillCircle(display_2.width()/2,45,3,WHITE);
  display_2.display();      // Show initial text
  delay(100);
}

void setPixelRGB(uint16_t brightness, uint16_t r, uint16_t g, uint16_t b){
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color((brightness*r/255) , (brightness*g/255), (brightness*b/255)));
  }
  pixels.show();
  // String resp = "RGB Color: red=" + String(r) +", green=" + String(g) + ", blue=" +String(b);
  // server.send(200,"text/plain",resp);
}

void pixelDemo_ColorRange(){
  const byte delay_t = 5;
  for(int x = 255; x > -1; x-=1){
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(max_brightness*x/255, max_brightness*x/255, max_brightness*x/255));
      
    }
    pixels.show();
    delay(delay_t);
  }
  byte n = 255;
  // Red
  for(byte x = 0; x < 255; x++){
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(max_brightness*x/255, 0, 0));
    }
    pixels.show();
    delay(delay_t);
  } 
  // Green
  for(byte x = 0; x < 255; x++){
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(max_brightness, max_brightness*x/255, 0));
    }
    pixels.show();
    delay(delay_t);
  } 
  // Remove red
  for(byte x = 255; x > 0; x--){
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(max_brightness*x/255, max_brightness, 0));
    }
    pixels.show();
    delay(delay_t);
  } 
  // Blue
  for(byte x = 0; x < 255; x++){
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, max_brightness, max_brightness*x/255));
    }
    pixels.show();
    delay(delay_t);
  }  
  // Remove blue
  for(byte x = 255; x > 0; x--){
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, max_brightness*x/255, max_brightness));
    }
    pixels.show();
    delay(delay_t);
  } 

  // Adding Red
  for(byte x = 0; x < 255; x++){
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(max_brightness*x/255, 0, max_brightness));
    }
    pixels.show();
    delay(delay_t);
  } 
}

void pixelDemo_Sequence(int val){
  //pixels.clear(); // Set all pixel colors to 'off'

  int primaryColor = random(0,3);
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    if(val > 0){
      switch (primaryColor){
        case 0:
          pixels.setPixelColor(i, pixels.Color((max_brightness*val/255), 0, 0));
          break;
        case 1:
          pixels.setPixelColor(i, pixels.Color(0, (max_brightness*val/255), 0));
          break;
        case 2:
          pixels.setPixelColor(i, pixels.Color(0, 0, (max_brightness*val/255)));
          break;
      }
    }
    else{
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
    //Serial.print("Pixel number: ");
    //Serial.println(i);
  }
}

void play_alarm(){
  // placeholder
  alarm_state_active = true;
  String alarm_name = server.arg("name");
  Serial.print("Currently running alarm: ");
  Serial.println(alarm_name);

  // Display the alarm name on the right OLED screen
  display_1.clearDisplay();
  display_1.setTextSize(2); // Draw 2X-scale text
  display_1.setTextColor(SSD1306_WHITE);
  display_1.setCursor(0, 0);
  display_1.print(alarm_name);
  display_1.println(F("!"));
  display_1.display();
  
  server_response =  "{\"status\": \"playing alarm\", \"alarm_name\":\"";
  server_response += alarm_name;
  server_response += "\"}";
  server.send(200, "text/plain", server_response);
  
  while (alarm_state_active){
    music_generator.PlayMelody1(music_generator.DrankinPatna_Notes, sizeof(music_generator.DrankinPatna_Notes)/sizeof(music_generator.DrankinPatna_Notes[0]));
    delay(3000);
  }
  set_default_displays();
  eyeAnimationDemo();
}

void parse_irgb(String _arg){
  uint16_t val_holder_l;
  uint16_t val_holder_r;
  current_irgb.payload = 0x0;
  for (uint8_t n = 0; n < 8 ; n+=2){
    val_holder_l = 0;
    val_holder_r = 0;

    if (_arg[n] >= 'A'){
      val_holder_l = (uint16_t)_arg[n] - 55;
    } else {
      val_holder_l = (uint16_t)_arg[n] - 48;
    }
    

    if (_arg[n + 1] >= 'A'){
      val_holder_r = ((uint16_t)_arg[n + 1] - 55);
    } else {
      val_holder_r = (uint16_t)_arg[n + 1] - 48;
    }

    switch (n){
      case 0:
        current_irgb.vals.i = (val_holder_l << 4) | val_holder_r;
        break;
      case 2:
        current_irgb.vals.r = (val_holder_l << 4) | val_holder_r;
        break;
      case 4:
        current_irgb.vals.g = (val_holder_l << 4) | val_holder_r;
        break;
      case 6:
        current_irgb.vals.b = (val_holder_l << 4) | val_holder_r;
        break;
    }
    
  }

  Serial.print("{ brightness: ");
  Serial.print(current_irgb.vals.i, HEX);
  Serial.print(", r: ");
  Serial.print(current_irgb.vals.r, HEX);
  Serial.print(", g: ");
  Serial.print(current_irgb.vals.g, HEX);
  Serial.print(", b: ");
  Serial.print(current_irgb.vals.b, HEX);
  Serial.println(" }");
  
}

void get_led_state(){
  server_response = "{\"state\":\"";
  server_response += LED_STATE;
  server_response += "\", \"irgb\":\"";
  // server_response += String((int)(current_irgb.payload | 0xffffffff), HEX);
  server_response += String((current_irgb.vals.i), HEX);
  server_response += String((current_irgb.vals.r), HEX);
  server_response += String((current_irgb.vals.g), HEX);
  server_response += String((current_irgb.vals.b), HEX);
  server_response += "\"}";
  server.send(200, "text/plain", server_response);
}

void set_led_state(){
  Serial.print("state: ");
  Serial.println(server.arg("state").toInt());
  if (server.arg("state").toInt() > 0){
    LED_STATE = true;
  } else {
    LED_STATE = false;
  }

  parse_irgb(server.arg("irgb"));
  Serial.print("incoming irgb: ");
  Serial.println(server.arg("irgb"));
  if (LED_STATE){
    setPixelRGB(current_irgb.vals.i,current_irgb.vals.r,current_irgb.vals.g,current_irgb.vals.b);
  } else {
    setPixelRGB(0,current_irgb.vals.r,current_irgb.vals.g,current_irgb.vals.b);
  }
  
  server_response = "{\"status\": \"led's set\"}";
  server.send(200, "text/plain", server_response);
}
