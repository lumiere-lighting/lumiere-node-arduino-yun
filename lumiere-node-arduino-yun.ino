#include <Bridge.h>
#include <HttpClient.h>

#include <FastLED.h>

#include <JsonParser.h>
using namespace ArduinoJson::Parser;


// Config level objects
#define NUM_LEDS 8
#define DATA_PIN 8
#define ANIMATION_TIME 1000
#define BRIGHTNESS 25
// Some lights need a clock pin
//#define CLOCK_PIN 8
// Some lights don't come in RGB, but in differnt order
#define COLOR_ORDER GRB


// Top level objects
JsonParser<64> parser;
HttpClient client;
String current_id;
String api = "http://lumiere.lighting/api/colors?format=hex0&noInput=true&limit=" + String(NUM_LEDS);
CRGB leds[NUM_LEDS];
double animation_interval = ANIMATION_TIME / NUM_LEDS;


void setup() {
  // Determine RGB
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // Set brightness
  LEDS.setBrightness(BRIGHTNESS);
  
  // Connect to bridge
  Bridge.begin();
  
  // Serial.  Uncomment to get Serial displaying, otherwise the firmware
  // will not actually run unless you open the Serial console
  //Serial.begin(9600);
  //while(!Serial);
}


void loop() {
  // Make call to API
  client.get(api);
  
  // Read in stream
  String response = "";
  while (client.available()) {
    char c = client.read();
    response += c;
  }
  
  // Convert to char array
  int str_len = response.length() + 1; 
  char response_char[str_len];
  response.toCharArray(response_char, str_len);
  
  // Check to see, hackishly, if JSON (maybe check for {)
  if (1) {
    // Parse JSON
    JsonObject parsed = parser.parse(response_char);
    if (parsed.success()) {
      // Get ID
      char * char_id = parsed["_id"];
      String id = String(char_id);
      
      // If different ID, change lights
      if (id != current_id) {
        current_id = id;
        
        // The sizeof does not work for json array so we work around it
        // a bit
        int color_set_count = 0;
        for (int i = 0; i < NUM_LEDS; i++) {
          char * s = parsed["colors"][i];
          String S = String(s);
          if (S.length() > 2) {
            color_set_count++;
          } 
        }
        
        // Reset lights
        for (int j = NUM_LEDS - 1; j >= 0; j--) {
          leds[j] = CRGB::Black;
          delay(animation_interval);
          FastLED.show();
        }
        
        // Change lights
        for (int j = 0; j < NUM_LEDS; j++) {
          char * c = parsed["colors"][j % color_set_count];
          String C = String(c);
          leds[j] = strtol(c, NULL, 0);
          
          delay(animation_interval);
          FastLED.show();
        }
      }
    }
  }
  
  // Flush serial and delay for polling
  Serial.flush();
  delay(5000);
}
