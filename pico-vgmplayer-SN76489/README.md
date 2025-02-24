# pico-vgmplayer-SN76489 <!-- omit in toc -->

- [概要](#概要)
- [ハードウェア側の準備](#ハードウェア側の準備)
- [ピン配置](#ピン配置)

## 概要

SN76489をArduinoシールドとして実装した基板（SN76489シールド）をRaspberry Pi picoから制御し、microSDスロット上のVGMデータを順に演奏できるようにするためのプログラムです。

プログラムは、Arduino用に<a href="https://x.com/h1romas4">ひろましゃ</a>氏が作成されていた<a href="https://github.com/h1romas4/arduino-vgmplayer">arduino-vgmplayer</a>をベースにRaspberry Pi pico対応＋SDカード読み出し対応、SN76489対応など手を加えて、完成させました。

とりあえずで、microSDから直接読み出しながらSN76489に信号を送るというなかなか性能的にキツそうな作りにしてみたんですが、ほぼほぼ遅延なく演奏できてしまったので、もうこれでいいか、ということにしましたw

VGMデータはmicroSDのルートディレクトリに入っているファイルを順に演奏していきます。ただし、VGMデータのファイルサイズが大きいと演奏がスキップされてしまうことがあるようです。これについては調査中です。

また、DFPlayer miniのUART I/Fを一部取り入れ、再生、一時停止、次曲、前曲の操作をUART経由で行うことが可能です。

## ハードウェア側の準備

1. Raspberry Pi picoをArduinoピン配列に変換するシールド

Raspberry Pi picoのピン配置をArduinoシールドに変換できなくてはいけませんので、まずそのシールドが必要です。

中の人は、これを試作したときたまたまArduinoシールド基板もピンヘッダも切らしていたため、適当なユニバーサル基板とピンヘッダ、ピンソケットで無理やり作ってしまいました。そのうちちゃんとAruduinoシールドで作り直しますので、写真はそのときにアップします。

2. microSDカードスロット用のシールド

中の人は、microSDカードのシールドは、以前Arduino用に用意していた自作シールドがあったのですが、今回Raspberry Pi pico用にピン配置を変更して再利用しました。

<div class="separator" style="clear: both;"><a href="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEgI9jBKjqcCUkefDVpTyBXSYf3TpkA_BqEtAgNErVx4lmKHX5n6LndAJCyDpJg7bCFkghIYt1HzhYCnzrXArgjWBzu3CeNi71Md0atWXcbxSEo5P-KkP7ax1d0ZRxfs3bOi9Z08E6miF65Gtfjd9IHVdZnPFn60RgsqaXneJRKot8OZK8F6sowd4YOIocZI/s2305/IMG_2187.jpg" style="display: block; padding: 1em 0; text-align: center; "><img alt="" border="0" width="400" data-original-height="1485" data-original-width="2305" src="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEgI9jBKjqcCUkefDVpTyBXSYf3TpkA_BqEtAgNErVx4lmKHX5n6LndAJCyDpJg7bCFkghIYt1HzhYCnzrXArgjWBzu3CeNi71Md0atWXcbxSEo5P-KkP7ax1d0ZRxfs3bOi9Z08E6miF65Gtfjd9IHVdZnPFn60RgsqaXneJRKot8OZK8F6sowd4YOIocZI/s400/IMG_2187.jpg"/></a></div>

3. SN76489シールド

中の人がまだ回路図がかけないので、すみません・・・今のところ写真だけです。

<div class="separator" style="clear: both;"><a href="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEgssUjn0LvA1p39rH9cF5jStT-E25CH0bMX5XPL2r9KBO31FnL7LUfsBgqcUi_4qIbS1TqJ5FArMP_TuAJ4mN9C68QxMYhDv2PTizj2sRONdyHMhxFRxpmeJid4ryiu03VrHQwXhlvd0QPesW6zZVUk0Zk11NeRbaX4kWlMyDD8ulbBYHBA3Z7kekqvtEUf/s2524/DSC_0775-EDIT.jpg" style="display: block; padding: 1em 0; text-align: center; "><img alt="" border="0" width="400" data-original-height="2207" data-original-width="2524" src="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEgssUjn0LvA1p39rH9cF5jStT-E25CH0bMX5XPL2r9KBO31FnL7LUfsBgqcUi_4qIbS1TqJ5FArMP_TuAJ4mN9C68QxMYhDv2PTizj2sRONdyHMhxFRxpmeJid4ryiu03VrHQwXhlvd0QPesW6zZVUk0Zk11NeRbaX4kWlMyDD8ulbBYHBA3Z7kekqvtEUf/s400/DSC_0775-EDIT.jpg"/></a></div>


## ピン配置

ピン配置は以下のようになっています。
※SN76489の()内はピン番号です

| Pico | Arduino | SN76489 | microSD |
| ---- | ------- | ------- | ------- |
| GP00 | D2      | D7(10)  | -       |
| GP01 | D3      | D6(11)  | -       |
| GP02 | D4      | D5(12)  | -       |
| GP03 | D5      | D4(13)  | -       |
| GP04 | D6      | D3(15)  | -       |
| GP05 | D7      | D2(1)   | -       |
| GP06 | D8      | D1(2)   | -       |
| GP07 | D9      | D0(3)   | -       |
| GP08 | D10     | WE(5)   | -       |
| ...  | ...     | ...     | ...     |
| GP16 | A3      | -       | MISO    |
| GP17 | A0      | -       | CS      |
| GP18 | A1      | -       | SCK     |
| GP19 | A2      | -       | MOSI    |
| ...  | ...     | ...     | ...     |
| VSYS | 5V      | VCC(16) | VCC     |
| GND  | GND     | GND(8)  | GND     |
 
以上
