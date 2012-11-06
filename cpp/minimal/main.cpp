
#include <string>
#include <iostream>

#include <truetypetracer/ttt.hpp>
#include <truetypetracer/ngc_writer.hpp>

#define TTFONT "/usr/share/fonts/truetype/freefont/FreeSerif.ttf"

// ttt example program
int main() {
    NGC_Writer my_writer; // this Writer writes G-code
    Ttt t( &my_writer, "Test", false , TTFONT ); // ( Writer*, text, unicode?, path-to-font )
    
    std::string out = t.get_output();
    std::cout << out;

    return 0;
}
