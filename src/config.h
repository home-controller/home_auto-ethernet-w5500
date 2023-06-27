const char* baseTopic = "h1";

const char* lights[] = {
  "living_room",
  "bedroom",
  // Add more lights as needed
};

const char configMsg[] PROGMEM = R"({
  "lights": [
    {
      "name": "Living Room Light",
      "command_topic": "h1/living_room/light/s/e",
      "state_topic": "state/h1/living_room/light/s/e",
      "payload_on": "1",
      "payload_off": "0"
    },
    {
      "name": "Bedroom Light",
      "command_topic": "h1/bedroom/light/s/e",
      "state_topic": "state/h1/bedroom/light/s/e",
      "payload_on": "1",
      "payload_off": "0"
    }
  ]
})";

// snprintf(lightConfig, sizeof(lightConfig),
//            R"({
//              "name": "%s Light",
//              "command_topic": "%s/%s/light/s/e",
//              "state_topic": "state/%s/%s/light/s/e",
//              "payload_on": "1",
//              "payload_off": "0"
//            })",
//            lights[i], baseTopic, lights[i], baseTopic, lights[i]);
