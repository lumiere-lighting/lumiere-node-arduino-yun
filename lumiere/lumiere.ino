#include <Bridge.h>
#include <HttpClient.h>
#include <FastLED.h>
#include <ArduinoJson.h>

// Config
#include "config-default.h"
#include "config.h"

// Top level objects
HttpClient client;
String current_id;
CRGB leds[NUM_LEDS];
double animation_interval = ANIMATION_TIME / NUM_LEDS;
const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(LIGHT_LIMIT) + 10;


void setup() {
  // Serial.  Uncomment to get Serial displaying, otherwise the firmware
  // will not actually run unless you open the Serial console
  //Serial.begin(9600);
  //while(!Serial);

  // Setup leds.  Not sure of best way to have a single config
  // to set this without a big if--then statement
  if (LED_TYPE == LPD8806) {
    FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(10)>(leds, NUM_LEDS);
  }
  else {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  }

  // Set brightness
  FastLED.setBrightness(BRIGHTNESS);

  // Reset lights
  FastLED.clear();
  for (int j = NUM_LEDS - 1; j >= 0; j--) {
    leds[j] = CRGB::Black;
  }
  FastLED.show();

  // Connect to bridge
  Bridge.begin();
}


void loop() {

  // Make call to API
  client.get(String(LUMIERE_SERVER) + "/api/colors?format=hex0&noInput=true&limit=" + String(LIGHT_LIMIT));

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
    //Serial.println(JSON_BUFFER_SIZE);
    StaticJsonBuffer<JSON_BUFFER_SIZE> buffer;
    JsonObject& parsed = buffer.parseObject(response_char);
    if (parsed.success()) {
      //Serial.println("success");

      // Get ID
      const char * char_id = parsed["_id"];
      String id = String(char_id);

      // If different ID, change lights
      if (id != current_id) {
        current_id = id;
        //Serial.println(id);

        // The sizeof does not work for json array so we work around it
        // a bit
        int color_set_count = 0;
        for (int i = 0; i < NUM_LEDS; i++) {
          if (parsed["colors"][i].success()) {
            color_set_count++;
          }
        }
        //Serial.println(color_set_count);

        // Reset lights
        for (int j = NUM_LEDS - 1; j >= 0; j--) {
          leds[j] = CRGB::Black;
          FastLED.show();
          delay(animation_interval / 2);
        }

        // Change lights
        for (int j = 0; j < NUM_LEDS; j++) {
          const char * c = parsed["colors"][j % color_set_count];
          leds[j] = strtol(c, NULL, 0);
          FastLED.show();
          delay(animation_interval / 2);
        }
      }
    }
  }

  // Flush serial
  //Serial.flush();

  // Delay for polling
  delay(POLL_TIME);
}

// Do animation of lights
