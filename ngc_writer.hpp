#pragma once

#include "writer.hpp"

class NGC_Writer : public Writer {
public:
    NGC_Writer() 
        :   unicode(false), scale(0.0003), bd(false) {
            has_arc(true);
            has_conic(true);
            has_cubic(true);
    }
    NGC_Writer(  bool unicode, double scale, bool blockdelete = false ) 
        :   unicode(unicode), scale(scale), bd(blockdelete) {
            has_arc(true);
            has_conic(true);
            has_cubic(true);
    }
        
    virtual void preamble() {
        std::cout << "(font: "<< get_font() <<")\n"; 
        
        if(!unicode) {
            std::cout << "(text: ";
            const char* s = get_text().data();
            int l = get_text().length();
            for (int i=0; i<l; i++)
                if(isalnum(s[i]))
                    std::cout << s[i];
                else
                    std::cout << "*";
            std::cout << ")\n";
        }
        
        std::cout << "#1=0.1    (SafeHeight)\n";
        std::cout << "#2=0.01   (Depth of Cut)\n";
        std::cout << "#3="<< scale << " (XY Scale)\n"; 
        std::cout << "#4=10.0   (Feed)\n";
        std::cout << "#5=0.0    (X offset)\n";
        std::cout << "#6=0.0    (Y offset)\n";    
    }
    virtual void postamble(long int offset, extents line_extents) {
        std::cout << "(final X offset: "<< offset << ")\n"; 
        if ( line_extents.maxx > line_extents.minx ) {
            std::cout << "(overall extents: X = "<< line_extents.minx <<" to ";
            std::cout << line_extents.maxx << ", Y = " << line_extents.miny << " to ";
            std::cout << line_extents.maxy << ")\n"; 
        }
        std::cout << "G00 Z #1\nM02\n";
    }
    virtual void start_glyph(const char* s,wchar_t wc, long int offset) {
        if(isalnum(*s))
            std::cout << "(start of symbol " << (char)wc << ")\n"; 
        else
            std::cout << "(start of symbol 0x"<< std::hex << wc << std::dec <<")\n"; 
        
        /* comment with offset info */
        std::cout << "(starting X offset: "<< offset << ")\n"; 
    }
    virtual void end_glyph(extents glyph_extents, FT_Vector advance) {
        if ( glyph_extents.maxx > glyph_extents.minx ) {
            std::cout << "(symbol extents: X = " << glyph_extents.minx;
            std::cout << " to " << glyph_extents.maxx;
            std::cout << ", Y = " << glyph_extents.miny;
            std::cout << " to " << glyph_extents.maxy << ")\n";
        }
        std::cout << "(symbol advance: X = "<< advance.x << ", Y = " << advance.y <<")\n"; //, ,  );
    }
    virtual void move_to(P p) {
        //if (debug) printf("(moveto %ld,%ld)\n", to->x, to->y);
        //char *blockdelete = user ? "/": "";
        //printf("%sG00 Z #1\n", blockdelete);
        //printf("%sG00 X [%ld*#3+#5] Y [%ld*#3+#6] (moveto)\n", blockdelete, to->x, to->y);
        //printf("%sG01 Z [0-#2] F#4\n", blockdelete);
        std::cout << "G00 Z #1\n";
        std::cout << "G00 X [" << p.x << "*#3+#5] Y [" << p.y << "*#3+#6] (moveto)\n";
        std::cout << "G01 Z [0-#2] F#4\n"; //, blockdelete);
    }
    virtual void line(P p) {
        std::cout << "G01 X [" << p.x << "*#3+#5] Y [" << p.y << "*#3+#6] (lineto)\n";
    }
    virtual void line_to(P p) {
        //char *blockdelete = user? "/": "";
        // printf("%sG01 X [%ld*#3+#5] Y [%ld*#3+#6] (lineto)\n", blockdelete, to->x, to->y);
        std::cout << "G01 X [" << p.x << "*#3+#5] Y [" << p.y << "*#3+#6] (lineto)\n"; //, blockdelete, to->x, to->y);
    }
    virtual void conic_to(P to, P diff ) {
        std::cout << "G5.1 X[" << to.x << "*#3+#5] Y[" << to.y << "*#3+#6] I["<< diff.x <<"*#3] J[" << diff.y <<"*#3]\n";
    }
    virtual void conic_comment(P to, P diff) {
        // printf("(conicto %ld,%ld via %ld,%ld)\n", to->x,to->y,control->x,control->y);
    }
    
    virtual void cubic_comment(P c1, P c2, P to) {
        std::cout << "(cubicto " << to.x << ", " << to.y << " via " << c1.x << ", " << c1.y;
        std::cout << " and " << c2.x << ", " << c2.y << "\n";
    }
    virtual void cubic_to(P ctrl1, P ctrl2, P to ) {
        std::cout << "G5.2 X[" << ctrl1.x << "*#3+#5] Y[" << ctrl1.y << "*#3+#6] L4 P1\n";
        std::cout << "X[" << ctrl2.x << "*#3+#5] Y[" << ctrl2.y << "*#3+#6] P1\n"; //, control2->x, control2->y
        std::cout << "X[" << to.x << "*#3+#5] Y[" << to.y << "*#3+#6] P1\n";
        std::cout << "G5.3\n";
    }
    virtual void arc_small_den(P p2) {
        std::cout << "G1 X["<< p2.x <<"*#3+#5] Y["<< p2.y<<"%.4f*#3+#6]\n"; //, p2.x, p2.y);
    }
    virtual void arc(P p2, double r, double gr, double bulge) {
        if(r < 0)
            std::cout << "G3 X[" << p2.x << "*#3+#5] Y["<< p2.y  << "*#3+#6] R["<< gr <<"*#3]\n";
        else
            std::cout << "G2 X[" << p2.x << "*#3+#5] Y["<< p2.y  << "*#3+#6] R["<< gr <<"*#3]\n";
    }
    double get_scale() {return scale;}
    void set_scale(double s) {scale = s;}
    bool get_blockdelete() {return bd;}
    void set_blockdelete(bool b) {bd = b;}
private:
    
    double scale;
    bool bd; // blockdelete

    bool unicode;
};
