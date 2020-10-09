// Copyright 2020 Shift Crypto AG
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

#include "screen_saver.h"

#include <hardfault.h>
#include <ui/components/screensaver.h>

#include <stdbool.h>
#include <stdint.h>

// TODO: use a TIMER interrupt to get a more accurate timer.
// 27000 is ~1min.
#define ACTIVE_AFTER 270000

static uint32_t _counter = 0;
static bool _is_active = false;

component_t* screen_saver_get(void)
{
    if (!_is_active) {
        return NULL;
    }
    static component_t* screensaver = NULL;
    if (screensaver == NULL) {
        screensaver = screensaver_create();
        if (screensaver == NULL) {
            Abort("Could not create\nscreensaver");
        }
    }
    return screensaver;
}

void screen_saver_process(void)
{
    if (!_is_active) {
        _counter++;
        if (_counter > ACTIVE_AFTER) {
            _is_active = true;
        }
    }
}

void screen_saver_reset(void)
{
    component_t* component = screen_saver_get();
    if (component != NULL) {
        screensaver_reset(component);
    }
    _is_active = false;
    _counter = 0;
}
