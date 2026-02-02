#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "sdkconfig.h"
#include "MessageQueue.h"
#include "SerialConsole.h"
#include "Command.h"
#include "Square.h"
#include "Astable.h"
#include "MsgConfig.h"
#include "MsgWifi.h"
#include "TelnetConsole.h"
#include "MsgOneWireTemperature.h"
#include <esp_log.h>
#include "MsgMux.h"
#include "MsgCO2.h"

Command cmd;
MsgCO2 co2;
void debug(Message *msg) {
    char buffer[CONFIG_MSG_PAYLOAD_LENGTH];
    switch(msg->type) {
        case MessageType::Stdin:
        case MessageType::Stdout:
        case MessageType::Command:
        case MessageType::OneHz:
            break;
        case MessageType::Debug:
            ESP_LOG_BUFFER_HEX("Debug", msg, 16);
            [[fallthrough]];
        default:
            cmd.messageToString(msg,buffer);
            systemQueue.send(MessageType::Stdout,"%s",buffer);
    }
}

extern "C" void app_main(void)
{
    systemQueue.begin();
    serialConsole.begin();
	vTaskDelay(pdMS_TO_TICKS(1000));
    cmd.begin();
    tester.begin();
    Astable::begin();
    MsgConfig::begin();
//    MsgWifi::begin();
//    TelnetConsole::begin();
    MsgOneWireTemperature::begin();
    MsgMux::begin();
    co2.begin();
	systemQueue.send(MessageType::Command,"config apply");
	systemQueue.registerListener(MessageType::All,debug);
	while(1) {
	    systemQueue.send(MessageType::OneHz,'1');
	    vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

