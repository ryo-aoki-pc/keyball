/*
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

#include QMK_KEYBOARD_H

#include "quantum.h"

// Keyboard Quantizer Mini (vial-qmk-kq-mini) 併用前提のキーマップ。
// MT/LT・記号・Vim レイヤーはすべて kq-mini 側 (LisM キーマップの EEPROM
// デフォルト) が処理するため、ベースレイヤーは LisM の BASE 配列に対応する
// 素の HID コードを送るだけにする。レイヤーキーは lism.vialmap.json の
// 割当に合わせる: &mo FUNC → Grave / &mo SYM → 右 Alt / &mo VIM_BASE → CapsLock。
// kq-mini に含まれないマウスレイヤー (LisM の MOUSE_MOVE / MOUSE_SCROLL) は
// 本体側のレイヤー 1 (AML) / 2 で再現する。

// LisM の MOUSE_SCROLL に相当するスクロールレイヤー
#define KEYBALL_SCROLL_LAYER 2

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // LisM BASE 相当 (kq-mini が MT/LT を付与するため素のキーコードのみ)
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_MINS  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    KC_GRV   , KC_LGUI  , KC_LALT  , KC_SPC   , KC_SPC   , KC_RALT  ,      KC_CAPS  , KC_ENT   , KC_NO    , KC_NO    , KC_GRV   , KC_GRV
  ),

  // LisM MOUSE_MOVE 相当 (AML レイヤー): D / K でスクロールレイヤーへ
  [1] = LAYOUT_universal(
    KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                            KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,
    KC_NO    , KC_NO    , MO(2)    , KC_NO    , KC_NO    ,                            KC_NO    , KC_NO    , MO(2)    , KC_NO    , KC_NO    ,
    KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                            KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,
    KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,      KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO
  ),

  // LisM MOUSE_SCROLL 相当: ボールはスクロール、S/F/J/L=クリック、X/V/M/.=戻る/進む
  [2] = LAYOUT_universal(
    KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                            KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,
    KC_NO    , KC_BTN2  , KC_NO    , KC_BTN1  , KC_NO    ,                            KC_NO    , KC_BTN1  , KC_NO    , KC_BTN2  , KC_NO    ,
    KC_NO    , KC_BTN5  , KC_NO    , KC_BTN4  , KC_NO    ,                            KC_NO    , KC_BTN4  , KC_NO    , KC_BTN5  , KC_NO    ,
    KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,      KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO
  ),

  // 設定レイヤー (RGB / AML / スクロールスナップ / スクロール速度 / CPI)
  [3] = LAYOUT_universal(
    RGB_TOG  , AML_TO   , AML_I50  , AML_D50  , _______  ,                            _______  , _______  , SSNP_HOR , SSNP_VRT , SSNP_FRE ,
    RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI  , SCRL_DVI ,                            _______  , _______  , _______  , _______  , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , SCRL_DVD ,                            CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , KBC_SAVE ,
    QK_BOOT  , KBC_RST  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , KBC_RST  , QK_BOOT
  ),
};
// clang-format on

#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE

// AML 中も押下でデフォルトレイヤーに戻さない物理キー (LisM の
// &zip_temp_layer excluded-positions = <12 17> = D / K に相当)。
// keyball39.h の LAYOUT_no_ball で D = L12 (row 1, col 2)、
// K = R12 (右手側は row 4〜7 で R1x が row 5、col 2)。
#define KEYBALL_D_KEYPOS_ROW 1
#define KEYBALL_D_KEYPOS_COL 2
#define KEYBALL_K_KEYPOS_ROW 5
#define KEYBALL_K_KEYPOS_COL 2

static bool is_keyball_aml_excluded_key(keyrecord_t *record) {
  return (record->event.key.row == KEYBALL_D_KEYPOS_ROW &&
          record->event.key.col == KEYBALL_D_KEYPOS_COL) ||
         (record->event.key.row == KEYBALL_K_KEYPOS_ROW &&
          record->event.key.col == KEYBALL_K_KEYPOS_COL);
}

static bool is_auto_mouse_allowed_layer(uint8_t layer) {
  return layer == get_auto_mouse_layer() || layer == KEYBALL_SCROLL_LAYER;
}

static bool is_auto_mouse_allowed_key(uint16_t keycode) {
  if (keycode >= QK_MODS && keycode <= QK_MODS_MAX) {
    keycode &= 0xff;
  }

  switch (keycode) {
    case KC_NO:
    case KC_TRANSPARENT:
    case KC_LEFT_CTRL ... KC_RIGHT_GUI:
    case KC_MS_BTN1 ... KC_MS_BTN8:
    case SCRL_MO:
    case AML_TO:
    case AML_I50:
    case AML_D50:
      return true;
  }

  switch (keycode) {
    case QK_TO ... QK_TO_MAX:
      return is_auto_mouse_allowed_layer(QK_TO_GET_LAYER(keycode));
    case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
      return is_auto_mouse_allowed_layer(QK_TOGGLE_LAYER_GET_LAYER(keycode));
    case QK_MOMENTARY ... QK_MOMENTARY_MAX:
      return is_auto_mouse_allowed_layer(QK_MOMENTARY_GET_LAYER(keycode));
    case QK_LAYER_MOD ... QK_LAYER_MOD_MAX:
      return is_auto_mouse_allowed_layer(QK_LAYER_MOD_GET_LAYER(keycode));
#    ifndef NO_ACTION_TAPPING
    case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
      return is_auto_mouse_allowed_layer(QK_LAYER_TAP_TOGGLE_GET_LAYER(keycode));
    case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
      return is_auto_mouse_allowed_layer(QK_LAYER_TAP_GET_LAYER(keycode));
#    endif
  }

  return false;
}

// AML 有効中、物理 D / K キーをマウスキー扱いとし AML を解除させない。
bool is_mouse_record_user(uint16_t keycode, keyrecord_t *record) {
  return is_keyball_aml_excluded_key(record) &&
         layer_state_is(get_auto_mouse_layer());
}

void keyboard_post_init_user(void) {
  set_auto_mouse_enable(true);
  set_auto_mouse_timeout(AUTO_MOUSE_TIME);
  // スクロールを上下左右フリー方向にする（既定の縦固定を解除）
  keyball_set_scrollsnap_mode(KEYBALL_SCROLLSNAP_MODE_FREE);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed && layer_state_is(get_auto_mouse_layer()) &&
      !is_keyball_aml_excluded_key(record) &&
      !is_auto_mouse_allowed_key(keycode)) {
    auto_mouse_reset_trigger(true);
  }

  return true;
}

#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    // スクロールレイヤーが最上位のときだけボールをスクロールモードにする
    keyball_set_scroll_mode(get_highest_layer(state) == KEYBALL_SCROLL_LAYER);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
