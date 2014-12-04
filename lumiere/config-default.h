// Default config for Arduino Yun Lumiere setup

// Type of chip used, currently supported NEOPIXEL and LPD8806
#define LED_TYPE NEOPIXEL
// Data PIN for the lights
#define DATA_PIN 3
// Clock pin, though not all lights have one
#define CLOCK_PIN 13
// Limit seems to be around 400 because of Yun's memory
#define NUM_LEDS 100
// Some lights don't come in RGB, but in differnt order
// use RGB, GRB, etc
#define COLOR_ORDER RGB
// Global brightness of the lights, the more bright, the more
// use of power, and the more the washed out the colors may look
#define BRIGHTNESS 100
// How long should the animation run
#define ANIMATION_TIME 1500
// Poll time (time between HTTP requests)
#define POLL_TIME 3000
// Color input limt for API.  The smaller the less memory that is needed,
// both with the HTTP request and parsing the JSON
//#define INPUT_LIMIT PIXEL_COUNT
#define LIGHT_LIMIT 10
// Server
#define LUMIERE_SERVER "http://lumiere.lighting"
