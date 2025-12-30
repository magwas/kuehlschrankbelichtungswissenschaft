#include "config.h"
#include "MessageQueue.h"
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

class Thermometer {
    protected:
        static void read(Message * msg) {
            sensors.requestTemperatures();
            int index = msg->payload[0];
            float tempC = sensors.getTempCByIndex(index);
            systemQueue.send(MessageType::Temperature,tempC,index);
        }
    public:
        void begin() {
            sensors.begin();
            systemQueue.registerListener(MessageType::TemperatureRequest,&read);
        }
};

extern Thermometer thermometer;