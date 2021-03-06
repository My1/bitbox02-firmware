// Copyright 2019 Shift Cryptosecurity AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "common_main.h"
#include "driver_init.h"
#include "qtouch.h"
#include "random.h"
#include "screen.h"
#include "ui/oled/oled.h"
#include "ui/ugui/ugui.h"
#include "usb_desc.h"
#include "usbdc.h"
#include "utils.h"
#include <string.h>
#include <ui/fonts/arial_fonts.h>
#include <usb/usb.h>

uint32_t __stack_chk_guard = 0;

// USB_REPORT_SIZE = 0x40
static uint8_t usb_ctrl_endpoint_buffer[USB_REPORT_SIZE];

int main(void)
{
    system_init();
    __stack_chk_guard = common_stack_chk_guard();

    UG_GUI guioled; // Global GUI structure (OLED)
    UG_Init(
        &guioled,
        (void (*)(UG_S16, UG_S16, UG_COLOR))oled_set_pixel,
        &font_font_a_11X10,
        SCREEN_WIDTH,
        SCREEN_HEIGHT);
    screen_print_debug("before test", 1000);

    int32_t init_error = usbdc_init(usb_ctrl_endpoint_buffer);

    if (init_error != ERR_NONE) {
        screen_print_debug("error", 1000);
        while (1) {
        }
    }
    screen_print_debug("after test", 1000);

    while (1) {
    }
}
