
#include <string>
#include <iostream>

#include <boost/foreach.hpp>

#include <ttt/ttt.hpp>
#include <ttt/segment_writer.hpp>

#define TTFONT "/usr/share/fonts/truetype/freefont/FreeSerif.ttf"

// ttt example program
int main() {
    SEG_Writer my_writer; // this Writer writes G-code
    Ttt t( &my_writer, "Test", false , TTFONT ); // ( Writer*, text, unicode?, path-to-font )
    
    Loops all_loops = my_writer.get_loops();
    
    // print out the points
    int nloop =0;
    BOOST_FOREACH(Loop l, all_loops) {
        std::cout << "Loop " << nloop << "\n";
        int npoint=0;
        BOOST_FOREACH(Point pt, l) {
            std::cout << " Point " << npoint << " x= " << pt.first << " y= " << pt.second << "\n";
            npoint++;
        }
        std::cout << "End Loop " << nloop << "\n";
        nloop++;
    } 
    
    //std::cout << out;

    return 0;
}
