ScreenX TV Client is software that enables you to broadcast your terminal to the web. 
The broadcasted terminal is displayed at [ScreenX TV](http://screenx.tv) in real time.

[![Screenshot](https://raw.github.com/tompng/screenxtv-gcc-client/master/images/ss-screenxtv.png)](http://screenx.tv)

## Requirements

- GCC Compiler
  - Mac OS X: [see this](http://stackoverflow.com/questions/9353444/how-to-use-install-gcc-on-mac-os-x-10-8-xcode-4-4)
  - Linux: `sudo aptitude install g++`
  - Windows: [Use Cygwin](http://www.eecg.utoronto.ca/~aamodt/ece242/cygwin.html)
- [Screen command](http://www.gnu.org/software/screen/)
  - Mac OS X: `brew install screen`
  - Linux: `sudo aptitude install screen`
  - Windows: Download and install [this](http://directory.fsf.org/wiki/Screen) via Cygwin.

## How to Start

1. Download source of ScreenX TV Client.

   `git clone https://github.com/tompng/screenxtv-gcc-client.git`
   
2. Compile the source.

   `cd screenxtv-gcc-client`

   `g++ -o screenxtv-gcc-client main.cc -lpthread -lutil`
   
3. Run the ScreenX TV Client.

   `./screenxtv-gcc-client`

4. Then, fill in the given items to start broadcasting your terminal.

   ![ss-start](https://raw.github.com/tompng/screenxtv-gcc-client/master/images/ss-start.png)

5. Done! Open the given URL with your web browser and share it with your friends :)


## How to Stop/Resume

1. To stop broadcasting, just exit from the screen.

   `exit`

   NOTE: ScreenX TV holds your session for __10 minutes__ after the `exit`. 
   
   ![ss-stop](https://raw.github.com/tompng/screenxtv-gcc-client/master/images/ss-stop.png)

2. If you'd like to resume, just re-run the client program.

   `./screenxtv-gcc-client`.

   Then, the program automatically detects and attaches the session to resume.
   
   ![ss-resume](https://raw.github.com/tompng/screenxtv-gcc-client/master/images/ss-resume.png)

## Related Works

- [ScreenX TV Sandbox](https://github.com/yasulab/screenxtv-sandbox)
- [ScreenX](https://github.com/tompng/screenx) (internal software)

## License

Copyright (c) 2012 Tomoya Ishida

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
