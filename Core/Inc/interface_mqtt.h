#pragma once
#include "stdbool.h"
#include "stdint.h"

#define MAX_MQTT_PUB_BUFFER_BYTES 128
#define MQTT_PUBLISH_TIMEOUT_MS (10 * 1000)

//bool connect(void);
void mqtt__process(void);
bool mqtt__publish(char *message, uint16_t byte_length);