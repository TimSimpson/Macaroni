Macaroni for C++
================

[![Build status](https://ci.appveyor.com/api/projects/status/q3ph7cjv7r3xw6j1?svg=true)](https://ci.appveyor.com/project/TimSimpson/macaroni)

<img align="right" src="http://border-town.com/macaroni/images/Maccy.png" />

Macaroni for C++ aims to make writing C++ code quick and fun.

C++ isn't difficult, yet writing it is often a hassle thanks to the 1980's way the language is processed and the macro boilerplate required to properly create portable, flexible library code (i.e. Windows with it's delightful need for things like <a href="http://www.codeproject.com/Articles/6351/Regular-DLL-Tutor-For-Beginners">__declspec(dllimport)</a>).

Macaroni gets rid of all that and allows you to organize code in a sleeker format which skips the header files (and other bits of preprocessor sadness) to keep things <a href="http://en.wikipedia.org/wiki/Don%27t_repeat_yourself">DRY</a>.


### Features ###

* Keeps you from writing header files and other bits of boilerplate.
* Automatically writes forward declarations where appropriate to reduce compile times.
* Simplified usage of namespaces and imports.
* Works with Boost to handle library config issues, making it easy to do things like build Windows DLLs.
* Allows metadata to live next to code.
* Generates glue code.
* Generates build scripts.
* Extensible via Lua plugins.
* Compilers and even debuggers map most issues in generated code back to Macaroni code thanks to usage of the #line directive.
* Easily gets out of the way when you need it to.


### Learn more ###
Learn more and download a copy at it's [website](http://border-town.com/macaroni).
