# Home Auto Arduino ethernet relay/lights controller

The main point of this is that it works with wired twisted pair, CAT5(etc.) RJ45 cable networks using the W5500 or W5100 and not wifi. Other modules than the W5x00 should work with just a different library

Relay controller for Arduino. Works with the basic ATMEG328p chips, nano, micro, UNO etc. using the W5500 & W5100 modules.

Has web server on port 80 and MQTT client to turn on/off a relay. To control the outside lights in my case.

Switches can be conected to pins to control the relays as well as MQTT and the Web server on port 80.

