ScreenXを使えばあなたのターミナル画面を生中継できます。
勉強会の配信、授業／講義、ペアプログラミングなどに。

[![Screenshot](https://raw.github.com/tompng/screenxtv-client/master/ss-screenxtv.png)](http://screenx.tv)

## 必要なソフトウェア

- GCC コンパイラ
  - Mac OS X: [see this](http://stackoverflow.com/questions/9353444/how-to-use-install-gcc-on-mac-os-x-10-8-xcode-4-4)
  - Linux: `sudo aptitude install g++`
  - Windows: [Use Cygwin](http://www.eecg.utoronto.ca/~aamodt/ece242/cygwin.html)
- [Screen command](http://www.gnu.org/software/screen/)
  - Mac OS X: `brew install screen`
  - Linux: `sudo aptitude install screen`
  - Windows: Download and install [this](http://directory.fsf.org/wiki/Screen) via Cygwin.

## 配信の仕方

1. 配信ツールのソースファイルを GitHub からダウンロードします。

   `git clone https://github.com/tompng/screenxtv-client.git`
   
2. ダウンロードしたソースファイルへ移動し、コンパイルします。

   `cd screenxtv-client`

   `g++ -o screenxtv-client main.cc -lpthread -lutil`
   
3. 配信ツールを起動します。

   `./screenxtv-client`

4. 必要な項目（配信URL、タイトルなど）を入力して、Enter を押します。

5. 配信完了！配信URLからターミナル画面が配信されるので、URLを友人などにシェアしてみてください :-)


## 配信の停止／再生

1. ターミナル画面から exit することで、配信を停止することができます。

   `exit`

   NOTE: ScreenX TV は `exit` してから、最長 __10分間__ セッションを保持します。

2. 配信停止の状態から、再び配信中のターミナル画面にアタッチしたい場合は、再度配信ツールを起動して下さい。

   `./screenxtv-client`.

   配信ツールを起動すると、配信中のターミナル画面に自動的にアタッチしてくれます。

## License

Copyright (c) 2012 Tomoya Ishida

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
