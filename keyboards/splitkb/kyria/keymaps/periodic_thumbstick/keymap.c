/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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

#include "keycodes.h"

#ifdef ENCODER_ENABLE
#    include "encoder_utils.h"
#endif

#ifdef OLED_ENABLE
#    include "oled_utils.h"
#endif

#ifdef THUMBSTICK_ENABLE
#    include "thumbstick.h"
#endif

#define KC_LCS  (QK_LCTL | QK_LSFT | KC_NO)

#define MC_LNCH LCMD(KC_SPC) // Launcher
#define MC_SS   C(G(KC_4))   // Screenshot with shift.
#define MC_SWAP LSA(KC_ENT)  // Swap main
#define MC_LYOT LSA(KC_SPC)  // Rotate layouts

enum custom_keycodes {
    TMB_MODE = SAFE_RANGE,
    G_ON,
    G_OFF,
    G_NEXT,
};

uint8_t game_layer = _MMORPG;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: Colemak
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   |   Q  |   W  |   f  |   p  |   G  |                              |   j  |   l  |   u  |   y  |   ;  |  - _   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/Esc|   A  |   r  |   s  |   t  |   D  |                              |   h  |   n  |   e  |   i  |   o  | Ctrl/' |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  | GAME |      |  |Leader| AltGr|   k  |   m  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | GUI  | UTIL | MOVE | BS   | Tab  |  | Enter| Space| WIN  | UTIL | Opt  |
 *                        |      |      |      |      | SYMB |  | SYMB |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_COLEMAK] = LAYOUT(
      KC_TAB,               KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                                        KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_MINS,
      MT(MOD_LGUI, KC_ESC), KC_A,    KC_R,    KC_S,    KC_T,    KC_D,                                        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    MT(MOD_RCTL, KC_QUOT),
      KC_LSFT,              KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    G_ON,    _______, QK_LEAD, KC_ALGR, KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
              KC_LCTL,
              MO(_UTIL),
              MO(_MOVE),
              KC_BSPC,
              LT(_SYMB, KC_TAB),
              LT(_SYMB, KC_ENT),
              KC_SPC,
              LM(_WIN, MOD_LGUI | MOD_LALT), // Switch to window layer with GUI down
              MO(_UTIL),
              KC_LOPT
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
      _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    _______,
      _______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, _______,
      _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    _______, _______, _______, _______, KC_N,    KC_M,    _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Overlay: Mac bindings
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |  GUI   |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | CTRL |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_MAC] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      MT(MOD_LGUI, KC_ESC), _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 KC_LCTL, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Symbol Layer: Numbers & Symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  *   |  1   |  2   |  3   |  +   |                              |   `  |  {   |  }   |  |   |  :   |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  -   |  4   |  5   |  6   |  0   |                              |   %  |  (   |  )   |  &   |  \   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  .   |  7   |  8   |  9   |  =   |      |      |  |      |      |   ~  |  [   |  ]   |  .   |  /   |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      | TRNS |  | TRNS |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYMB] = LAYOUT(
      _______, KC_ASTR, KC_1,    KC_2,    KC_3,    KC_PLUS,                                     KC_GRV,  KC_LCBR, KC_RCBR, KC_PIPE, KC_COLN, _______,
      _______, KC_MINS, KC_4,    KC_5,    KC_6,    KC_0   ,                                     KC_PERC, KC_LPRN, KC_RPRN, KC_AMPR, KC_BSLS, _______,
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
       _______, KC_Q,   KC_W,   KC_F,    KC_P,    KC_G,                                        KC_6,   KC_7,   KC_8,   KC_9,   KC_0, _______,
       _______, KC_A,   KC_R,   KC_S,    KC_T,    KC_D,                                        KC_LEFT,KC_DOWN, KC_UP, KC_RGHT, _______, _______,
       _______, KC_Z,   KC_X,   KC_C,    KC_V,    KC_B,    _______, _______, _______, _______, KC_1,   KC_2,   KC_3,   KC_4,   KC_5, _______,
                                _______, _______, _______, KC_BSPC, KC_TAB,  _______, _______, KC_TRNS, _______, _______
     ),
/*
 * Utility Layer: Function keys, RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        | F1   |  F2  | F3   | F4   | F5   |                              | F6   | F7   |  F8  | F9   | F10  | F11    |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        | TOG  | SAI  | HUI  | VAI  | MOD  |                              |QWERTY| COLE |      |      |      | F12    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      | SAD  | HUD  | VAD  | RMOD |      |      |  |      |      |      | GNext|      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_UTIL] = LAYOUT(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
      _______, RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD,                                     TG(_MAC), TO(_COLEMAK), _______, _______, _______, KC_F12,
      _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD,_______, _______, _______, _______, _______, G_NEXT,  _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * ARPG - Action RPGs
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Tab   |  8   |  7   |  6   |  5   |  r   |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Shft/Esc|  4   |  3   |  2   |  1   |  e   |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |  Ctrl  |  -   |  0   |  9   |  8   |  w   | GOFF |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |  alt |  l   |  s   |  c   | m    |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ARPG] = LAYOUT(
      KC_TAB,               KC_8,    KC_7,    KC_6,    KC_5,    KC_R,                                      _______, _______, _______, _______, _______, _______,
      MT(MOD_LSFT, KC_ESC), KC_4,    KC_3,    KC_2,    KC_1,    KC_E,                                      _______, _______, _______, _______, _______, _______,
      KC_LCTL,              KC_MINS, KC_0,    KC_9,    KC_8,    KC_Y,    G_OFF,   _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                              KC_L,    KC_S,    KC_C,    KC_SPC,  _______, _______, _______, _______, _______, _______
    ),
/*
 * MMORPG
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |   `    | Tab  |  8   |  7   |  6   |  5   |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |  Esc   | Ctrl |  3   |  2   |  1   |  4   |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |   p    | Shift|  -   |  0   |  9   |  b   | GOFF |      |  |       |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |  x   |  c   |  m   | Space|      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_MMORPG] = LAYOUT(
      KC_GRV,  KC_TAB,  KC_8,    KC_7,    KC_6,    KC_5,                                      _______, _______, _______, _______, _______, _______,
      KC_ESC,  KC_LCTL, KC_3,    KC_2,    KC_1,    KC_4,                                      _______, _______, _______, _______, _______, _______,
      KC_LCS,  KC_LSFT, KC_MINS, KC_0,    KC_9,    KC_B, G_OFF,   _______,  _______, _______, _______, _______, _______, _______, _______, _______,
                                 KC_X,    KC_C,    KC_M, KC_SPC,  _______,  _______, _______, _______, _______, _______
    ),
