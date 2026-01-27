# WCout (C++, Windows)

WCout is a simple, consistent output and dialog utility for C++ on Windows,
designed for Embarcadero C++Builder (VCL).

It provides a single, readable chaining syntax to format text and numbers
and send output to message dialogs, the clipboard, memos, or strings.

## Example

```cpp
WCout << "Pi = " << FF-7.2 << PI << SHOW;
WCout << LongMessage >> CLIPBOARD;
