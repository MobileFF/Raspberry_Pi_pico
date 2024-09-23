# Raspberry_Pi_pico

Raspberry Pi pico用のサンプルプログラムです。

## pico-vgmplayer

[ひろましゃ](https://x.com/h1romas4)氏が作成した[arduino-vgmplayer](https://github.com/h1romas4/arduino-vgmplayer)をベースに、Arduino化したRaspberry Pi picoで動作するようにしたものです。VGMデータはmicroSDのルートディレクトリに入っているファイルを順に演奏していきます。また、DFPlayer miniのUART I/Fを一部取り入れ、再生、一時停止、次曲、前曲の操作をUART経由で行うことが可能です。

