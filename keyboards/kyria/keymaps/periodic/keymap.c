/* Copyright 2020 Drew Haven <drew.haven@gmail.com>
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
#include <stdio.h>
#include <string.h>
#include "user_buffer.h"
#include "oled.h"

#define RGB_CORANGE 0xff, 0x62, 0x72

#define MC_LNCH LCMD(KC_SPC) // Launcher
#define MC_SS   C(G(KC_4))   // Screenshot with shift.
#define MC_SWAP LSA(KC_ENT)  // Swap main
#define MC_LYOT LSA(KC_SPC)  // Rotate layouts

typedef struct _UserBuffer {
    uint8_t layer_state;
} UserBuffer;

enum layers {
    _COLEMAK = 0,
    _QWERTY,
    _MAC,
    _SYMB,
    _MOVE,
    _UTIL,
    _WIN,
    _MACWIN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: Colemak
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   |   Q  |   W  |   f  |   p  |   G  |                              |   j  |   l  |   u  |   y  |   ;  |  - _   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   r  |   s  |   t  |   D  |                              |   h  |   n  |   e  |   i  |   o  |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      | Prev |  | Next | Mac  |   k  |   m  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | GUI  | UTIL | MOVE | BS   | Tab  |  | Enter| Space| WIN  | UTIL | AltGr|
 *                        |      |      |      |      | SYMB |  | SYMB |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_COLEMAK] = LAYOUT(
      KC_TAB,               KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                                             KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_MINS,
      MT(MOD_LCTL, KC_ESC), KC_A,    KC_R,    KC_S,    KC_T,    KC_D,                                             KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
      KC_LSFT,              KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    _______, _______, TO(_QWERTY), TG(_MAC), KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
              KC_LGUI,
              MO(_UTIL),
              MO(_MOVE),
              KC_BSPC,
              LT(_SYMB, KC_TAB),
              LT(_SYMB, KC_ENT),
              KC_SPC,
              MO(_WIN),
              MO(_UTIL),  // Redundant?
              KC_RALT   // Redundant?
    ),
/*
 * Alpha Layer: Qwerty fallback
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  Q   |  W   |  e   |  r   |  t   |                              |  y   |  u   |  i   |  o   |  p   |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  a   |  s   |  d   |  f   |  g   |                              |  h   |  j   |  k   |  l   |  ;   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  z   |  x   |  c   |  v   |  b   |      |      |  |      |      |  n   |  m   |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
      _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    _______,
      _______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                          KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, _______,
      _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B, _______, _______, TO(_COLEMAK), _______, KC_N,    KC_M,    _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______
    ),
/*
 * Symbol Layer: Numbers & Symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  *   |  1   |  2   |  3   |  +   |                              |   `  |  {   |  }   |  |   |  \   |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  -   |  4   |  5   |  6   |  0   |                              |      |  (   |  )   |  *   |  :   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  .   |  7   |  8   |  9   |  =   |      |      |  |      |      |   ~  |  [   |  ]   |  .   |  /   |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      | TRNS |  | TRNS |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYMB] = LAYOUT(
      _______, KC_ASTR, KC_1,    KC_2,    KC_3,    KC_PLUS,                                     KC_GRV,  KC_LCBR, KC_RCBR, KC_PIPE, KC_BSLS, _______,
      _______, KC_MINS, KC_4,    KC_5,    KC_6,    KC_0   ,                                     _______, KC_LPRN, KC_RPRN, KC_ASTR, KC_COLN, _______,
      _______, KC_DOT,  KC_7,    KC_8,    KC_9,    KC_EQL,  _______, _______, _______, _______, KC_TILD, KC_LBRC, KC_RBRC, KC_DOT,  KC_SLSH, _______,
                                 _______, _______, _______, _______, KC_TRNS, KC_TRNS, _______, _______, _______, _______
    ),
/*
 * Movement Layer: media, navigation
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              | Home | PgDn | PgUp | End  |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | Prev | Play | Next |      |                              | Left | Down | Up   | Right|      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      | Mute | VolDn| VolUp|      |      |      |  |      |      | MLeft| Mdown| MUp  |MRight|      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      | TRNS | Del  |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_MOVE] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     KC_HOME, KC_PGDN, KC_PGUP, KC_END,    KC_0,    _______,
      _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, _______,                                     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,
      _______, _______, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, _______, _______, _______, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______, _______,
                                 _______, _______, KC_TRNS, KC_DEL,  _______, _______, _______, _______, _______, _______
    ),
 /*
  * WIN Layer: Window functions
  * TODO: Add i3 functions
  *
  * ,-------------------------------------------.                              ,-------------------------------------------.
  * |        | Quit |Tabbed| Split|Reload| Term |                              | WS 6 | WS 7 | WS 8 | WS 9 |      |        |
  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
  * |        |Parent| Stack|Launch| Full | Float|                              | Left |  Up  | Down | Right|      |        |
  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
  * |        |      |      | Conf | Vert |Browse|      |      |  |      |      | WS 1 | WS 2 | ws 3 | ws 4 | ws 5 |       |
  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
  *                        |      |      |      |      |      |  |      |      |      |      |      |
  *                        |      |      |      |      |      |  |      |      |      |      |      |
  *                        `----------------------------------'  `----------------------------------'
  */
     [_WIN] = LAYOUT(
       _______, G(KC_Q), G(KC_W), G(KC_E), G(KC_R), G(KC_T),                                     G(KC_6), G(KC_7), G(KC_8), G(KC_9), _______, _______,
       _______, G(KC_A), G(KC_S), G(KC_D), G(KC_F), G(KC_SPC),                                   G(KC_H), G(KC_J), G(KC_K), G(KC_L), _______, _______,
       _______, _______, _______, G(KC_C), G(KC_V), G(KC_B), _______, _______, _______, _______, G(KC_1), G(KC_2), G(KC_3), G(KC_4), G(KC_5), _______,
                                  _______, _______, _______, _______, _______, _______, _______, KC_TRNS, _______, _______
     ),
