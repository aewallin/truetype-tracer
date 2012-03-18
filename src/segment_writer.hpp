#pragma once

#include <boost/foreach.hpp>

#include <boost/python.hpp>
namespace bp = boost::python;
#include "writer.hpp"

// this experimental writer outputs python lists with coordinates.
// used for testing OpenVoronoi.
class SEG_Writer : public Writer {
public:
    SEG_Writer() : Writer() {
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
        if ( !current_loop.empty() ) {
            //seglist.append(seg); 
            all_loops.push_back(current_loop);
            current_loop.clear();
            //seg = bp::list(); // empty seg
        }
        std::cout << "pen UP \n";
        std::cout << "  move to " << p.x << " , " << p.y << "\n";
        std::cout << "pen DOWN \n";
        append_point(p);
    }
    
    virtual void line(P p) {
        std::cout << last_point.x << " , " << last_point.y << " line " <<p.x << " , " << p.y << "\n";
        append_point(p);
    }
    //virtual void line_comment(P c1, P c2, P to) {}
    virtual void line_to(P p) {
        std::cout << last_point.x << " , " << last_point.y << " lineto " << p.x << " , " << p.y << "\n";
        append_point(p);
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
    //virtual void conic_to(P to, P diff) {}
    //virtual void arc_small_den(P p) {} // dxf_writer lacks this functn!
    //virtual void arc(P p2, double r, double gr, double bulge) {} 
    
    virtual void start_glyph(const char* s, wchar_t wc, long int offset) {
        if(isalnum(*s))
            std::cout << "(start of symbol " << (char)wc << ")\n"; 
        else
            std::cout << "(start of symbol 0x"<< std::hex << wc << std::dec <<")\n"; 
    }
    virtual void end_glyph(extents glyph_extents, FT_Vector advance) {
        if ( !current_loop.empty() ) {
            //seglist.append(seg); //std::cout << "(empty seg)\n";
            all_loops.push_back(current_loop);
            current_loop.clear();
            //seg = bp::list();
        }
        std::cout << "(end glyph)\n";
    }
    bp::list get_segments() {
        bp::list out;
        BOOST_FOREACH(Loop l, all_loops) {
            bp::list pyloop;
            BOOST_FOREACH(Point pt, l) {
                bp::list pypt;
                pypt.append(pt.first);
                pypt.append(pt.second);
                pyloop.append(pypt);
            }
            out.append(pyloop);
        } 
        return out; 
    }
protected:
    typedef std::pair<double,double> Point;
    typedef std::vector<Point> Loop;
    typedef std::vector<Loop> Loops;
    
    void append_point(P p) {
        Point pt;
        pt.first  = get_scale()*p.x;
        pt.second = get_scale()*p.y;
        current_loop.push_back(pt);
        last_point = p;
    }

    //std::vector< s
    Loop current_loop;
    Loops all_loops;
    //bp::list seglist;
    //bp::list seg;
    P last_point;
};
