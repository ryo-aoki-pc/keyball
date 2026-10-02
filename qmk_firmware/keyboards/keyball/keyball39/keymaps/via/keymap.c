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

#if defined(RGBLIGHT_ENABLE) || defined(VIA_ENABLE)
#    include "version.h"
#endif

#ifdef VIA_ENABLE
#    include <string.h>
#    include "via.h"
#endif

// Keyboard Quantizer Mini (vial-qmk-kq-mini) 併用前提のキーマップ。
// MT/LT・記号・Vim レイヤーはすべて kq-mini 側 (LisM キーマップの EEPROM
// デフォルト) が処理するため、ベースレイヤーは LisM の BASE 配列に対応する
// 素の HID コードを送るだけにする。レイヤーキーは lism.vialmap.json の
// 割当に合わせる: &mo FUNC → Grave / &mo SYM → 右 Alt / &mo VIM_BASE → CapsLock。
// kq-mini に含まれないマウスレイヤー (LisM の MOUSE_MOVE / MOUSE_SCROLL) は
// 本体側のレイヤー 1 (AML) / 2 で再現する。
// AML レイヤーでは、修飾キーの位置 (kq-mini が mod-tap にする A = 左 Ctrl /
// - = 右 Ctrl / Z = 左 Shift / / = 右 Shift と、ベースの Win / Alt) をベースと同じ
// キー (KC_TRNS) にし、押すと AML を解除する (LisM の MOUSE_MOVE の &trans と同じ)。
// A / - / Z / / は kq-mini の mod-tap になる (タップで文字、長押しで Ctrl / Shift)。
// スクロールレイヤーでは、これらの位置を素の修飾キーにする (kq-mini はそのまま
// 素通しする)。D / K を押してから修飾キーを押せば、Shift + クリックなどができる。

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

  // LisM MOUSE_MOVE 相当 (AML レイヤー): D / K でスクロールレイヤーへ、
  // A / - / Z / / / Win / Alt はベースと同じキー (押すと AML を解除)
  [1] = LAYOUT_universal(
    KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                            KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,
    _______  , KC_NO    , MO(2)    , KC_NO    , KC_NO    ,                            KC_NO    , KC_NO    , MO(2)    , KC_NO    , _______  ,
    _______  , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                            KC_NO    , KC_NO    , KC_NO    , KC_NO    , _______  ,
    KC_NO    , _______  , _______  , KC_NO    , KC_NO    , KC_NO    ,      KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO
  ),

  // LisM MOUSE_SCROLL 相当: ボールはスクロール、S/F/J/L=クリック、X/V/M/.=戻る/進む、
  // A / - / Z / / / Win / Alt は修飾キー
  [2] = LAYOUT_universal(
    KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                            KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,
    KC_LCTL  , KC_BTN2  , KC_NO    , KC_BTN1  , KC_NO    ,                            KC_NO    , KC_BTN1  , KC_NO    , KC_BTN2  , KC_RCTL  ,
    KC_LSFT  , KC_BTN5  , KC_NO    , KC_BTN4  , KC_NO    ,                            KC_NO    , KC_BTN4  , KC_NO    , KC_BTN5  , KC_RSFT  ,
    KC_NO    , KC_LGUI  , KC_LALT  , KC_NO    , KC_NO    , KC_NO    ,      KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO
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
// &zip_temp_layer excluded-positions の 12 / 17 = D / K に相当)。
// ほかのキーは修飾キーも含めて、押すと AML を解除する (QMK の process_auto_mouse() は
// 修飾キーを無視するので、is_auto_mouse_allowed_key() で修飾キーを許可せず、
// process_record_user() で解除する)。
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed && layer_state_is(get_auto_mouse_layer()) &&
      !is_keyball_aml_excluded_key(record) &&
      !is_auto_mouse_allowed_key(keycode)) {
    auto_mouse_reset_trigger(true);
  } else if (!layer_state_is(get_auto_mouse_layer()) && IS_MODIFIER_KEYCODE(keycode)) {
    // QMK (process_auto_mouse) は修飾キーでは AUTO_MOUSE_DELAY を数え直さない。Win / Alt と
    // SYM の親指キー (右 Alt) を押した・離したときの振動でも AML にならないよう数え直す
    // (ほかのキーと同じ。ZMK の aml_threshold もすべてのキーを数える)
    auto_mouse_reset_trigger(record->event.pressed);
  }

  return true;
}

