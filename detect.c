#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "gpiodev.h"

static inline uint64_t get_time_now()
{
    static struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

volatile sig_atomic_t done = 0;
void sig_handler(int sig)
{
    done = 1;
}

uint64_t counter = 0;
uint64_t tnow = 0;

// static int counter = 0;
void gpio_callback(void *_fp)
{
    FILE *fp = (FILE *)_fp;
    tnow = get_time_now();
    fwrite(&tnow, 0x1, sizeof(uint64_t), fp);
    fwrite(&counter, 0x1, sizeof(uint64_t), fp);
    counter++;
}

int main(int argc, char *argv[])
{
    int detect_pin = -1;
    if (argc == 1)
    {
        if (gpiodev_pinout == PINOUT_RPI)
        {
            detect_pin = 13;
        }
        else
        {
            // Doesn't work if not a RaspPi.
            return 0;
        }
    }

    FILE* fp = fopen("stepper.info.txt", "wb");
    if (fp == NULL)
    {
        return -1;
    }

    signal(SIGINT, &sig_handler);
    gpioRegisterIRQ(detect_pin, GPIO_IRQ_RISE, &gpio_callback, fp, 1000);
    
    while(!done)
    {
        sleep(1);
    }

    gpioUnregisterIRQ(detect_pin);
    fclose(fp);
    return 0;
}