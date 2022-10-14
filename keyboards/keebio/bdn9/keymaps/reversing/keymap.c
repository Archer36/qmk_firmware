/* Copyright 2019 Danny Nguyen <danny@keeb.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        |                              |               | Middle Knob: Hue Up/Dn   |
        | F7: Step Into                | F8: Step Over | Press: RGB mode cycle    |
        | Ctrl+F9: Execute till return | Up            | Alt+F9: Run to user code |
        | Held: Layer 1                | RGB Tog       | Right                    |
     */
    [0] = LAYOUT(
        KC_F7   , KC_F8  , RGB_MOD,
        C(KC_F9), KC_UP  , A(KC_F9),
        MO(1)   , RGB_TOG, KC_RGHT
    ),
    /*
        |                | Middle Knob: Value Up/Dn |            |
        | QK_BOOT        | N/A                      | Media Stop |
        | Held: Layer 2  | Home                     | RGB Mode   |
        | Media Previous | End                      | Media Next |
     */
    [1] = LAYOUT(
        QK_BOOT  , BL_STEP, KC_STOP,
        _______, KC_HOME, RGB_MOD,
        KC_MPRV, KC_END , KC_MNXT
    ),
};

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    HSV hsvCyan = {127, 255, 63};
    HSV hsvGreen = {85, 255, 63};
    HSV hsvRed = {0, 255, 63};

    RGB rgb;

    if (layer_state_is(0)) {
        // Set underglow to cyan for layer 0
        rgb = hsv_to_rgb(hsvCyan);
    } else if (layer_state_is(1)) {
        // Set underglow to green for layer 1
        rgb = hsv_to_rgb(hsvGreen);
    } else {
        // Set underglow to red for default/unknown
        rgb = hsv_to_rgb(hsvRed); 
    }

    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02)) { // 0x02 == underglow
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
    
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    switch(get_highest_layer(layer_state|default_layer_state)) {
        case 0:
            if (index == _LEFT) {
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
            }
            else if (index == _MIDDLE) {
                if (clockwise) {
                    rgb_matrix_increase_val();
                } else {
                    rgb_matrix_decrease_val();
                }
            }
            else if (index == _RIGHT) {
                if (clockwise) {
                    rgb_matrix_increase_val_noeeprom();
                } else {
                    rgb_matrix_decrease_val_noeeprom();
                }
            }
            break;
        case 1:
            if (index == _LEFT) {
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
            }
            else if (index == _MIDDLE) {
                if (clockwise) {
                    rgb_matrix_increase_hue();
                } else {
                    rgb_matrix_decrease_hue();
                }
            }
            else if (index == _RIGHT) {
                if (clockwise) {
                    rgb_matrix_increase_hue_noeeprom();
                } else {
                    rgb_matrix_decrease_hue_noeeprom();
                }
            }
            break;
    }
    return false;
}