// AML を発動するか (QMK の weak 関数 auto_mouse_activation を置き換える)。キー入力の振動などで
// ボールがわずかに動いても AML にしないため、止まっていた状態から動いた量を X と Y それぞれ
// 向き付きで足し (行ったり来たりする振動は打ち消し合う)、大きさ (大きいほう + 小さいほうの半分) が
// KEYBALL_AML_THRESHOLD に達したときだけ発動する。KEYBALL_AML_IDLE_MS 以上動きが途切れたら
// 数え直す。QMK はキーを押した・離したあと AUTO_MOUSE_DELAY の間この関数を呼ばないので、
// その後は途切れていたとみなして 0 から数える。ZMK の aml_threshold と同じ判定。
// AML 中とスクロール・ボタンは従来どおり (動けばタイムアウトを延ばす)。
#define KEYBALL_AML_IDLE_MS 100

bool auto_mouse_activation(report_mouse_t r) {
  static int16_t  sum_x, sum_y;
  static uint16_t last_move;

  if (r.buttons || r.h || r.v || layer_state_is(get_auto_mouse_layer())) {
    sum_x = sum_y = 0;
    return r.x || r.y || r.h || r.v || r.buttons;
  }
  if (!r.x && !r.y) {
    return false;
  }
  if (timer_elapsed(last_move) > KEYBALL_AML_IDLE_MS) {
    sum_x = sum_y = 0;
  }
  last_move = timer_read();
  // 1 回の報告は ±127 で、しきい値に達すると 0 に戻すので、あふれない
  sum_x += r.x;
  sum_y += r.y;
  uint16_t ax = sum_x < 0 ? -sum_x : sum_x;
  uint16_t ay = sum_y < 0 ? -sum_y : sum_y;
  if ((ax > ay ? ax + ay / 2 : ay + ax / 2) < KEYBALL_AML_THRESHOLD) {
    return false;
  }
  sum_x = sum_y = 0;
  return true;
}

#endif

#ifdef RGBLIGHT_ENABLE

// LED の設定は左右の Pro Micro に別々に保存され、USB 側の設定が左右に使われる。
// config.h の RGBLIGHT_DEFAULT_* を新しいファームの初回起動時に保存し、同じ
// ファームを書いた左右で揃える。適用済みのビルドはビルド日時のハッシュを
// EEPROM の user 領域に記録して判定する (それ以降に VIA で変えた設定は、次に
// ファームを書き込むまで保持される)。
static uint32_t build_stamp(void) {
  uint32_t hash = 2166136261UL; // FNV-1a
  for (const char *p = QMK_BUILDDATE; *p != '\0'; p++) {
    hash = (hash ^ (uint8_t)*p) * 16777619UL;
  }
  return hash;
}

static void apply_rgblight_defaults_once(void) {
  uint32_t stamp = build_stamp();
  if (eeconfig_read_user() == stamp) {
    return;
  }
  eeconfig_update_rgblight_default();
  rgblight_reload_from_eeprom();
  eeconfig_update_user(stamp);
}

#endif

// カーソルの加速。ボールを転がす速さに応じて移動量に倍率を掛け、ゆっくり動かしたときは
// カーソルを細かく、速く動かしたときは遠くまで動かす (倍率は config.h の KEYBALL_ACCEL_*)。
// 速さは X と Y を合わせた移動量から求めるので、斜めでも縦横と同じ倍率になる。
// 1 に満たない端数は軸ごとに次へ持ち越す (0.5 倍でも 2 カウントで 1 動く)。
// lib/keyball の keyball_on_apply_motion_to_mouse_move (weak) を置き換える。左右それぞれの
// ボールの移動量が KEYBALL_REPORTMOUSE_INTERVAL (8ms) ごとに渡される (動いていなくても呼ばれる)。
//
// フラッシュの残りが少ないため、AVR で 32 ビットの割り算をしない単位に直して計算する
// (コンパイル時に計算する)。倍率は 256 = 等倍。速さは 1 回の報告あたりの移動量の 16 倍。
#define KEYBALL_ACCEL_MIN       ((uint16_t)(KEYBALL_ACCEL_MIN_FACTOR * 256L / 1000))
#define KEYBALL_ACCEL_MAX       ((uint16_t)(KEYBALL_ACCEL_MAX_FACTOR * 256L / 1000))
#define KEYBALL_ACCEL_THRESHOLD ((uint16_t)(KEYBALL_ACCEL_SPEED_THRESHOLD * 16L * KEYBALL_REPORTMOUSE_INTERVAL / 1000))
#define KEYBALL_ACCEL_SPEED     ((uint16_t)(KEYBALL_ACCEL_SPEED_MAX * 16L * KEYBALL_REPORTMOUSE_INTERVAL / 1000))

