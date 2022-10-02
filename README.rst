====
inem
====
**Note:** This library is licensed under both GPLv2 and commercial license
terms.

The inem library is the core math library used by the *Aion* application.  The
library depends on a matrix function library conforming to the API defined by
the `inemat <https://github.com/inesonic/inemat>` header only library.  To use
this library, you will need to supply your own matrix function library.

This library uses the qmake build system but is designed not to depend on any
libraries or headers provided as part of the Qt framework.  The library does
depend on the Boost libraries.

The provided test framework is an exception that does depend on the Qt
libraries as well as several other libraries produced by Inesonic.

For details on *Aion*, see our website at https://inesonic.com .
