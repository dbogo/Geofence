#ifndef MAVLINK_CONTROL_H_
#define MAVLINK_CONTROL_H_

// #include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "interface.h"
#include "commands.h"

int main(int argc, char** argv);

// Scheduler
void commands(void);

void send_pre_arm_void_commands(void);
int autopilot_ok(void);

#endif