_Static_assert(KEYBALL_ACCEL_MIN <= 256 && KEYBALL_ACCEL_MAX >= 256, "KEYBALL_ACCEL_MIN_FACTOR <= 1000 <= KEYBALL_ACCEL_MAX_FACTOR");
_Static_assert(0 < KEYBALL_ACCEL_THRESHOLD && KEYBALL_ACCEL_THRESHOLD < KEYBALL_ACCEL_SPEED, "0 < KEYBALL_ACCEL_SPEED_THRESHOLD < KEYBALL_ACCEL_SPEED_MAX");
_Static_assert((256L - KEYBALL_ACCEL_MIN) * KEYBALL_ACCEL_THRESHOLD <= UINT16_MAX && (KEYBALL_ACCEL_MAX - 256L) * (KEYBALL_ACCEL_SPEED - KEYBALL_ACCEL_THRESHOLD) <= UINT16_MAX, "KEYBALL_ACCEL_* too large");

typedef struct {
    uint16_t speed;       // 速さの移動平均 (1 回の報告あたりの移動量の 16 倍)
    uint8_t  remainder_x; // 1 に満たない端数 (256 = 1)
    uint8_t  remainder_y;
} keyball_accel_t;

static uint16_t keyball_accel_factor(uint16_t speed) {
    if (speed >= KEYBALL_ACCEL_SPEED) {
        return KEYBALL_ACCEL_MAX;
    }
    if (speed <= KEYBALL_ACCEL_THRESHOLD) {
        return KEYBALL_ACCEL_MIN + (uint16_t)(256 - KEYBALL_ACCEL_MIN) * speed / KEYBALL_ACCEL_THRESHOLD;
    }
    return 256 + (uint16_t)(KEYBALL_ACCEL_MAX - 256) * (speed - KEYBALL_ACCEL_THRESHOLD) / (KEYBALL_ACCEL_SPEED - KEYBALL_ACCEL_THRESHOLD);
}

static int8_t keyball_accel_apply(int16_t v, uint16_t factor, uint8_t *remainder) {
    int32_t total = (int32_t)v * factor + *remainder;
    int32_t out   = total >> 8; // 端数は切り捨て、下位 8 ビットを次へ持ち越す
    *remainder    = (uint8_t)total;
    return out < -127 ? -127 : out > 127 ? 127 : (int8_t)out;
}

void keyball_on_apply_motion_to_mouse_move(keyball_motion_t *m, report_mouse_t *r, bool is_left) {
    static keyball_accel_t accel[2];
    keyball_accel_t       *a = &accel[is_left ? 1 : 0];

    // 向きは lib/keyball の既定 (Keyball39) と同じ
    int16_t x = m->y;
    int16_t y = m->x;
    if (is_left) {
        x = -x;
        y = -y;
    }
    m->x = 0;
    m->y = 0;

    // 移動量 √(x² + y²) を「大きいほう + 小さいほうの半分」で近似する (誤差 12% 以内)
    uint16_t ax       = x < 0 ? -x : x;
    uint16_t ay       = y < 0 ? -y : y;
    uint16_t distance = ax > ay ? ax + ay / 2 : ay + ax / 2;
    if (distance > 1023) {
        distance = 1023;
    }
    a->speed = (a->speed + distance * 16) / 2;

    uint16_t factor = keyball_accel_factor(a->speed);
    r->x            = keyball_accel_apply(x, factor, &a->remainder_x);
    r->y            = keyball_accel_apply(y, factor, &a->remainder_y);
}

#ifdef VIA_ENABLE

// 検査ツール (zmk-config-keyboards の tools/keyboard-check.cmd) が読む、読み取り専用の VIA コマンド。
// CPI・スクロールの倍率・AML の設定は EEPROM や起動時の処理で決まり、VIA のキーマップの読み出しでは
// 分からないため、id_custom_get_value (0x08) のチャンネル 0 (id_custom_channel) で返す。
//   08 00 01: 状態。応答の [3] 以降 (複数バイトの値はビッグエンディアン)
//     [3] 形式 (2)  [4] KEYBALL_MODEL  [5] フラグ (bit0 USB 側にボール / bit1 反対側と通信できる /
//         bit2 反対側にボール / bit3 USB 側が左 / bit4 USB 側がマスター / bit5 スクロールモード /
//         bit6 AML 有効 / bit7 AML トグル中)
//     [6] CPI (100 単位、keyball_get_cpi)  [7] EEPROM の CPI (0 = 既定)
//     [8] スクロール除数 (keyball_get_scroll_div)  [9] EEPROM のスクロール除数 (0 = 既定)
//     [10] スクロールスナップ  [11] AML のレイヤー  [12-13] AML のタイムアウト (ms)
//     [14-15] AUTO_MOUSE_DELAY (ms)  [16] AML のデバウンス (ms)  [17] スクロールレイヤー
//     [18] layer_state  [19-22] eeconfig_read_kb()  [23-26] eeconfig_read_user()
//     [27] KEYBALL_CPI_DEFAULT / 100  [28] KEYBALL_SCROLL_DIV_DEFAULT
//     [29] KEYBALL_AML_THRESHOLD (AML の発動に要る動きの量。形式 2 から)
//   08 00 02: ファームのビルド日時 (QMK_BUILDDATE、ASCII)
//   08 00 03: カーソルの加速。[3-4] KEYBALL_ACCEL_MIN_FACTOR  [5-6] MAX_FACTOR  [7-8] SPEED_THRESHOLD
//             [9-10] SPEED_MAX  [11] KEYBALL_REPORTMOUSE_INTERVAL (ms)
// 設定を変えるコマンド (08 以外、チャンネル 0 の 07 / 09) は受け付けず、id_unhandled を返す。
// 0.22.14 の via.c の注意どおり、raw_hid_send() は呼ばない (応答は via.c が送る)。

