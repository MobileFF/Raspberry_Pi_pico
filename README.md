# Raspberry_Pi_pico

Raspberry Pi pico用のサンプルプログラムです。

## pico-vgmplayer-YM2151

[ひろましゃ](https://x.com/h1romas4)氏が作成したYM2151演奏アプリケーションである[arduino-vgmplayer](https://github.com/h1romas4/arduino-vgmplayer)をベースに、Arduino化したRaspberry Pi picoでVGMデータを演奏できるようにしたものです。VGMデータはmicroSDのルートディレクトリに入っているファイルを順に演奏していきます。また、DFPlayer miniのUART I/Fを一部取り入れ、再生、一時停止、次曲、前曲の操作をUART経由で行うことが可能です。

(2025/2/24) YM2151以外のVGMデータ演奏アプリケーションを作成したことに伴い、このアプリケーションの名称を「pico-vgmplayer-YM2151」に変更しました。

## pico-vgmplayer-SN76489

[ひろましゃ](https://x.com/h1romas4)氏が作成した[arduino-vgmplayer](https://github.com/h1romas4/arduino-vgmplayer)をベースに、Arduino化したRaspberry Pi picoでSN76489を使ってVGMデータを演奏できるようにしたものです。VGMデータはmicroSDのルートディレクトリに入っているファイルを順に演奏していきます。また、DFPlayer miniのUART I/Fを一部取り入れ、再生、一時停止、次曲、前曲の操作をUART経由で行うことが可能です。

## pico-vgmplayer-AY-3-8910

[ひろましゃ](https://x.com/h1romas4)氏が作成した[arduino-vgmplayer](https://github.com/h1romas4/arduino-vgmplayer)をベースに、Arduino化したRaspberry Pi picoでAY-3-8910を使ってVGMデータを演奏できるようにしたものです。VGMデータはmicroSDのルートディレクトリに入っているファイルを順に演奏していきます。また、DFPlayer miniのUART I/Fを一部取り入れ、再生、一時停止、次曲、前曲の操作をUART経由で行うことが可能です。
