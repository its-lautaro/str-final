#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // make the LED pin an output for PORTB5
    DDRB = 1 << 7;

    while (1)
    {
        _delay_ms(500);
        // toggle the LED
        PORTB ^= 1 << PB7;
    }

    return 0;
}