#    define KEYBALL_VIA_STATUS_FORMAT 2

static void put_be16(uint8_t *p, uint16_t v) {
    p[0] = v >> 8;
    p[1] = v & 0xFF;
}

static void put_be32(uint8_t *p, uint32_t v) {
    put_be16(&p[0], v >> 16);
    put_be16(&p[2], v & 0xFFFF);
}

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    // data = [ command_id, channel_id, value_id, value_data... ]
    if (data[0] != id_custom_get_value || data[1] != id_custom_channel || length < 32) {
        data[0] = id_unhandled;
        return;
    }
    uint8_t *v = &data[3];
    switch (data[2]) {
        case 0x01: {
            memset(v, 0, length - 3);
            uint8_t flags = 0;
            if (keyball.this_have_ball) flags |= 0x01;
            if (keyball.that_enable) flags |= 0x02;
            if (keyball.that_have_ball) flags |= 0x04;
            if (is_keyboard_left()) flags |= 0x08;
            if (is_keyboard_master()) flags |= 0x10;
            if (keyball_get_scroll_mode()) flags |= 0x20;
#    ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
            if (get_auto_mouse_enable()) flags |= 0x40;
            if (get_auto_mouse_toggle()) flags |= 0x80;
#    endif
            v[0] = KEYBALL_VIA_STATUS_FORMAT;
            v[1] = KEYBALL_MODEL;
            v[2] = flags;
            v[3] = keyball_get_cpi();
            v[4] = keyball.cpi_value;
            v[5] = keyball_get_scroll_div();
            v[6] = keyball.scroll_div;
            v[7] = keyball_get_scrollsnap_mode();
#    ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
            v[8] = get_auto_mouse_layer();
            put_be16(&v[9], get_auto_mouse_timeout());
            put_be16(&v[11], AUTO_MOUSE_DELAY);
            v[13] = get_auto_mouse_debounce();
#    endif
            v[14] = KEYBALL_SCROLL_LAYER;
            v[15] = (uint8_t)layer_state;
            put_be32(&v[16], eeconfig_read_kb());
            put_be32(&v[20], eeconfig_read_user());
            v[24] = KEYBALL_CPI_DEFAULT / 100;
            v[25] = KEYBALL_SCROLL_DIV_DEFAULT;
            v[26] = KEYBALL_AML_THRESHOLD;
            break;
        }
        case 0x02:
            memset(v, 0, length - 3);
            strncpy((char *)v, QMK_BUILDDATE, length - 4);
            break;
        case 0x03:
            // カーソルの加速 (config.h の KEYBALL_ACCEL_*)
            memset(v, 0, length - 3);
            put_be16(&v[0], KEYBALL_ACCEL_MIN_FACTOR);
            put_be16(&v[2], KEYBALL_ACCEL_MAX_FACTOR);
            put_be16(&v[4], KEYBALL_ACCEL_SPEED_THRESHOLD);
            put_be16(&v[6], KEYBALL_ACCEL_SPEED_MAX);
            v[8] = KEYBALL_REPORTMOUSE_INTERVAL;
            break;
        default:
            data[0] = id_unhandled;
            break;
    }
}

#endif

void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_ENABLE
  apply_rgblight_defaults_once();
#endif
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
  set_auto_mouse_enable(true);
  set_auto_mouse_timeout(AUTO_MOUSE_TIME);
#endif
  // スクロールを上下左右フリー方向にする（既定の縦固定を解除）
  keyball_set_scrollsnap_mode(KEYBALL_SCROLLSNAP_MODE_FREE);
}

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
