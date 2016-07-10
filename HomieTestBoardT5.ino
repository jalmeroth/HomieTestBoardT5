#include <Homie.h>

#define FW_NAME "test-board"
#define FW_VERSION "1.1.1"

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

HomieNode temperatureNode("temperature", "temperature");

bool lightHandler(String value) {
  Serial.print("LIGHT:");
  Serial.println(value);
  return true;
}

bool lightRHandler(String value) {
  Serial.print("LIGHTR:");
  Serial.println(value);
  return true;
}

bool lightGHandler(String value) {
  Serial.print("LIGHTG:");
  Serial.println(value);
  return true;
}

bool lightBHandler(String value) {
  Serial.print("LIGHTB:");
  Serial.println(value);
  return true;
}

String content = "";

void setupHandler() {
  Homie.setNodeProperty(temperatureNode, "unit", "c", true);
}

void loopHandler() {
  if (Homie.isReadyToOperate()) {
    while(Serial.available()) {
        content = Serial.readString();
        content.trim();
    }
  
    if (content != "") {
      if(content == "AT+CWMODE=1") {
        Serial.println("WIFI CONNECTED");
        Serial.println("WIFI GOT IP");
      } else if(content.startsWith("TEMP:")) {
        Serial.print("Teh heat: ");
        String temperature = content.substring(5, content.indexOf(";"));
        Serial.println(temperature);
        if (!Homie.setNodeProperty(temperatureNode, "degrees", temperature, true)) {
          Serial.println("Temperature sending failed");
        }
      } else {
        Serial.println(content);
      }
      content = "";
    }
  }
}

void setup() {
  Homie.enableBuiltInLedIndicator(false);
  Homie.setFirmware(FW_NAME, FW_VERSION);
  Homie.registerNode(temperatureNode);
  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);
  temperatureNode.subscribe("LIGHT", lightHandler);
  temperatureNode.subscribe("LIGHTR", lightRHandler);
  temperatureNode.subscribe("LIGHTG", lightGHandler);
  temperatureNode.subscribe("LIGHTB", lightBHandler);
  Homie.setup();
}

void loop() {
  Homie.loop();
}
