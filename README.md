2048 Engine + AI
================

## Synopsis

This is a lightweight 2048 Engine and AI written in C. Boards are stored as 64-bit integers, with 4 bits for each spot. Moves are found using a transposition table, and rows/columns are extracted using bit manipulation. The AI is an expectimax AI, and the board evaluation function counts the number of open spaces and if the pieces on the bottom row are in order.

## Code Example

```bash
gcc 2048.c -o 2048
./2048
```

## Installation

git clone 

## API Reference



## License

The MIT License (MIT)

Copyright (c) 2015 Shane Barratt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
