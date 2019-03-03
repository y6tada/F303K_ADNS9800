#include "mbed.h"
#include "adns_9800.h"
#include "buffer.h"

#define DEBUG_BAUD      115200
#define RS485_BAUD      460800
#define WAIT_TIME       20      /// Unit:[us]

#define BUF_X           0
#define BUF_Y           1

#define HEADER_H        0xFA
#define HEADER_L        0xAF

#define MOUSE_ADDRESS   0x02

Buffer data;
Serial RS485(PA_9, PA_10);
DigitalOut REDE(PA_1);

Serial port(USBTX, USBRX);
DigitalOut sLED(PA_0);
Ticker sendDataTicker;
adns_9800 mouse_sensor(PA_7, PA_6, PA_5, PA_4);

int16_t delta_x, delta_y;

void sendData(void) {
    
    sLED = 1;
    port.putc(127);
    port.putc((int8_t)data.get(BUF_X));
    port.putc((int8_t)data.get(BUF_Y));
    sLED = 0;
    
    data.reset(BUF_X);
    data.reset(BUF_Y);
}

// ===========================================================================//
int main() {
    
    port.baud(115200);
    
    sendDataTicker.attach_us(sendData,2500);

    while(true) {
        if(mouse_sensor.new_motion_data_available()) {
            mouse_sensor.get_delta_x_y(delta_x, delta_y);
            data.setWithIntegral(BUF_X, delta_x);
            data.setWithIntegral(BUF_Y, delta_y);
        }
    }
    
    return 0;
}
// ===========================================================================//