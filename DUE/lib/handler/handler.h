#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>  
#include <sys/time.h>

/**
 * @brief Initialize handler
 */
void initialize();

/**
 * @brief sendBlockforUart1
 */
void sendBlock();

/**
 * @brief receiveResultforUart1
 */
void receiveResult();

/**
 * @brief Loop for handler
 */
void loopHandler();