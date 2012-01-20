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

// routine to print out hopefully-useful error messages
void handle_ft_error(std::string where, int f, int x);

#define NEQ(a,b) ((a).x != (b).x || (a).y != (b).y)
#define SQ(a) ((a)*(a))
#define CUBE(a) ((a)*(a)*(a))

// this redirects std::cout to a buffer which we can later output to python
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

// a class for the core ttt-algorithm
// calls the FreeType library to get geometry, and then calls a Writer to produce output
// FIXME:
// - make font an adjustabe parameter
class Ttt {
public:
    Ttt( Writer* wr, std::string str, int unicode , std::string ttfont );
    std::string get_output() { return buffer.str(); }
private:
    long int render_char(FT_Face face, wchar_t c, long int offset, int linescale) ;
    void my_draw_bitmap(FT_Bitmap *b, FT_Int x, FT_Int y, int linescale);

    void line(P p);
    void arc(P p1, P p2, P d);
    void biarc(P p0, P ts, P p4, P te, double r);

// FIXME: none of these int return-values are used for anything. Can be changed to void?

    int my_line_to( const FT_Vector* to, void* user );
    int my_move_to( const FT_Vector* to, void* user );
    
    int        my_conic_to( const FT_Vector* control, const FT_Vector* to, void* user );
    int my_conic_as_biarcs( const FT_Vector* control, const FT_Vector* to, void* user );
    int  my_conic_as_lines( const FT_Vector* control, const FT_Vector* to, void* user );
    std::pair<double,extents> conic_length(const FT_Vector* control, const FT_Vector* to);
        
    int        my_cubic_to(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector *to, void* user);
    int my_cubic_as_biarcs(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector *to, void* user);
    int  my_cubic_as_lines(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector *to, void* user);
    std::pair<double, extents> cubic_length(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to);

    // these static wrappers are required because Freetype wants callback
    // functions to be of (*void) type.
    static int move_to_wrapper( const FT_Vector* to, void* user ) { self->my_move_to(to,user); }
    static int line_to_wrapper( const FT_Vector* to, void* user ) { self->my_line_to(to,user); }
    static int conic_to_wrapper( const FT_Vector* control, const FT_Vector* to, void* user ) { return self->my_conic_dispatch(control,to,user); }
    static int cubic_to_wrapper( const FT_Vector* control1, 
                                 const FT_Vector* control2,
                                 const FT_Vector *to, void* user ) {
        self->my_cubic_dispatch(control1, control2, to,user);
    }
    
    // dispatch to native conic output, if writer is capable
    // otherwise approximate as arcs, if writer has arcs
    // otherwise approximate as lines, if writer does not have arcs
    int my_conic_dispatch( const FT_Vector* control, const FT_Vector* to, void* user ) {
        
        P to_pt(to);
        P ctrl_pt(control);
        P last_pt(&last_point);
        P diff =  ctrl_pt-last_pt ;
        my_writer->conic_comment(to_pt, diff);

        if (my_writer->has_conic())
            return self->my_conic_to(control,to,user);
        else if (my_writer->has_arc())
            return self->my_conic_as_biarcs(control,to,user);
        else
            return self->my_conic_as_lines(control,to,user);
    }
    
    // output a cubic as native, arcs, or lines.
    int my_cubic_dispatch( const FT_Vector* control1, 
                                 const FT_Vector* control2,
                                 const FT_Vector *to, void* user ) {
        P ctl1(control1);
        P ctl2(control2);
        P to_pt(to);
        my_writer->cubic_comment(ctl1,ctl2,to_pt);
    
        if (my_writer->has_cubic()) 
            return self->my_cubic_to(control1, control2, to,user);
        else if (my_writer->has_arc())
            return self->my_cubic_as_biarcs(control1,control2,to,user);
        else
            return self->my_cubic_as_lines(control1,control2,to,user);
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
