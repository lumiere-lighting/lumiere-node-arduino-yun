# Lumière with Ardunio Yun

[Lumiere](http://lumiere.lighting/).  Change holiday lights around the world.

Here you will find code and instructions on how to set up some Lumiere lights with a Raspberry Pi.  By default, it will talk with the main Lumiere application at [lumiere.lighting](http://lumiere.lighting), but you can also set up your own [Lumiere server](https://github.com/lumiere-lighting/lumiere-server).

## Ingredients

There are links to products at Adafruit as reference points; though Adafruit is awesome, there is no requirement to buy products from them.

### Controller

* An [Arduino YÚN](https://www.adafruit.com/product/1498).  There are many different kits and extensions at  [Adafruit](http://www.adafruit.com/category/105).

### Lights

* Any lights supported by [FastLED](https://github.com/FastLED/FastLED/wiki/Chipset-reference).  [NeoPixels](https://www.adafruit.com/category/168) are a good choice.
    * You will have to update configuration in the firmware script.
    * Depending on the amount of lights you have, you will have different power requirements; consult information regarding your strand of lights.

### Other stuff

For prototyping, you will probably need the following (note that you can probably get a kit with most of this stuff, too):

* [Breadboard](http://www.adafruit.com/products/64)
* [Jumper/prototyping wires](https://www.adafruit.com/product/153)
* Resistor, any will do; this is just to go in between the LED data connector to help protect it.
* If you need more power than what the Arduino Yun can provide, you will need something like the following:
    * [Capacitor](https://www.adafruit.com/products/1589) to be connected to the power supply in parallel so that a possible intial power surge will not harm your LEDs.
    * [5V 10A power supply](http://www.adafruit.com/products/658).  This power source is solid and will power many lights.
    * [Female DC power adaptor](http://www.adafruit.com/products/368) used to connect the power supply to the breadboard and then the lights and Arduino.  For those of us that don't know about these things that well, the `-` should be connected to ground and `+` to the 5V power.

## Setup

### Software

1. Install the [Arduino IDE](http://arduino.cc/en/main/software).  Make sure to install the one that supports the Yun, which at this time it is `1.5.8 beta`.
1. Open up the Arduino IDE.
1. Install libraries.  There is a "package manager" in the IDE which is suggested.
    1. Download [FastLED](https://github.com/FastLED/FastLED/releases).  The `zip` file.  This was last tested with the `v2-final` version.
      1. Load the library in the Arduino IDE by going to `Skitch > Import Library > Add Library` and choose the `zip` file you downloaded.
    1. Download [ArdunioJSON](https://github.com/bblanchon/ArduinoJson/archive/v3.4.zip) and load as library like above.
1. Open the `lumiere-node-arduino-yun.ino.ino` file in the Arduino IDE.
1. Update configuration values as needed.
1. Upload the sketch to your Ardunio Yun.

### Hardware

For prototyping with a short string of lights, do the following:

* Connect the `5V` pin on the Arduino to the power of the lights.
* Connect the `GND` pin on the Arduino to the ground on the lights.
* Connect the `digital 8` pin on the Arduino to the data in on the lights.
* (optional) If your lights have a clock connection, connect this to `digitial 9` pin on the Arduino.

For larger strings of lights, do the following:

(coming soon)
