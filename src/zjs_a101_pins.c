// Copyright (c) 2016-2017, Intel Corporation.

// ZJS includes
#include "zjs_common.h"
#include "zjs_gpio.h"
#include "zjs_pwm.h"
#include "zjs_util.h"

#ifdef BUILD_MODULE_PWM
static void zjs_a101_num_to_pwm(u32_t num, int *dev, int *pin)
{
    *dev = 0;
    if (num < 0) {
        *pin = -1;
        return;
    }
    if (num < 4) {
        // support directly giving channel number 0-3
        *pin = num;
        return;
    }
    if (num >= 0x20 && num <= 0x23) {
        // support PWM0 through PWM3
        *pin = num - 0x20;
        return;
    }

    switch (num) {
    case 17:  // IO3
        *pin = 0;
        break;
    case 15:  // IO5
        *pin = 1;
        break;
    case 12:  // IO6
        *pin = 2;
        break;
    case 13:  // IO9
        *pin = 3;
        break;

    default:
        *pin = -1;
    }
}
#endif

jerry_value_t zjs_a101_init()
{
    // effects: returns an object with Arduino 101 pin mappings
#ifdef BUILD_MODULE_PWM
    zjs_pwm_convert_pin = zjs_a101_num_to_pwm;
#endif

    jerry_value_t obj = zjs_create_object();

    // These are all the GPIOs that can be accessed as GPIOs by the X86 side.
    zjs_obj_add_number(obj, 18, "IO2");
    zjs_obj_add_number(obj, 17, "IO3");  // doesn't seem to work as output
    zjs_obj_add_number(obj, 19, "IO4");
    zjs_obj_add_number(obj, 15, "IO5");  // doesn't seem to work as output
    zjs_obj_add_number(obj, 20, "IO7");
    zjs_obj_add_number(obj, 16, "IO8");
    zjs_obj_add_number(obj, 11, "IO10");
    zjs_obj_add_number(obj, 10, "IO11");
    zjs_obj_add_number(obj, 9,  "IO12");
    zjs_obj_add_number(obj, 8,  "IO13");  // output also displayed on LED0

    // These are onboard LEDs
    zjs_obj_add_number(obj, 26, "LED0");  // active low, red fault LED
    zjs_obj_add_number(obj, 12, "LED1");  // active low
    // LED2 is unavailable when SPI is in use because that reconfigures pin
    //   IO13 which this is tied to. The filesystem uses SPI to talk to the
    //   flash chip, so when you use filesystem APIs or ashell (which always
    //   uses the filesystem) this LED will not work.
    zjs_obj_add_number(obj, 8,  "LED2");

    // These cannot currently be used as GPIOs because they are controlled by
    // the ARC side and we don't have support for that. But they can be used as
    // PWMs.
    zjs_obj_add_number(obj, 12, "IO6");
    zjs_obj_add_number(obj, 13, "IO9");

    zjs_obj_add_number(obj, 0x20,  "PWM0");
    zjs_obj_add_number(obj, 0x21,  "PWM1");
    zjs_obj_add_number(obj, 0x22,  "PWM2");
    zjs_obj_add_number(obj, 0x23,  "PWM3");

    // TODO: It appears that some other GPIO pins can be used as analog inputs
    //   too, from the X86 side. We haven't tried that.
    zjs_obj_add_number(obj, 10, "A0");
    zjs_obj_add_number(obj, 11, "A1");
    zjs_obj_add_number(obj, 12, "A2");
    zjs_obj_add_number(obj, 13, "A3");
    zjs_obj_add_number(obj, 14, "A4");
    zjs_obj_add_number(obj, 9,  "A5");

    return obj;
}
