#include <platform.h>
#include <gpio.h>
#include "switches.h"

void switches_init(void) {
}

int switch_get(Pin pin) {
	return !gpio_get(pin);
}

// *******************************ARM University Program Copyright � ARM Ltd 2016*************************************   
