
#pragma once

// this is a base-class for Writers.
// Ttt calls this public interface 
class Writer {
public:
    virtual bool has_arc() {return _arc;}
    virtual bool has_arc(bool b) {_arc=b;}
    bool get_arc() { return has_arc();}
    void set_arc(bool b) { has_arc(b);}
    
    virtual bool has_conic() {return _conic;}
    virtual bool has_conic(bool b) {_conic=b;}
    bool get_conic() {return has_conic();}
    void set_conic(bool b) { has_conic(b);}
    
    virtual bool has_cubic() {return _cubic;}
    virtual bool has_cubic(bool b) {_cubic=b;}
    bool get_cubic() {return has_cubic();}
    void set_cubic(bool b) { has_cubic(b);}
    
    void set_font(std::string f) {ttfont = f;}
    std::string get_font() {return ttfont;}
    
    void set_text(std::string t) {text = t;}
    std::string get_text() { return text; }
    
    virtual void preamble() {}
    virtual void postamble(long int offset, extents line_extents) {}
    virtual void move_to(P p) {}
    virtual void line(P p) {}
    virtual void line_to(P p) {}
    virtual void cubic_to(P c1, P c2, P to) {}
    virtual void conic_to(P to, P diff) {}
    virtual void arc_small_den(P p) {} // dxf_writer lacks this functn!
    virtual void arc(P p2, double r, double gr, double bulge) {} 
    virtual void start_glyph(const char* s, wchar_t wc, long int offset) {}
    virtual void end_glyph(extents glyph_extents, FT_Vector advance) {}
private:
    bool _conic;
    bool _cubic;
    bool _arc;
    std::string ttfont;
    std::string text;
};
