/*
This is the c configuration file for the keymap

Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifdef RGBLIGHT_ENABLE
//#    define RGBLIGHT_EFFECT_BREATHING
//#    define RGBLIGHT_EFFECT_RAINBOW_MOOD
//#    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
//#    define RGBLIGHT_EFFECT_SNAKE
//#    define RGBLIGHT_EFFECT_KNIGHT
//#    define RGBLIGHT_EFFECT_CHRISTMAS
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
//#    define RGBLIGHT_EFFECT_RGB_TEST
//#    define RGBLIGHT_EFFECT_ALTERNATING
//#    define RGBLIGHT_EFFECT_TWINKLE
#endif

#define SPLIT_USB_TIMEOUT 2500

// Reboot and retry master/slave detection when this half decided it is a
// slave but never receives communication from a master. This recovers the
// keyboard when USB enumeration loses the race against SPLIT_USB_TIMEOUT
// (e.g. behind a Keyboard Quantizer on cold boot or replug).
#define SPLIT_WATCHDOG_ENABLE

#define TAP_CODE_DELAY 5

#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 1
#define AUTO_MOUSE_TIME 10000

// スクロール速度を 1 段階遅くする (1/8 -> 1/16)。値が大きいほど遅い (範囲 1-7)。
// カーソル速度 (CPI) には影響しない。
#define KEYBALL_SCROLL_DIV_DEFAULT 5
