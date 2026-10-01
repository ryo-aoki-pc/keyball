# Keyball39

![Keyball39](../../../../keyball39/doc/rev1/images/kb39_001.jpg)

A split keyboard with 39 vertically staggered keys and 34mm track ball.

* Keyboard Maintainer: [@Yowkees](https://twitter.com/Yowkees)
* Hardware Supported: Keyball39 PCB, ProMicro
* Hardware Availability:
    * <https://shop.yushakobo.jp/products/5357>
    * <https://shirogane-lab.net/items/64b8f8693ee3fd0045280190>

Make example for this keyboard (after setting up your build environment):

    make keyball/keyball39:default

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Special keycodes

See [Special Keycode](../lib/keyball/keycodes.md) file.

## Read-only VIA command for the settings / 設定を読み出す VIA コマンド (keymaps/via)

`keymaps/via` は、CPI・スクロールの倍率・AML (自動マウスレイヤー) の設定を読み出す、読み取り専用の
VIA コマンドを持つ。これらは EEPROM と起動時の処理で決まり、VIA のキーマップの読み出しでは分からない
(EEPROM の CPI やスクロール除数は、ファームを書き直しても残る)。
[zmk-config-keyboards](https://github.com/ryo-aoki-pc/zmk-config-keyboards) の `tools/keyboard-check.cmd` が、
意図した値 (CPI 500 / スクロール 1/16 / AML 10 秒など) になっているかの検査に使う。

| 要求 | 応答 |
| --- | --- |
| `08 00 01` | 状態。`[3]` 形式 (1)、`[4]` 機種、`[5]` フラグ、`[6]` CPI (100 単位)、`[7]` EEPROM の CPI、`[8]` スクロール除数、`[9]` EEPROM のスクロール除数、`[10]` スクロールスナップ、`[11]` AML のレイヤー、`[12-13]` AML のタイムアウト (ms)、`[14-15]` `AUTO_MOUSE_DELAY`、`[16]` AML のデバウンス、`[17]` スクロールレイヤー、`[18]` `layer_state`、`[19-22]` `eeconfig_read_kb()`、`[23-26]` `eeconfig_read_user()`、`[27]` `KEYBALL_CPI_DEFAULT / 100`、`[28]` `KEYBALL_SCROLL_DIV_DEFAULT` |
| `08 00 02` | ファームのビルド日時 (`QMK_BUILDDATE`、ASCII) |

- 要求と応答は VIA の raw HID (32 バイト)。複数バイトの値はビッグエンディアン
- フラグ: bit0 USB 側にボール、bit1 反対側と通信できる、bit2 反対側にボール、bit3 USB 側が左、
  bit4 USB 側がマスター、bit5 スクロールモード、bit6 AML 有効、bit7 AML トグル中
- 値を変えるコマンドは無い (チャンネル 0 のほかの要求には `id_unhandled` (0xFF) を返す)。
  このコマンドの無い古いファームも 0xFF を返す
