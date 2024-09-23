# pico-vgmplayer <!-- omit in toc -->

<div class="separator" style="clear: both;">
<a href="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEhbmsX2J3pXsSzcKEfjSspUDzI9JXmOQKXJ2lfm7ZgAFvSTKTVKtvRGjJxAEI7kze8kNH2_f5zHMmXs9TEhIM4gQIaXckjaLKZuvWn-UspZXd3G15OgIjrQ3kOluPD1f5dZapslawYufR8q1vVaaBejxsbWK_CGXucCyz3F9yNB867YdI0d86NGP3IK78qn/s3474/IMG_20240908_012131_1.jpg" style="display: block; padding: 1em 0; text-align: center; "><img alt="" border="0" height="400" data-original-height="3474" data-original-width="3336" src="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEhbmsX2J3pXsSzcKEfjSspUDzI9JXmOQKXJ2lfm7ZgAFvSTKTVKtvRGjJxAEI7kze8kNH2_f5zHMmXs9TEhIM4gQIaXckjaLKZuvWn-UspZXd3G15OgIjrQ3kOluPD1f5dZapslawYufR8q1vVaaBejxsbWK_CGXucCyz3F9yNB867YdI0d86NGP3IK78qn/s400/IMG_20240908_012131_1.jpg"/></a></div>

