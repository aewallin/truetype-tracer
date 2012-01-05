
#pragma once

#include "writer.hpp"

class SEG_Writer : public Writer {
public:
    SEG_Writer() {
        has_arc(false);
        has_cubic(false);
        has_conic(false);
    }
    virtual void preamble() {
        std::cout << "(preamble)\n";
    }
    virtual void postamble(long int offset, extents line_extents) {
        std::cout << "(postamble)\n";
    }
    
    virtual void move_comment(P p) {}
    virtual void move_to(P p) {
        std::cout << "pen UP \n";
        std::cout << "  move to " << p.x << " , " << p.y << "\n";
        std::cout << "pen DOWN \n";
    }
    virtual void line(P p) {
        std::cout << "  line " << p.x << " , " << p.y << "\n";
    }
    virtual void line_comment(P c1, P c2, P to) {}
    virtual void line_to(P p) {
        std::cout << "lineto " << p.x << " , " << p.y << "\n";
    }
    
    virtual void cubic_comment(P c1, P c2, P to) {
        std::cout << "(cubic)\n";
    }
    virtual void cubic_to(P c1, P c2, P to) {}
    
    virtual void conic_comment(P to, P diff) {
        std::cout << "(conic)\n";
    }
    virtual void conic_as_lines_comment(int steps) {
        std::cout << " (approximating conic with "<< steps << " line segments)\n";
    }
    virtual void conic_to(P to, P diff) {}
    virtual void arc_small_den(P p) {} // dxf_writer lacks this functn!
    virtual void arc(P p2, double r, double gr, double bulge) {} 
    
    virtual void start_glyph(const char* s, wchar_t wc, long int offset) {
        if(isalnum(*s))
            std::cout << "(start of symbol " << (char)wc << ")\n"; 
        else
            std::cout << "(start of symbol 0x"<< std::hex << wc << std::dec <<")\n"; 
    }
    virtual void end_glyph(extents glyph_extents, FT_Vector advance) {
        std::cout << "(end glyph)\n";
    }

};
