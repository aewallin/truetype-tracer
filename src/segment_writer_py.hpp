#pragma once

#include <boost/foreach.hpp>

#include <boost/python.hpp>
namespace bp = boost::python;
#include "segment_writer.hpp"

// python wrapper SEG_Writer
class PySEG_Writer : public SEG_Writer {
public:
    PySEG_Writer() : SEG_Writer() {
    }

    bp::list py_get_loops() {
        bp::list out;
        BOOST_FOREACH(Loop l, all_loops) {
            bp::list pyloop;
            BOOST_FOREACH(Point pt, l) {
                bp::list pypt;
                pypt.append(pt.x);
                pypt.append(pt.y);
                pypt.append(pt.r);
                pypt.append(pt.cw);
                pypt.append(pt.cx);
                pypt.append(pt.cy);
                pyloop.append(pypt);
            }
            out.append(pyloop);
        } 
        return out; 
    }
};
