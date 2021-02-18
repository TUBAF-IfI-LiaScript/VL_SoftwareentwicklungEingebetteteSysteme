#include "mbed.h"

DigitalOut myled(LED1);

int main() {
    while(1) {
        myled = 1; // LED is ON
        ThisThread::sleep_for(1s);
        myled = 0; // LED is OFF
        ThisThread::sleep_for(1s);
    }
}
