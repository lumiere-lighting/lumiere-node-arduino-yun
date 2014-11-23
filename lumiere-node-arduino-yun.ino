#include <Bridge.h>
#include <HttpClient.h>

#include <FastLED.h>

#include <JsonParser.h>
using namespace ArduinoJson::Parser;


/**
 * Config level objects
 */

// Data PIN for the lights
#define DATA_PIN 8
// Limit seems to be over 800
#define NUM_LEDS 115
// Some lights don't come in RGB, but in differnt order
#define COLOR_ORDER RGB
// Global brightness of the lights
#define BRIGHTNESS 50
// How long should the animation run
#define ANIMATION_TIME 1500
// Poll time (time between HTTP requests)
#define POLL_TIME 5000
// Color input limt for API
//#define INPUT_LIMIT PIXEL_COUNT
#define LIGHT_LIMIT 15


// Top level objects
JsonParser<40> parser;
HttpClient client;
String current_id;
String api = "http://lumiere.lighting/api/colors?format=hex0&noInput=true&limit=" + String(LIGHT_LIMIT);
CRGB leds[NUM_LEDS];
double animation_interval = ANIMATION_TIME / NUM_LEDS;


void setup() {
  // Setup leds
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
          if (parsed["colors"][i].success()) {
            color_set_count++;
          }
        }
        
        // Reset lights
        for (int j = NUM_LEDS - 1; j >= 0; j--) {
          leds[j] = CRGB::Black;
          FastLED.show();
          delay(animation_interval);
        }
        
        // Change lights
        for (int j = 0; j < NUM_LEDS; j++) {
          char * c = parsed["colors"][j % color_set_count];
          String C = String(c);
          leds[j] = strtol(c, NULL, 0);
          
          FastLED.show();
          delay(animation_interval);
        }
      }
    }
  }
  
  // Flush serial and delay for polling
  Serial.flush();
  delay(POLL_TIME);
}
