# pico-vgmplayer-AY-3-8910 <!-- omit in toc -->

<div class="separator" style="clear: both;"><a href="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEj5wVk4ZBq1Npk6DfZAWBf60YEHUTBUPjytoFB7ejAMeirbvrfiICDtk21l1BSllzCjxgtDuY457wXob6837pF5OYUFDXxS1sxbKbRXMCORYkTVvAC-Wjv7HbYKM-KS-XuaraTLChWAJiHNyh3-IvllStez81BdmSOIiKqOoqsROAOCqf_u7Pq_sHhwHAFD/s3100/DSC_0638.JPG" style="display: block; padding: 1em 0; text-align: center; "><img alt="" border="0" width="400" data-original-height="2297" data-original-width="3100" src="https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEj5wVk4ZBq1Npk6DfZAWBf60YEHUTBUPjytoFB7ejAMeirbvrfiICDtk21l1BSllzCjxgtDuY457wXob6837pF5OYUFDXxS1sxbKbRXMCORYkTVvAC-Wjv7HbYKM-KS-XuaraTLChWAJiHNyh3-IvllStez81BdmSOIiKqOoqsROAOCqf_u7Pq_sHhwHAFD/s400/DSC_0638.JPG"/></a></div>

- [概要](#概要)
- [ハードウェア側の準備](#ハードウェア側の準備)

## 概要

KUNI-NET氏の制作された偽MSX1用のKZ80-YM2149基板をRaspberry Pi pico2から制御し、microSDスロット上のVGMデータを順に演奏できるようにするためのプログラムです。

プログラムは、Arduino用に<a href="https://x.com/h1romas4">ひろましゃ</a>氏が作成されていた<a href="https://github.com/h1romas4/arduino-vgmplayer">arduino-vgmplayer</a>をベースにRaspberry Pi pico対応＋SDカード読み出し対応、AY-3-8910対応など手を加えて、完成させました。

microSDから直接VGMデータを読み出しながらAY-3-8910に信号を送るという性能的にキツそうな作りになっているのですが、Raspberry pi pico1ですと、若干演奏がもたつく感がありまして、Raspberry pi pico2だと大丈夫そうだったので、お試しになる際は、Raspberry pi pico2をご準備されるのがおすすめです。

VGMデータはmicroSDのルートディレクトリに入っているファイルを順に演奏していきます。ただし、VGMデータのファイルサイズが大きいと演奏がスキップされてしまうことがあるようです。これについては調査中です。

また、DFPlayer miniのUART I/Fを一部取り入れ、再生、一時停止、次曲、前曲の操作をUART経由で行うことが可能です。（今回の試作では基板上にはUART用のピンは実装していません）

## ハードウェア側の準備

1. Raspberry Pi pico2側の基板

Raspberry Pi pico2とmicroSDスロットをPSG/JOYSTICK/CMT基板に接続するための基板を実装します。

中の人は回路図がまだかけないため、写真とピン配置でご勘弁ください。

KZ80-YM2149ボード側のSBC8080 BUSのピン配置については、KZ80-YM2149のGitHubに回路図がありますので、そちらを参考にしてください。


| Pico2 | microSD | SBC8080 BUS | AY-3-8910 |
| ----- | ------- | ----------- | --------- |
| GP00  | -       | D0(9)       | D0        |
| GP01  | -       | D1(10)      | D1        |
| GP02  | -       | D2(11)      | D2        |
| GP03  | -       | D3(12)      | D3        |
| GP04  | -       | D4(13)      | D4        |
| GP05  | -       | D5(14)      | D5        |
| GP06  | -       | D6(15)      | D6        |
| GP07  | -       | D7(16)      | D7        |
| GP08  | -       | A0(25)      | -         |
| GP09  | -       | ~IOWR(19)   | BDIR      |
| GP10  | -       | RES(21)     | ~RESET    |
| GP11  | -       | ~IORD(17)   | BC1       |
| ...   | ...     | ...         | ...       |
| GP16  | MISO    | -           | -         |
| GP17  | CS      | -           | -         |
| GP18  | SCK     | -           | -         |
| GP19  | MOSI    | -           | -         |
| ...   | ...     | ...         | ...       |
| VSYS  | 5V      | VCC(1 or 3) | VCC       |
| GND   | GND     | GND(2 or 4) | GND       |
| GND   | -       | A3(28)      | -         |
| GND   | -       | A4(29)      | -         |
| GND   | -       | A5(30)      | -         |
| GND   | -       | A6(31)      | -         |
| GND   | -       | A7(32)      | -         |


2. AY-3-8910シールド

KUNI-NET氏の[KZ80-YM2149](https://github.com/kuninet/KZ80-YM2149/wiki/REV2) を使用しています。

この基板はオレンジピコショップさんで取り扱いがあります。

[MSXを作ろう(4)-KZ80シリーズ用PSG音源ボード(KZ80-YM2149)専用プリント基板(オレンジピコショップ)](https://store.shopping.yahoo.co.jp/orangepicoshop/pico-a-083.html)
 
以上