/*
 * FPS - First-Person Shooters
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  T   |  Tab   |  Q   |  W   |  E   |  R   |                              |      |      |      |      |      |        |
 * |------+--------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |  G   |  Ctrl  |  A   |  S   |  D   |  F   |                              |      |      |      |      |      |        |
 * |------+--------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |  B   | Shift  |  Z   |  X   |  C   |  V   | GOFF |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | Esc  |      | M    | Space| Ctrl |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_FPS] = LAYOUT(
      KC_T,  KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,                                      _______, _______, _______, _______, _______, _______,
      KC_G,  KC_LCTL,   KC_A,    KC_S,    KC_D,    KC_F,                                      _______, _______, _______, _______, _______, _______,
      KC_B,  KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_V, G_OFF,    _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 KC_ESC, MO(_UTIL),KC_M, KC_SPC, MO(_SYMB), _______, _______, _______, _______, _______
    ),
/*
 * FPS 2 - First-Person Shooters with thumbstick
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  T   |  Tab   |  Q   |  1   |  E   |  R   |                              |      |      |      |      |      |        |
 * |------+--------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |  G   |  Ctrl  |  4   |  3   |  2   |  F   |                              |      |      |      |      |      |        |
 * |------+--------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |  B   | Shift  |  Z   |  X   |  C   |  V   | GOFF |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | Esc  |      | M    | Space| Stick|  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_FPS2] = LAYOUT(
      KC_T,  KC_TAB,    KC_Q,    KC_1,    KC_E,    KC_R,                                      _______, _______, _______, _______, _______, _______,
      KC_G,  KC_LCTL,   KC_4,    KC_3,    KC_2,    KC_F,                                      _______, _______, _______, _______, _______, _______,
      KC_B,  KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_V, G_OFF,    _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 KC_ESC, MO(_UTIL),KC_M, KC_SPC, MO(_SYMB), _______, _______, _______, _______, _______
    ),
/*
 * MHR - Monster Hunter: Rise
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  T   |  Tab   |  Q   |  W   |  E   |  R   |                              |      |      |      |      |      |        |
 * |------+--------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |  G   |  Ctrl  |  A   |Space |  D   |  F   |                              |      |      |      |      |      |        |
 * |------+--------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |  B   | Shift  |  Z   |  X   |  C   |  V   | GOFF |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | Esc  |      | M    | Space|      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_MHR] = LAYOUT(
      KC_T,  KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,                                    _______, _______, _______, _______, _______, _______,
      KC_G,  KC_LCTL,   KC_A,    KC_SPC,  KC_D,    KC_F,                                    _______, _______, _______, _______, _______, _______,
      KC_B,  KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_V, G_OFF,  _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 KC_ESC, MO(_UTIL),KC_M, KC_SPC, _______, _______, _______, _______, _______, _______
    ),
};
// clang-format on

void matrix_init_user(void) {
#ifdef ENCODER_ENABLE
    encoder_utils_init();
#endif
}

// layer_state_t layer_state_set_user(layer_state_t state) {
//     return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
// }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case G_ON:
            if (record->event.pressed) {
                layer_on(game_layer);
            }
            break;
        case G_OFF:
            if (record->event.pressed) {
                layer_off(game_layer);
            }
            break;
        case G_NEXT:
            if (record->event.pressed) {
                switch (game_layer) {
                    case _MMORPG:
                    game_layer = _MHR;
                    if (IS_LAYER_ON(_MMORPG)) {
                        layer_off(_MMORPG);
                        layer_on(_MHR);
                    }
                    break;
                    case _MHR:
                    game_layer = _ARPG;
                    if (IS_LAYER_ON(_MHR)) {
                        layer_off(_MHR);
                        layer_on(_ARPG);
                    }
                    break;
                    case _ARPG:
                    game_layer = _FPS;
                    if (IS_LAYER_ON(_ARPG)) {
                        layer_off(_ARPG);
                        layer_on(_FPS);
                    }
                    break;
                    case _FPS:
                    game_layer = _FPS2;
                    if (IS_LAYER_ON(_FPS)) {
                        layer_off(_FPS);
                        layer_on(_FPS2);
                    }
                    break;
                    case _FPS2:
                    default: // Just in case something goes wrong.
                    game_layer = _MMORPG;
                    if (IS_LAYER_ON(_FPS2)) {
                        layer_off(_FPS2);
                        layer_on(_MMORPG);
                    }
                    break;
                }
            }
            break;
#ifdef ENCODER_ENABLE
        case ENC_MODE_L:
            if (record->event.pressed) {
                cycle_encoder_mode(true, false);
            }
            break;
        case ENC_MODE_R:
            if (record->event.pressed) {
                cycle_encoder_mode(false, false);
            }
            break;
#endif
#ifdef THUMBSTICK_ENABLE
        case TMB_MODE:
            if (record->event.pressed) {
                thumbstick_mode_cycle(false);
            }
#endif
    }
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
#ifdef THUMBSTICK_ENABLE
    // Set up custom thumbstick modes for certail layouts.
    switch(get_highest_layer(state)) {
        case _FPS:
            thumbstick_mode_set(THUMBSTICK_MODE_NUMBERS);
            break;
        default:
            thumbstick_mode_set(THUMBSTICK_MODE_ARROWS);
            break;
    }
#endif

    return state;
}

void keyboard_post_init_user(void) {
    layer_state_set_user(layer_state);
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

bool oled_task_user(void) {
    render_status();
    return false;
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        encoder_action(get_encoder_mode(true), clockwise);
#    ifdef OLED_ENABLE
        oled_on();
#    endif
    } else if (index == 1) {
        encoder_action(get_encoder_mode(false), clockwise);
#    ifdef OLED_ENABLE
        oled_on();
#    endif
    }
    return true;
}
#endif
