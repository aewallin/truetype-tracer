// experimental c++ port of truetype-tracer
// January 2012, anders.e.e.wallin "at" gmail.com
    
#include "ttt.hpp"
#include "p.hpp"
#include "ngc_writer.hpp"

void handle_ft_error(std::string where, int f, int x) {
    const struct ftError *e = &ft_errors[0];
    for(;e->err_msg && e->err_code != f;e++) ;
    if(e->err_msg) {
        fprintf(stderr, "Fatal error in %s: %s (%d) at line:%d\n", where.data() , e->err_msg, f, x);
    } else {
        fprintf(stderr, "Fatal error in %s: %d at line:%d\n", where.data() , f,x);
    }
    exit(x);
}

Ttt* Ttt::self = NULL;

Ttt::Ttt() {
        self = this;
        int error;
        //int i, l;
        //long int offset;
        int unicode = 0;
        //char *s;
        std::string ttfont = TTFONT;
        //double scale = 0.0003;
        int linescale = 0;

        //csteps=100;
        my_writer = new NGC_Writer(0.003, false);

        error = FT_Init_FreeType(&library);
        if(error) handle_ft_error("FT_Init_FreeType" , error, __LINE__);

        error = FT_New_Face(library, ttfont.data() , 0, &face);
        if(error) handle_ft_error("FT_New_Face", error, __LINE__);
        
        #define MYFSIZE 64
        error = FT_Set_Pixel_Sizes(face, 0, MYFSIZE);     
        if (error) handle_ft_error("FT_Set_Pixel_Sizes", error, __LINE__);

        if (unicode) setlocale(LC_CTYPE, "");
        
        std::string str="Hello world.";

        int l = str.length();
        
        //  preamble
        my_writer->preamble();
        
        line_extents.reset();
        long int offset = 0;
        
        const char* s = str.data();
        // loop through characters
        while(*s ) {
            wchar_t wc;
            int r = mbtowc(&wc, s, l); // convert multibyte s, store in wc. return number of converted bytes
            if(r==-1) { s++; continue; }
            
            // g-code start-of-glyph comments
            
            glyph_extents.reset();
            offset += render_char(face, wc, offset, linescale);
            line_extents.add_extents(glyph_extents);
            s += r; l -= r;
            
            // g-code glyph extents comments

        }
        
        // post-amble
        my_writer->postamble(offset, line_extents);
}

// lookup glyph and extract all the shapes required to draw the outline
long int Ttt::render_char(FT_Face face, wchar_t c, long int offset, int linescale) {
    int error;
    int glyph_index;
    FT_Outline outline;
    FT_Outline_Funcs func_interface;
    error = FT_Set_Pixel_Sizes(face, 4096, linescale ? linescale : 64);
    if(error) handle_ft_error("FT_Set_Pixel_Sizes", error, __LINE__);
    /* lookup glyph */
    glyph_index = FT_Get_Char_Index(face, (FT_ULong)c);
    if(!glyph_index) handle_ft_error("FT_Get_Char_Index", 0, __LINE__);
    /* load glyph */
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING);
    if(error) handle_ft_error("FT_Load_Glyph", error, __LINE__);
    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);
    if(error) handle_ft_error("FT_Render_Glyph", error, __LINE__);
    if(linescale > 0)
        my_draw_bitmap(&face->glyph->bitmap, 
                       face->glyph->bitmap_left + offset,
                       face->glyph->bitmap_top,
                       linescale);

    error = FT_Set_Pixel_Sizes(face, 0, 64);
    if(error) handle_ft_error("FT_Set_Pixel_Sizes", error, __LINE__);
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_BITMAP |
                                             FT_LOAD_NO_HINTING);
    if(error) handle_ft_error("FT_Load_Glyph", error, __LINE__);

    /* shortcut to the outline for our desired character */
    outline = face->glyph->outline;

    /* set up entries in the interface used by FT_Outline_Decompose() */
    func_interface.shift = 0;
    func_interface.delta = 0;
    func_interface.move_to = move_to_wrapper;
    func_interface.line_to = line_to_wrapper;
    func_interface.conic_to = conic_to_wrapper;
    func_interface.cubic_to = cubic_to_wrapper;

    /* offset the outline to the correct position in x */
    FT_Outline_Translate( &outline, offset, 0L );

    /* plot the current character */
    error = FT_Outline_Decompose( &outline, &func_interface, NULL);
    if(error) handle_ft_error("FT_Outline_Decompose", error, __LINE__);

    /* save advance in a global */
    advance.x = face->glyph->advance.x;
    advance.y = face->glyph->advance.y;

    /* offset will get bumped up by the x size of the char just plotted */
    return face->glyph->advance.x;
}

