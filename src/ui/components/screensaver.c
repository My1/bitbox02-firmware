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

#include "screensaver.h"

#include "image.h"
#include "ui_images.h"

#include <hardfault.h>
#include <screen.h>
#include <ui/ui_util.h>
#include <ui/oled/oled.h>

#include <random.h>

#include <string.h>

static void _render(component_t* component)
{
    component_t* image = component->sub_components.sub_components[0];

    static uint16_t counter = 0;
    counter++;

    static int8_t x_direction = 1;
    static int8_t y_direction = 1;
    static int8_t x_slowdown = 1;
    static int8_t y_slowdown = 2;

    const uint16_t master_slowdown = 2; // for slowing down both x and y in the same amount

    if (counter % (master_slowdown * x_slowdown) == 0) {
        image->position.left += x_direction;
        if (((image->position.left + image->dimension.width) >= component->dimension.width &&
             x_direction > 0) ||
            (image->position.left < 0 && x_direction < 0)) {
            x_direction *= -1;
            oled_set_brightness(random_byte_mcu());
        }
    }
    if (counter % (master_slowdown * y_slowdown) == 0) {
        image->position.top += y_direction;
        if (((image->position.top + image->dimension.height) >= component->dimension.height &&
             y_direction > 0) ||
            (image->position.top < 0 && y_direction < 0)) {
            y_direction *= -1;
            oled_set_brightness(random_byte_mcu());
        }
    }
    ui_util_component_render_subcomponents(component);
}

/**
 * Collects all component functions.
 */
static component_functions_t _component_functions = {
    .cleanup = ui_util_component_cleanup,
    .render = _render,
    .on_event = ui_util_on_event_noop,
};

component_t* screensaver_create(void)
{
    component_t* component = malloc(sizeof(component_t));
    if (!component) {
        Abort("Error: malloc screensaver");
    }
    memset(component, 0, sizeof(component_t));
    component->f = &_component_functions;
    component->dimension.width = SCREEN_WIDTH;
    component->dimension.height = SCREEN_HEIGHT;
    component_t* bb2_logo = image_create(
        IMAGE_SCREENSAVER,
        sizeof(IMAGE_SCREENSAVER),
        IMAGE_SCREENSAVER_W,
        IMAGE_SCREENSAVER_H,
        CENTER,
        component);
    ui_util_add_sub_component(component, bb2_logo);
    screensaver_reset(component);
    return component;
}

void screensaver_reset(component_t* component)
{
    component_t* image = component->sub_components.sub_components[0];
    image->position.left = -image->dimension.width;
    // We move the image even more out of screen, to have a moment where the screen is just black
    // before the screensaver becomes visible.
    image->position.left -= 30;
}
