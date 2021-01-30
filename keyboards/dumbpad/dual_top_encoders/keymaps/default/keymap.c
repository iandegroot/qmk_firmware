/* Copyright 2020 imchipwood
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

#define _BASE 0
#define _SUB  1
#define _DBG  2

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        BASE LAYER
    /-----------------------------------------------------`
    |             |  Mute   | MO(DBG) |  Play   | TO(SUB) |
    |             |---------|---------|---------|---------|
    |             |   F9    |   F10   |   F11   |   F12   |
    |             |---------|---------|---------|---------|
    |             |   F5    |   F6    |   F7    |   F8    |
    |-------------|---------|---------|---------|---------|
    |             |   F1    |   F2    |   F3    |   F4    |
    \-----------------------------------------------------'
    */
    [_BASE] = LAYOUT(
                    KC_MUTE,   MO(_DBG), KC_MPLY,  TO(_SUB),
                    KC_F9,     KC_F10,   KC_F11,   KC_F12,
                    KC_F5,     KC_F6,    KC_F7,    KC_F8,
        _______,    KC_F1,     KC_F2,    KC_F3,    KC_F4
    ),
    /*
        SUB LAYER
    /-----------------------------------------------------`
    |             |    .    |    /    |    *    | TO(BASE)|
    |             |---------|---------|---------|---------|
    |             |    9    |    0    |    -    |    +    |
    |             |---------|---------|---------|---------|
    |             |    5    |    6    |    7    |    8    |
    |-------------|---------|---------|---------|---------|
    |    Enter    |    1    |    2    |    3    |    4    |
    \-----------------------------------------------------'
    */
    [_SUB] = LAYOUT(
                    KC_PDOT,  KC_PSLS,  KC_PAST,  TO(_BASE),
                    KC_9,     KC_0,     KC_PMNS,  KC_PPLS,
                    KC_5,     KC_6,     KC_7,     KC_8,
        KC_ENTER,   KC_1,     KC_2,     KC_3,     KC_4
    ),
    /*
        DEBUG LAYER
    /-----------------------------------------------------`
    |             |         |         |         |  Reset  |
    |             |---------|---------|---------|---------|
    |             |         |         |         |         |
    |             |---------|---------|---------|---------|
    |             |         |         |         |         |
    |-------------|---------|---------|---------|---------|
    |             |         |         |         |         |
    \-----------------------------------------------------'
    */
    [_DBG] = LAYOUT(
                    _______,  _______,  _______,  RESET,
                    _______,  _______,  _______,  _______,
                    _______,  _______,  _______,  _______,
         _______,   _______,  _______,  _______,  _______
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // If console is enabled, it will print the matrix position and status of each key pressed
/*
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed);
#endif
*/
    return true;
}

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    // debug_enable = true;
    // debug_matrix = true;
    // debug_keyboard = true;
    // debug_mouse = true;
}

void encoder_update_user(uint8_t index, bool clockwise) {
    /*  Custom encoder control - handles CW/CCW turning of encoder
     *  Default behavior:
     *    left encoder:
     *      CW:  Alt + Tab
     *      CCW: Shift + Alt + Tab
     *    right encoder:
     *      CW:  volume up
     *      CCW: volume down     
     */
    if (index == 0) {
        // Alt tab through open applications
        if (clockwise) {
            if (!is_alt_tab_active) {
                is_alt_tab_active = true;
                register_code(KC_LALT);
            }
            alt_tab_timer = timer_read();
            tap_code16(KC_TAB);
        } else {
            alt_tab_timer = timer_read();
            tap_code16(S(KC_TAB));
        }
    } else if (index == 1) {
        // Volume up (CW) and down (CCW)
        if (clockwise) {    
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
}

void matrix_scan_user(void) {
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 700) {
            unregister_code(KC_LALT);
            is_alt_tab_active = false;
        }
    }
}