// move with 'pen up' to a new position and then put 'pen down' 
int Ttt::my_move_to( const FT_Vector* to, void* user ) {
    P pt(to);
    my_writer->move_to( pt );
    last_point = *to;
    glyph_extents.add_point( *to);
    return 0;
}

int Ttt::my_line_to( const FT_Vector* to, void* user ) {
    P p(to);
    my_writer->line_to(p);
    last_point = *to;
    glyph_extents.add_point( *to);
    return 0;
}

int Ttt::my_conic_to( const FT_Vector* control, const FT_Vector* to, void* user ) {
    // G5.1 output here
    P to_pt(to);
    P ctrl_pt(control);
    P last_pt(&last_point);
    P diff( ctrl_pt-last_pt );
    my_writer->conic_to( to_pt, diff );
    return 0;
// OR, for dxf, calculate polyline
/*
    int t;
    double x,y;
    FT_Vector point=last_point;
    double len=0;
    int csteps = 10; // ??
    double dsteps = 1; // ??
    //double l[csteps+1];
    //l[0] = 0;
    for(t=1; t<=csteps; t++) {
        double tf = (double)t/(double)csteps;
        x = SQ(1-tf) * last_point.x + 2*tf*(1-tf) * control->x + SQ(tf) * to->x;
        y = SQ(1-tf) * last_point.y + 2*tf*(1-tf) * control->y + SQ(tf) * to->y;
        len += hypot(x-point.x, y-point.y);
        point.x = x;
        point.y = y;
        glyph_extents.add_point( point );
    }

    P p0( &last_point );
    P p1( control );
    P p2( to );
    P q0=p1 - p0;
    P q1=p2 - p1;
    P ps=p0;
    P ts=q0;
    int steps = (int) std::max( (double)2, len/dsteps);
    for(t=1; t<=steps; t++) {
        double tf = (double)t/(double)steps;
        double t1 = 1-tf;
        //P p = add3(scale(p0, SQ(t1)), scale(p1, 2*tf*t1), scale(p2, SQ(tf)));
        P p = p0.scale(SQ(t1)) + p1.scale(2*tf*t1) + p2.scale(SQ(tf));
        //P t = add(scale(q0, t1), scale(q1, tf));
        P t = q0.scale(t1) + q1.scale(tf); //add(scale(q0, t1), scale(q1, tf));
        biarc(ps, ts, p, t, 1.0); // DXF?
        ps = p; ts = t;
    }

    last_point = *to;
    return 0;
    */
}

int Ttt::my_cubic_to(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector *to, void* user) {

    P ctrl1(control1);
    P ctrl2(control2);
    P to_pt(to);
    my_writer->cubic_to( ctrl1, ctrl2, to_pt );

/*
    FT_Vector point=last_point;
    double len=0;
    
    int csteps = 10;
    double dsteps = 0.1;
// OR dxf-output
    for(int t=1; t<=csteps; t++) {
        double tf = (double)t/(double)csteps;
        int x = CUBE(1-tf)*last_point.x     + 
            SQ(1-tf)*3*tf*control1->x   +
            SQ(tf)*(1-tf)*3*control2->x +
            CUBE(tf)*to->x;
        int y = CUBE(1-tf)*last_point.y     + 
            SQ(1-tf)*3*tf*control1->y   +
            SQ(tf)*(1-tf)*3*control2->y +
            CUBE(tf)*to->y;
        len += hypot(x-point.x, y-point.y);
        point.x = x;
        point.y = y;
        glyph_extents.add_point(point);
    }

    int steps = (int) std::max( (double)2, len/dsteps);
*/

    /*   // g-code comment
#ifndef DXF // we NEVER get here ??
    printf(";cubicto %ld,%ld %ld,%ld %ld,%ld) ", control1->x, control1->y, control2->x, control2->y, to->x, to->y);
    printf("len=%f steps=%d\n", len, steps);
#endif
* */

/*
    P p0(&last_point);
    P p1(control1);
    P p2(control2);
    P p3=(to);
    P q0=p1-p0;
    P q1=p2-p1;
    P q2=p3-p2;
    P ps=p0;
    P ts=q0;
    for(int t=1; t<=steps; t++) {
        double tf = t*1.0/steps;
        double t1 = 1-tf;
        //P p = add4(
        //    scale(p0, CUBE(t1)), scale(p1, 3*tf*SQ(t1)),
        //    scale(p2, 3*SQ(tf)*t1), scale(p3, CUBE(tf)));
        P p = p0*CUBE(t1) + p1*3*tf*SQ(t1) + p2*3*SQ(tf)*t1 + p3*CUBE(tf) ;
        //P t = add3(scale(q0, SQ(t1)), scale(q1, 2*tf*t1), scale(q2, SQ(tf)));
        P t = q0*SQ(t1) + q1*2*tf*t1 + q2*SQ(tf);
        biarc(ps, ts, p, t, 1.0);
        ps = p; ts = t;
    }

    last_point = *to;

    return 0;
    */
}

