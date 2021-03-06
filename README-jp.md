### ScreenXを使えばあなたのターミナル画面を生中継できます。
### 勉強会の配信、講義、ペアプログラミングなどのシーンでご活用ください。


[![Screenshot](https://raw.github.com/screenxtv/screenxtv-gcc-client/master/images/ss-screenxtv.png)](http://screenx.tv/)


## 必要なソフトウェア

- GCC コンパイラ
  - Mac OS X: [install via Xcode](http://stackoverflow.com/questions/9353444/how-to-use-install-gcc-on-mac-os-x-10-8-xcode-4-4)
  - Linux: `sudo aptitude install g++`
  - Windows: [Use Cygwin](http://www.eecg.utoronto.ca/~aamodt/ece242/cygwin.html)
- [Screen](http://www.gnu.org/software/screen/) コマンド
  - Mac OS X: `brew install screen`
  - Linux: `sudo aptitude install screen`
  - Windows: Download and install [this](http://directory.fsf.org/wiki/Screen) via Cygwin.

## 配信の仕方

1. 配信ツールのソースファイルを GitHub からダウンロードします。

   `git clone https://github.com/screenxtv/screenxtv-gcc-client.git`
   
2. ダウンロードしたソースファイルへ移動し、コンパイルします。

   `cd screenxtv-gcc-client`

   `make`
   
3. 配信ツールを起動します。

   `./screenxtv-gcc-client`

4. 必要な項目（配信URL、タイトルなど）を入力して、Enter を押します。

   ![ss-start](https://raw.github.com/screenxtv/screenxtv-gcc-client/master/images/ss-start.png)
   
5. 配信完了！配信URLからターミナル画面が配信されるので、URLを友人などにシェアしてみてください :-)


## 配信の停止／再開

1. ターミナル画面から exit することで、配信を停止することができます。

   `exit`

   NOTE: ScreenX TV は、 `exit` してから最長 __10分間__ セッションを保持します。

   ![ss-stop](https://raw.github.com/screenxtv/screenxtv-gcc-client/master/images/ss-stop.png)
   
2. 配信停止の状態から、再び配信中のターミナル画面にアタッチしたい場合は、再度配信ツールを起動して下さい。

   `./screenxtv-gcc-client`

   配信ツールを起動すると、配信中のターミナル画面に自動的にアタッチしてくれます。
   
   ![ss-resume](https://raw.github.com/screenxtv/screenxtv-gcc-client/master/images/ss-resume.png)   

## 関連レポジトリ

- [ScreenX TV](http://screenx.tv/)
- [ScreenX TV GCC Client](https://github.com/screenxtv/screenxtv-gcc-client)
- [ScreenX TV Ruby Client](https://github.com/screenxtv/screenxtv-ruby-client)
- [ScreenX TV Sandbox](https://github.com/screenxtv/screenxtv-sandbox)
- [ScreenX TV News](https://github.com/screenxtv/screenxtv-news)
- [ScreenX](https://github.com/screenxtv/screenx) (Java-based stand-alone Software)

## ライセンス

(The MIT License)

Copyright (c) 2012 Tomoya Ishida

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