- [概要](#概要)
- [ハードウェア側の準備](#ハードウェア側の準備)
- [ピン配置](#ピン配置)
- [使用例](#使用例)

## 概要

ooISHoo氏の[YM2151 Shield](https://github.com/ooISHoo/Arduino_YM2151) をRaspberry Pi picoを使い、microSDスロット上のVGMデータを順に演奏できるようにするためのプログラムです。

プログラムは、Arduino用に<a href="https://x.com/h1romas4">ひろましゃ</a>氏が作成されていた<a href="https://github.com/h1romas4/arduino-vgmplayer">arduino-vgmplayer</a>をベースにRaspberry Pi pico対応＋SDカード読み出し対応など手を加えて、完成させました。

とりあえずで、microSDから直接読み出しながらYM2151に信号を送るというなかなか性能的にキツそうな作りにしてみたんですが、ほぼほぼ遅延なく演奏できてしまったので、もうこれでいいか、ということにしましたw

VGMデータはmicroSDのルートディレクトリに入っているファイルを順に演奏していきます。ただし、VGMデータのファイルサイズが大きいと演奏がスキップされてしまうことがあるようです。これについては調査中です。

また、DFPlayer miniのUART I/Fを一部取り入れ、再生、一時停止、次曲、前曲の操作をUART経由で行うことが可能です。

## ハードウェア側の準備

1. Raspberry Pi picoをArduinoピン配列に変換するシールド

Raspberry Pi picoのピン配置をArduinoシールドに変換できなくてはいけませんので、まずそのシールドが必要です。
中の人はジャンパピンでつなげるように作ってみました。

<div class="separator" style="clear: both;"><a href="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEj5yPZMalBtGE4qh9ODEayuhFZbtT8TE-OtnD9nbMWezZLm9r8Q2TY1OPihCWOfLPJHC9ol40YJYmHVPLF4JZQ1M6jjHxDQmBspcUmnKWbDKqTaHDyzziAE64QwVqnuObq8RpiRrCt53ZhXMhjzzk2I5gcp-pGZC3TlUifFY3OM68d0B5XRVB0rStt_NPMq/s2166/IMG_2185.jpg" style="display: block; padding: 1em 0; text-align: center; "><img alt="" border="0" height="400" data-original-height="2166" data-original-width="1926" src="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEj5yPZMalBtGE4qh9ODEayuhFZbtT8TE-OtnD9nbMWezZLm9r8Q2TY1OPihCWOfLPJHC9ol40YJYmHVPLF4JZQ1M6jjHxDQmBspcUmnKWbDKqTaHDyzziAE64QwVqnuObq8RpiRrCt53ZhXMhjzzk2I5gcp-pGZC3TlUifFY3OM68d0B5XRVB0rStt_NPMq/s400/IMG_2185.jpg"/></a></div>
  
2. microSDカードスロット用のシールド

中の人は、microSDカードのシールドは、以前Arduino用に用意していた自作シールドがあったのですが、今回Raspberry Pi pico用にピン配置を変更して再利用しました。

<div class="separator" style="clear: both;"><a href="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEgI9jBKjqcCUkefDVpTyBXSYf3TpkA_BqEtAgNErVx4lmKHX5n6LndAJCyDpJg7bCFkghIYt1HzhYCnzrXArgjWBzu3CeNi71Md0atWXcbxSEo5P-KkP7ax1d0ZRxfs3bOi9Z08E6miF65Gtfjd9IHVdZnPFn60RgsqaXneJRKot8OZK8F6sowd4YOIocZI/s2305/IMG_2187.jpg" style="display: block; padding: 1em 0; text-align: center; "><img alt="" border="0" width="400" data-original-height="1485" data-original-width="2305" src="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEgI9jBKjqcCUkefDVpTyBXSYf3TpkA_BqEtAgNErVx4lmKHX5n6LndAJCyDpJg7bCFkghIYt1HzhYCnzrXArgjWBzu3CeNi71Md0atWXcbxSEo5P-KkP7ax1d0ZRxfs3bOi9Z08E6miF65Gtfjd9IHVdZnPFn60RgsqaXneJRKot8OZK8F6sowd4YOIocZI/s400/IMG_2187.jpg"/></a></div>

3. YM2151シールド

ooISHoo氏の[YM2151 Shield](https://github.com/ooISHoo/Arduino_YM2151) にある回路図をもとに作りました。

もし、さすがにYM2151シールドをゼロからユニバーサル基板で配線はしんどい、という方向けにはオレンジピコショップさんで基板の取り扱いあります。

[Z80 FM音源 YM2151 ボード (KZ80_YM2151) 専用プリント基板(オレンジピコショップ)](https://store.shopping.yahoo.co.jp/orangepicoshop/pico-a-013.html)

ただし、中の人はまだこの基板で組んだことがないので、ご利用は自己責任でお願いします。

## ピン配置

ピン配置は以下のようになっています。

| Pico | Arduino | YM2151 | microSD |
| ---- | ------- | ------ | ------- |
| GP02 | D2      | D0     | -       |
| GP03 | D3      | D1     | -       |
| GP04 | D4      | D2     | -       |
| GP05 | D5      | D3     | -       |
| GP06 | D6      | D4     | -       |
| GP07 | D7      | D5     | -       |
| GP08 | D8      | D6     | -       |
| GP09 | D9      | D7     | -       |
| GP10 | D10     | RD     | -       |
| GP11 | D11     | WR     | -       |
| GP12 | D12     | A0     | -       |
| GP13 | D13     | IC     | -       |
| GP16 | A3      | -      | MISO    |
| GP17 | A0      | -      | CS      |
| GP18 | A1      | -      | SCK     |
| GP19 | A2      | -      | MOSI    |
| VSYS | 5V      | VCC    | VCC     |
| GND  | GND     | GND    | GND     |

## 使用例

ダイソーの300円スピーカーをつなげると、なんともいえない味のあるジュークボックスが出来上がります。

<div class="separator" style="clear: both;"><a href="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEjgzGs73mUqoK_PlTc8ZLtDuWv3-uOm-tCez4juKkcFURXPSwfh-NEVQ1WanE509Yh7m12GdHvbcPRIDDWjtZ1lIKXNwWZtCFivq5S5jClQnQ15m_vYG5OIfuBCt-gqQ0Mlvv2f6j3n5Wn8wAj9JcCiaisLs9uYY5vsI9YavrSheIRXAQ0x68-wflURvBj9/s2228/IMG_2119.jpg" style="display: block; padding: 1em 0; text-align: center; "><img alt="" border="0" width="400" data-original-height="1484" data-original-width="2228" src="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEjgzGs73mUqoK_PlTc8ZLtDuWv3-uOm-tCez4juKkcFURXPSwfh-NEVQ1WanE509Yh7m12GdHvbcPRIDDWjtZ1lIKXNwWZtCFivq5S5jClQnQ15m_vYG5OIfuBCt-gqQ0Mlvv2f6j3n5Wn8wAj9JcCiaisLs9uYY5vsI9YavrSheIRXAQ0x68-wflURvBj9/s400/IMG_2119.jpg"/></a></div>
  
以上