void Ttt::line(P p) {
    my_writer->line(p);

}

void Ttt::arc(P p1, P p2, P d) {
    d.unit();
    P p = p2-p1; //sub(p2, p1);
    double den = 2 * (p.y*d.x - p.x*d.y);

    if(fabs(den) < 1e-10) { // does this happen for DXF?
        //printf("G1 X[%.4f*#3+#5] Y[%.4f*#3+#6]\n", p2.x, p2.y);
        return;
    }

    double r = - p.dot(p)/den;
    double i = d.y*r;
    double j = -d.x*r;

    P c(p1.x+i, p1.y+j );
    double st = atan2(p1.y-c.y, p1.x-c.x);
    double en = atan2(p2.y-c.y, p2.x-c.x);

    if(r < 0)
        while(en <= st) en += 2*M_PI;
    else
        while(en >= st) en -= 2*M_PI;

/*
#ifdef DXF
    double bulge = tan(fabs(en-st)/4);
    if(r > 0) bulge = -bulge;
        printf("  42\n%.4f\n  70\n1\n"
           "  0\nVERTEX\n  8\n0\n  10\n%.4f\n  20\n%.4f\n  30\n0.0\n",
           bulge, p2.x, p2.y);
#else
    double gr = (en - st) < M_PI ? fabs(r) : -fabs(r);
    if(r < 0)
        printf("G3 X[%.4f*#3+#5] Y[%.4f*#3+#6] R[%.4f*#3]\n",
            p2.x, p2.y, gr);
    else
        printf("G2 X[%.4f*#3+#5] Y[%.4f*#3+#6] R[%.4f*#3]\n",
            p2.x, p2.y, gr);
#endif
*/
}

void Ttt::biarc(P p0, P ts, P p4, P te, double r) {
    ts.unit();
    te.unit();

    P v = p0-p4; //sub(p0, p4);

    double c = v.dot(v); // dot(v,v);
    double b = 2 * v.dot( ts*r + te  ); // add(scale(ts, r), te));
    double a = 2 * r * ( ts.dot(te) -1);

    double disc = b*b-4*a*c;
    
    if(a == 0 || disc < 0) {
        line(p4);
        return;
    }

    double disq = sqrt(disc);
    double beta1 = (-b - disq) / 2 / a;
    double beta2 = (-b + disq) / 2 / a;
    double beta = std::max(beta1, beta2);
    
    if(beta <= 0) {
        line(p4);
        return;
    }

    double alpha = beta*r;
    double ab = alpha+beta;
    P p1 = p0 + ts*alpha; //add(p0, scale(ts, alpha));
    P p3 = p4 + te*(-beta); //add(p4, scale(te, -beta));
    P p2 = p1*(beta/ab) + p3*(alpha/ab); //add(scale(p1, beta/ab), scale(p3, alpha/ab));
    P tm = p3-p2; //sub(p3, p2);

    arc(p0, p2, ts);
    arc(p2, p4, tm);    
}

void Ttt::my_draw_bitmap(FT_Bitmap *b, FT_Int x, FT_Int y, int linescale) {
    FT_Int i, j;
    static int oldbit; // ?
    FT_Vector oldv = {99999,0};
    FT_Vector vbuf[100]; //freetype says no more than 32 ever?
    int spans = 0;
    int pitch = abs(b->pitch);
    static int odd=0;
    for(j = 0; j < b->rows; j++) {
        FT_Vector v;
        oldbit = 0;
        spans = 0;
        for(i = 0; i < pitch; i++) {
            unsigned char byte = b->buffer[j * pitch + i], mask, bits;
            for(bits = 0, mask = 0x80; mask; bits++, mask >>= 1) {
                unsigned char bit = byte & mask;
                v.x = i*8+bits+x;
                v.y = (y-j)*64*64/linescale-64*32/linescale;
                if(!oldbit && bit) {
                    v.x += 8;
                    oldv = v;
                    vbuf[spans++] = v;
                }
                if(oldbit && !bit) {
                    v.x -= 8;
                    if(oldv.x < v.x) {
                        vbuf[spans++] = v;
                    } else spans--;
                }
                oldbit = bit;
            }
        }
        if(oldbit) {
            v.x -= 8;
            vbuf[spans++] = v;
        }
        odd = !odd;
        spans /= 2;
        if(odd) {
            for (int i=spans-1; i>=0; i--) {
                my_move_to(vbuf+1+(i*2), (void*)1);
                my_line_to(vbuf+(i*2), (void*)1);
            }
        } else {
            for (int i=0; i<spans; i++) {
                my_move_to(vbuf+(i*2), (void*)1);
                my_line_to(vbuf+1+(i*2), (void*)1);
            }
        }
    }
}
 
