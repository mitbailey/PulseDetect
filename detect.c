#include <signal.h>
#include <unistd.h>
#include "gpiodev.h"
#include "meb_print.h"

volatile sig_atomic_t done = 0;
void sig_handler(int sig)
{
    done = 1;
}

static int counter = 0;
void gpio_callback(void *vp)
{
    // Call back triggered.
    dbprintlf("Callback triggered!");
}

int main(int argc, char *argv[])
{
    int detect_pin = -1;
    if (argc == 1)
    {
        if (gpiodev_pinout == PINOUT_RPI)
        {
            detect_pin = 13
        }
        else
        {
            // Doesn't work if not a RaspPi.
            return 0;
        }
    }
    
    signal(SIGINT, &sig_handler);
    gpioRegisterIRQ(detect_pin, GPIO_IRQ_RISE, &gpio_callback, &counter 1000);
    
    while(!done)
    {
        sleep(1);
    }

    gpioUnregisterIRQ(detect_pin);
    return 0;
}