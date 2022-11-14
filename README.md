![Build-badge](https://github.com/aewallin/truetype-tracer/actions/workflows/cmake-build.yml/badge.svg)

True-type tracer
================


Experimental C++ port, with python bindings, of True-type tracer, 
forked from Chris Radek's site at http://timeguy.com/cradek/01276453959
Original git repo at git://timeguy.com/truetype-tracer.git

Uses http://www.freetype.org/index2.html[FreeType] to produce font-outline geometry in various output formats such as: 

- G-code
- DXF (broken, see To-Do list)

Dependencies: git, cmake, FreeType, boost-python.


Installation from source

    $ git clone https://aewallin@github.com/aewallin/truetype-tracer.git
    $ cd truetype-tracer
    $ mkdir build
    $ cd build
    $ cmake ../src
    $ make
    $ sudo make install

See Also
http://ttfquery.sourceforge.net/

To-Do

* Make it possible to change the font.
* Auto-scale output to be of certain width/height.
* Clean up the interface and API (now it's not very clear how to use ttt...)
* DXF_Writer: Make DXF output work.
* SVG output? (write a new class SVG_Writer)
* NGC_Writer:
** make variables optional, for controllers that don't have LinuxCNC-like variables
** make number of digits afer decimal point adjustable
* Use Kerning to avoid overlapping characters. See http://www.freetype.org/freetype2/docs/tutorial/step2.html