/*
 * Utility Layer: Function keys, RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        | F1   |  F2  | F3   | F4   | F5   |                              | F6   | F7   |  F8  | F9   | F10  | F11    |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        | TOG  | SAI  | HUI  | VAI  | MOD  |                              |      |      |      |      |      | F12    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      | SAD  | HUD  | VAD  | RMOD |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | CAPS | TRNS |      |      |      |  |      |      |      | TRNS |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_UTIL] = LAYOUT(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
      _______, RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD,                                     _______, _______, _______, _______, _______, KC_F12,
      _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD,_______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 KC_CAPS, KC_TRNS, _______, _______, _______, _______, _______, _______, KC_TRNS, _______
    ),
 /*
  * Mac Layer: Update keys for mac
  *
  * ,-------------------------------------------.                              ,-------------------------------------------.
  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
  * | Cmd/Esc|      |      |      |      |      |                              |      |      |      |      |      |        |
  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
  *                        | Ctrl |      |      |      |      |  |      |      |MACWIN|      |      |
  *                        |      |      |      |      |      |  |      |      |      |      |      |
  *                        `----------------------------------'  `----------------------------------'
  */
     [_MAC] = LAYOUT(
       _______,              _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
       MT(MOD_LGUI, KC_ESC), _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
       _______,              _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                      KC_LCTL, _______, _______, _______, _______, _______, _______, MO(_MACWIN), _______, _______
     ),
 /*
  * Mac Window Layer:
  *
  * ,-------------------------------------------.                              ,-------------------------------------------.
  * |  Apps  | Sleep|      |      |      |      |                              | ws 6 | ws 7 | ws 8 | ws 9 |      |        |
  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
  * |        |      | ScSh |Launch| Full |      |                              | WLeft| WDown| WUp  |WRight|      |        |
  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
  * |        |      |      |      |      |      |      |      |  |      |      | ws 1 | ws 2 | ws 3 | ws 4 | ws 5 |        |
  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
  *                        |      |      |      | Main | Lout |  |      |      | TRNS |      |      |
  *                        |      |      |      |      |      |  |      |      |      |      |      |
  *                        `----------------------------------'  `----------------------------------'
  */
     [_MACWIN] = LAYOUT(
       G(KC_TAB), A(KC_EJCT), _______, _______, _______,   _______,                                     LCA(KC_6), LCA(KC_7), LCA(KC_8), LCA(KC_9), _______, _______,
       _______,   _______,    MC_SS,   MC_LNCH, MEH(KC_F), _______,                                     LCA(KC_H), LCA(KC_J), LCA(KC_K), LCA(KC_L), _______, _______,
       _______,   _______,    _______, _______, _______,   _______, _______, _______, _______, _______, LCA(KC_1), LCA(KC_2), LCA(KC_3), LCA(KC_4), LCA(KC_5), _______,
                                       _______, _______,   _______, MC_SWAP, MC_LYOT, _______, _______, KC_TRNS, _______, _______
     ),
// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

void _set_hsv(uint8_t h, uint8_t s, uint8_t v) {
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_sethsv_noeeprom(h, s, v);
}

void _disable_rgb(void) {
    rgblight_disable_noeeprom();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (is_keyboard_master()) {
        set_user_buffer(&state);
    }
    switch (get_highest_layer(state)) {
        case _QWERTY:
            _set_hsv(HSV_CYAN);
            break;
        case _COLEMAK:
            _disable_rgb();
            break;
        case _MAC:
            _set_hsv(HSV_CORAL);
            break;
        case _SYMB:
            _set_hsv(HSV_PURPLE);
            break;
        case _MOVE:
            _set_hsv(HSV_GREEN);
            break;
        case _UTIL:
            _set_hsv(HSV_MAGENTA);
            break;
        case _WIN:
        case _MACWIN:
            _set_hsv(HSV_SPRINGGREEN);
            break;
        default:
            _set_hsv(HSV_WHITE);
            break;
    }

    return state;
}

void keyboard_post_init_user(void) {
    layer_state_set_user(layer_state);
}


#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}

static void render_status(void) {
    // QMK Logo and version information
    render_qmk_logo();
    oled_write_P(PSTR("Kyria rev1.0\n\n"), false);

    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("Qwerty\n"), false);
            break;
        case _COLEMAK:
            oled_write_P(PSTR("Colemak\n"), false);
            break;
        case _MAC:
            if (IS_LAYER_ON(_QWERTY)) {
                oled_write_P(PSTR("Qwerty + Mac\n"), false);
                break;
            }
            if (IS_LAYER_ON(_COLEMAK)) {
                oled_write_P(PSTR("Colemak + Mac\n"), false);
                break;
            }
            oled_write_P(PSTR("\?\?\?/Mac\n"), false);
            break;
        case _SYMB:
            oled_write_P(PSTR("Symbols\n"), false);
            break;
        case _MOVE:
            oled_write_P(PSTR("Movement\n"), false);
            break;
        case _WIN:
            oled_write_P(PSTR("GUI\n"), false);
            break;
        case _UTIL:
            oled_write_P(PSTR("Util\n"), false);
            break;
        case _MACWIN:
            oled_write_P(PSTR("Mac GUI\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
    }

    // Host Keyboard LED Status
    uint8_t led_usb_state = host_keyboard_leds();
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
        UserBuffer user_buffer = {};
        memcpy(&user_buffer, get_user_buffer(), SPLIT_USER_BUFFER_SIZE);

        // TODO
        if (layer_state_cmp(user_buffer.layer_state, _MAC)) {
            render_asana_logo();
        } else {
            render_kyria_logo();
        }
    }
}

#endif
