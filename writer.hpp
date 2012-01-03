
#pragma once

class Writer {
public:

    virtual bool has_conic() {return false;}
    virtual bool has_cubic() {return false;}
    
    virtual void preamble() = 0;
    virtual void postamble(long int offset, extents line_extents) = 0;
    virtual void move_to(P p) = 0;
    virtual void line(P p) = 0;
    virtual void line_to(P p) = 0;
    virtual void cubic_to(P c1, P c2, P to) = 0;
    virtual void conic_to(P to, P diff) = 0;
    virtual void arc_small_den(P p) {} // dxf_writer lacks this functn
    virtual void arc(P p2, double r, double gr, double bulge) {} // todo: dxf
    virtual void start_glyph(const char* s, wchar_t wc, long int offset) {}
    virtual void end_glyph(extents glyph_extents, FT_Vector advance) {}
};
