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
            uint8_t* address = (uint8_t *) msg->payload[0];
            float tempC = sensors.getTempC(address);
            systemQueue.send(MessageType::Temperature,tempC,);
        }
    public:
        void begin() {
            pinMode(ONE_WIRE_BUS,OUTPUT);
            sensors.begin();
            uint8_t addr[8];
            systemQueue.printf("c:%u",sensors.getDeviceCount());
            sensors.getAddress(addr,0);
            systemQueue.printf("0:%u,%u",((int*)&addr)[0],((int*)&addr)[1]);
            sensors.getAddress(addr,1);
            systemQueue.printf("1:%u,%u",((int*)&addr)[0],((int*)&addr)[1]);
            systemQueue.printf("onewire:%u",ONE_WIRE_BUS);
            systemQueue.registerListener(MessageType::TemperatureRequest,&read);
        }
};

extern Thermometer thermometer;