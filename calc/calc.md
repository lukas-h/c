# calc
only some experiments to create a calculator for the console, works with mac, linux and other unix
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)  
Built with tinyexpr (https://github.com/codeplea/tinyexpr)  

```
 +-------------+
 | quick intro |
 +-------------+
 numbers are just normal: 45, 3.14159, 100000, ...

 operators: +, -, *, /, % (modulo), ...
	-> for example: 4 * 4, 13 + (4*5)

 functions: sqrt, sin, cos, floor, ...
	-> use this way: sinh(4) or without brackets: log 4.6543

 constants: pi, e (euler)

 environment variables: r, t, c
	-> r represents the result of the last calculation.
	-> t is the total of all results in the session.
	-> c is the count of evaluations.

 environment commands: help, exit, q, clear

	-> help this page (WOW)
	-> exit or q has the same effect as ^C - It quits the application!
	-> clear purge console screen

 That's it basically ...

 +-------------+
 | information |
 +-------------+
 https://github.com/lukas-h/calc
 License MIT

 Built with tinyexpr (https://github.com/codeplea/tinyexpr)
 - there you'll find more documentation.
```

### build it:

1. make sure a C compiler and *make* is available  
2. run `make` to build it  
3. with `sudo make install` you install it system-wide
