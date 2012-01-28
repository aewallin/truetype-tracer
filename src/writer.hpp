#pragma once

#include "extents.hpp"

// this is a base-class for Writers.
// Ttt calls this public interface, to produce output in various formats.
// subclass this to produce another writer.
// FIXME: dwg, APT, SVG, STEP and other formats could possibly be produced?
class Writer {
public:
    Writer() {
        _conic_biarc_subdiv = 200;
        _conic_line_subdiv = 200;
        _cubic_biarc_subdiv = 200;
        _cubic_line_subdiv = 200;
        scale = 1;
    }
    virtual bool has_arc() const {return _arc;} 
    virtual bool has_arc(bool b) {_arc=b;}
    bool get_arc() const { return has_arc();}
    void set_arc(bool b) { has_arc(b);}
    
    virtual bool has_conic() const {return _conic;}
    virtual bool has_conic(bool b) {_conic=b;}
    bool get_conic() const {return has_conic();}
    void set_conic(bool b) { has_conic(b);}
    
    virtual bool has_cubic() const {return _cubic;}
    virtual bool has_cubic(bool b) {_cubic=b;}
    bool get_cubic() const {return has_cubic();}
    void set_cubic(bool b) { has_cubic(b);}
    
    void set_font(std::string f) {ttfont = f;}
    std::string get_font() const {return ttfont;}
    
    void set_text(std::string t) {text = t;}
    std::string get_text() const { return text; }
    
    void set_scale(double s) {scale = s;}
    double get_scale() const { return scale; }
    
    extents get_extents() {
        return ext;
    }
    void set_extents(extents e) {
        ext=e;
    }
    virtual void preamble() {}
    virtual void postamble(long int offset, extents line_extents) {}
    
    virtual void move_comment(P p) {}
    virtual void move_to(P p) {}
    virtual void line(P p) {}
    virtual void line_comment(P c1, P c2, P to) {}
    virtual void line_to(P p) {}
    
    virtual void cubic_to(P c1, P c2, P to) {}
    virtual void cubic_comment(P c1, P c2, P to) {}
    // cubic_as_lines_comment(int stes)
    double get_cubic_biarc_subdiv() const {return _cubic_biarc_subdiv;}
    void set_cubic_biarc_subdiv(double s) {_cubic_biarc_subdiv=s;}
    double get_cubic_line_subdiv() const {return _cubic_line_subdiv;}
    void set_cubic_line_subdiv(double s) {_cubic_line_subdiv=s;}
    
    virtual void conic_to(P to, P diff) {}
    virtual void conic_comment(P to, P diff) {}
    virtual void conic_as_lines_comment(int steps) {}
    double get_conic_biarc_subdiv() const {return _conic_biarc_subdiv;}
    void set_conic_biarc_subdiv(double s) {_conic_biarc_subdiv=s;}
    double get_conic_line_subdiv() const {return _conic_line_subdiv;}
    void set_conic_line_subdiv(double s) {_conic_line_subdiv=s;}
    
    virtual void arc_small_den(P p) {} // dxf_writer lacks this functn!
    virtual void arc(P p2, double r, double gr, double bulge) {}
    //double get_arc_line_subdiv() const {return _arc_line_subdiv;}
    //void set_arc_line_subdiv(double s) {_arc_line_subdiv=s;}
    
    
    virtual void start_glyph(const char* s, wchar_t wc, long int offset) {}
    virtual void end_glyph(extents glyph_extents, FT_Vector advance) {}
private:
    bool _conic; // true if the Writer can output native conics. If false, conics are output as biarcs.
    double _conic_biarc_subdiv;
    double _conic_line_subdiv;
    
    bool _cubic; // true if Writer can output cubics. If false, cubics are  output as biarcs.
    double _cubic_biarc_subdiv;
    double _cubic_line_subdiv;
    
    bool _arc; // true if Writer can ouput arcs. If false, arcs are output as lines.
    //double _arc_line_subdiv; // UNUSED!!
    
    std::string ttfont;
    std::string text;
    double scale;
    extents ext;
};
