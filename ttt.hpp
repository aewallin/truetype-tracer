// experimental c++ port of truetype-tracer
// January 2012, anders.e.e.wallin "at" gmail.com
    
#pragma once

#include <string>
#include <cmath>
#include <iostream>

#include "extents.hpp"
#include "p.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

#undef __FTERRORS_H__
#define FT_ERRORDEF( e, v, s )  { e, s },
#define FT_ERROR_START_LIST     {
#define FT_ERROR_END_LIST       { 0, 0 } };

const struct ftError {
  int          err_code;
  const char*  err_msg;
} ft_errors[] =
#include FT_ERRORS_H

// this is the default font used if not specified on commandline
#define TTFONT "/usr/share/fonts/truetype/freefont/FreeSerifBoldItalic.ttf"

// routine to print out hopefully-useful error messages
void handle_ft_error(std::string where, int f, int x);

#define NEQ(a,b) ((a).x != (b).x || (a).y != (b).y)
#define SQ(a) ((a)*(a))
#define CUBE(a) ((a)*(a)*(a))

class Writer {
public:
    virtual void preamble() = 0;
    virtual void postamble(long int offset, extents line_extents) = 0;
    virtual void move_to(P p) = 0;
    virtual void line(P p) = 0;
    virtual void line_to(P p) = 0;
    virtual void cubic_to(P c1, P c2, P to) = 0;
    virtual void conic_to(P to, P diff) = 0;
};

class NGC_Writer : public Writer {
public:
    NGC_Writer(double scale, bool blockdelete = false ) : scale(scale), bd(blockdelete) {}
    
    virtual void preamble() {
        std::cout << "#1=0.1    (SafeHeight)\n";
        std::cout << "#2=0.01   (Depth of Cut)\n";
        std::cout << "#3="<< scale << " (XY Scale)\n"; //, scale); // scale used here!
        std::cout << "#4=10.0   (Feed)\n";
        std::cout << "#5=0.0    (X offset)\n";
        std::cout << "#6=0.0    (Y offset)\n";
    
        //std::cout << " (g-code preamble)\n";
    }
    virtual void postamble(long int offset, extents line_extents) {
        std::cout << "(final X offset: "<< offset << ")\n"; //, offset);
        if ( line_extents.maxx > line_extents.minx ) {
            std::cout << "(overall extents: X = "<< line_extents.minx <<" to ";
            std::cout << line_extents.maxx << ", Y = " << line_extents.miny << "  to ";
            std::cout << line_extents.maxy << ")\n"; //, line_extents.minx, , line_extents.miny, line_extents.maxy);
        }
        std::cout << "G00 Z #1\nM02\n";
    }
    virtual void move_to(P p) {
        //if (debug) printf("(moveto %ld,%ld)\n", to->x, to->y);
        //char *blockdelete = user? "/": "";
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
        //printf("G5.1 X[%ld*#3+#5] Y[%ld*#3+#6] I[%ld*#3] J[%ld*#3]\n", to->x, to->y, control->x - last_point.x, control->y - last_point.y);
        std::cout << "G5.1 X[" << to.x << "*#3+#5] Y[" << to.y << "*#3+#6] I["<< diff.x <<"*#3] J[" << diff.y <<"*#3]\n";
        //, to->x, to->y, control->x - last_point.x, control->y - last_point.y)
    }
    virtual void cubic_to(P ctrl1, P ctrl2, P to ) {
        // printf("G5.2 X[%ld*#3+#5] Y[%ld*#3+#6] L4 P1\n", control1->x, control1->y);
        // printf("X[%ld*#3+#5] Y[%ld*#3+#6] P1\n", control2->x, control2->y);
        // printf("X[%ld*#3+#5] Y[%ld*#3+#6] P1\n", to->x, to->y);
        // printf("G5.3\n");
        std::cout << "G5.2 X[" << ctrl1.x << "*#3+#5] Y[" << ctrl1.y << "*#3+#6] L4 P1\n";
        std::cout << "X[" << ctrl2.x << "*#3+#5] Y[" << ctrl2.y << "*#3+#6] P1\n"; //, control2->x, control2->y
        std::cout << "X[" << to.x << "*#3+#5] Y[" << to.y << "*#3+#6] P1\n";
        std::cout << "G5.3\n";
    }
private:
    double scale;
    bool bd; // blockdelete
};



class Ttt {
public:
    Ttt();

private:
    long int render_char(FT_Face face, wchar_t c, long int offset, int linescale) ;
    void my_draw_bitmap(FT_Bitmap *b, FT_Int x, FT_Int y, int linescale);
    
    int my_move_to( const FT_Vector* to, void* user );
    int my_line_to( const FT_Vector* to, void* user );
    int my_conic_to( const FT_Vector* control, const FT_Vector* to, void* user );
    int my_cubic_to(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector *to, void* user);
    
    void line(P p);
    void arc(P p1, P p2, P d);
    void biarc(P p0, P ts, P p4, P te, double r);
    
    static int move_to_wrapper( const FT_Vector* to, void* user ) {
        self->my_move_to(to,user);
    }
    static int line_to_wrapper( const FT_Vector* to, void* user ) {
        self->my_line_to(to,user);
    }
    static int conic_to_wrapper( const FT_Vector* control, const FT_Vector* to, void* user ) {
        self->my_conic_to(control,to,user);
    }
    static int cubic_to_wrapper( const FT_Vector* control1, const FT_Vector* control2,
                                     const FT_Vector *to, void* user ) {
        self->my_cubic_to(control1, control2, to,user);
    }

    FT_Library library;
    FT_Face face;
    extents line_extents;
    extents glyph_extents;
    FT_Vector advance;
    FT_Vector last_point;
    static Ttt* self;
    Writer* my_writer;
};
