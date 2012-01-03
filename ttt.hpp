// experimental c++ port of truetype-tracer
// January 2012, anders.e.e.wallin "at" gmail.com

#pragma once

#include <string>
#include <cmath>
#include <iostream>
#include <sstream>

#include "extents.hpp"
#include "p.hpp"
#include "writer.hpp"


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


struct cout_redirect {
    cout_redirect( std::streambuf* new_buffer ) 
        : old( std::cout.rdbuf( new_buffer ) )
    { }
    ~cout_redirect( ) {
        std::cout.rdbuf( old );
    }

private:
    std::streambuf * old;
};

class Ttt {
public:
    Ttt( Writer* wr, 
         std::string str="Hello world.",
         int unicode = 0,
         std::string ttfont = TTFONT) ;
         
    std::string get_output() {
        return buffer.str();
    }
private:
    long int render_char(FT_Face face, wchar_t c, long int offset, int linescale) ;
    void my_draw_bitmap(FT_Bitmap *b, FT_Int x, FT_Int y, int linescale);
    
    int my_move_to( const FT_Vector* to, void* user );
    int my_line_to( const FT_Vector* to, void* user );
    int my_conic_to( const FT_Vector* control, const FT_Vector* to, void* user );
    int my_cubic_to(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector *to, void* user);
    int my_cubic_as_biarcs(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector *to, void* user);
    int my_conic_as_biarcs( const FT_Vector* control, const FT_Vector* to, void* user );
    
    void line(P p);
    void arc(P p1, P p2, P d);
    void biarc(P p0, P ts, P p4, P te, double r);
    
    // these static wrappers are required because Freetype wants callback
    // functions to be of (*void) type.
    
    static int move_to_wrapper( const FT_Vector* to, void* user ) {
        self->my_move_to(to,user);
    }
    static int line_to_wrapper( const FT_Vector* to, void* user ) {
        self->my_line_to(to,user);
    }
    static int conic_to_wrapper( const FT_Vector* control, const FT_Vector* to, void* user ) {
        return self->my_conic_dispatch(control,to,user);
    }
    static int cubic_to_wrapper( const FT_Vector* control1, 
                                 const FT_Vector* control2,
                                 const FT_Vector *to, void* user ) {
        self->my_cubic_dispatch(control1, control2, to,user);
    }
    
    // dispatch to native conic output, if writer is capable
    int my_conic_dispatch( const FT_Vector* control, const FT_Vector* to, void* user ) {
        if (my_writer->has_conic())
            return self->my_conic_to(control,to,user);
        else
            return self->my_conic_as_biarcs(control,to,user);
    }
    
    // native or biarc cubic output
    int my_cubic_dispatch( const FT_Vector* control1, 
                                 const FT_Vector* control2,
                                 const FT_Vector *to, void* user ) {
        if (my_writer->has_cubic()) 
            return self->my_cubic_to(control1, control2, to,user);
        else
            return self->my_cubic_as_biarcs(control1,control2,to,user);
    }
    

    
    FT_Library library;
    FT_Face face;
    extents line_extents;
    extents glyph_extents;
    FT_Vector advance;
    FT_Vector last_point;
    static Ttt* self;
    Writer* my_writer;
    std::stringstream buffer;
};
