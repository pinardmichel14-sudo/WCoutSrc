# WCout (C++, Windows)

WCout is a simple, consistent output and dialog utility for C++ on Windows,
designed for **Embarcadero C++Builder (VCL)**.

It provides a single, readable chaining syntax to format text and numbers
and send output to message dialogs, the clipboard, memos, or strings —
all using the same model.

---

## Example

```cpp
WCout << "Pi = " << FF-7.2 << PI << SHOW;
WCout << LongMessage >> CLIPBOARD;
```




Key ideas

One consistent << / >> chaining syntax everywhere

Formatting, dialogs, and output targets use the same model

Display text, numbers, and user-defined types

No external libraries

Drop-in .cpp / .h files

## Quick start

Add WCout.cpp to your project

Include WCout.h where you want to use WCout

Build — no external libraries or complex setup required

That’s it.

Platform / compiler

WCout is designed for Embarcadero C++Builder (VCL, Windows).

If you are using a different compiler or framework, WCout will emit a clear
compile-time error indicating that VCL-specific headers must be replaced
with equivalents for your environment.

This makes porting explicit and intentional.

Documentation

A full illustrated user manual is available as a PDF:

[WCout User Manual (PDF)](docs/WCout_User_Manual_v1.pdf)


The manual shows:

- Real examples
- Matching source and results
- Formatting behavior


Dialog usage

Clipboard and memo output

Status

WCout is stable and actively documented.
The focus is clarity, consistency, and minimal friction for everyday debugging
and output.


