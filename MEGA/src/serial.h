#ifndef SERIAL_H_
#define SERIAL_H_

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

void SERIAL_Begin(long baud);
void SERIAL_Print(const uint32_t n);
void SERIAL_Print(const char* str);

#endif