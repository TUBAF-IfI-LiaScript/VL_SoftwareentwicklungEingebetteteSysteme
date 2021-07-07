#include "mbed.h"

DigitalOut myled(LED1);

//// MBED OS 5 Version    // switch in platformio.ini !
//Serial pc(USBTX, USBRX, 9600); // tx, rx
//
//int main() {
//    wait(1);
//    pc.printf("\nHello World!\n");
//    while(1) {
//        myled = 1; // LED is ON
//        wait(1);
//        myled = 0; // LED is OFF
//        wait(1);
//    }
//}

// MBED OS 6 Version
#define MAXIMUM_BUFFER_SIZE 30
static BufferedSerial serial_port(USBTX, USBRX);

int main() {
    serial_port.set_baud(9600);
    serial_port.set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );
    char buf[MAXIMUM_BUFFER_SIZE] = "\nHello World!\n";
    serial_port.write(buf, sizeof(buf));
    while(1) {
        myled = 1; // LED is ON
        ThisThread::sleep_for(1s);
        myled = 0; // LED is OFF
        ThisThread::sleep_for(1s);
    }
}
