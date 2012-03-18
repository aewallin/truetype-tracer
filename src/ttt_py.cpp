// experimental c++ port of truetype-tracer
// January 2012, anders.e.e.wallin "at" gmail.com
    
#include <boost/python.hpp>
namespace bp = boost::python;

#include "ttt.hpp"
#include "writer.hpp"
#include "ngc_writer.hpp"
#include "dxf_writer.hpp"
#include "segment_writer.hpp"
#include "segment_writer_py.hpp"
#include "version_string.hpp"

// this is the default font used if not specified on commandline
// #define TTFONT "/usr/share/fonts/truetype/freefont/FreeSerifBoldItalic.ttf"
#define TTFONT "/usr/share/fonts/truetype/freefont/FreeSerif.ttf"

std::string version() {
    return VERSION_STRING;
}

static std::string dxf1(std::string str) {
    Writer* wr = new DXF_Writer( );
    Ttt* ttt = new Ttt(wr, str, false, TTFONT);
    std::string outp = ttt->get_output();
    delete ttt;
    delete wr;
    return outp;
}

static std::string ngc1(std::string str) {
    Writer* wr = new NGC_Writer(  false, 0.003, false );
    wr->set_text(str);
    wr->set_font(TTFONT);
    Ttt* ttt = new Ttt(wr, str, false, TTFONT);
    std::string outp = ttt->get_output();
    delete ttt;
    delete wr;
    return outp;
}

static std::string dxf0() {
    return dxf1("Hello World.");
}   

static std::string ngc0() {
    return ngc1("Hello World.");
}

static std::string ttt_with_writer(std::string str, Writer* wr) {
    //Writer* wr = new NGC_Writer( TTFONT, str, false, 0.003, false );
    if (str=="")
        str = "Hello world.";
    wr->set_text(str);
    //wr->set_font(TTFONT);
    Ttt* ttt = new Ttt(wr, str, false, wr->get_font() );
    std::string outp = ttt->get_output();
    delete ttt;
    //delete wr;
    return outp;
}
 
BOOST_PYTHON_MODULE(ttt) {

    bp::def("version", version);
    bp::def("ngc", ngc0); // zero arguments. "hello world" with all else defaults
    bp::def("ngc", ngc1);
    
    bp::def("dxf", dxf1);
    bp::def("dxf", dxf0);
    
    bp::def("ttt", ttt_with_writer);
    bp::class_<extents>("extents")
        .add_property("minx", &extents::minx)
        .add_property("miny", &extents::miny)
        .add_property("maxy", &extents::maxy)
        .add_property("maxx", &extents::maxx)
    ;
    bp::class_<Writer>("Writer")
        .add_property("arc", &Writer::get_arc, &Writer::set_arc)
        .add_property("conic", &Writer::get_conic, &Writer::set_conic)
        .add_property("cubic", &Writer::get_cubic, &Writer::set_cubic)
        .add_property("scale", &Writer::get_scale, &Writer::set_scale)
        .add_property("conic_biarc_subdivision", &Writer::get_conic_biarc_subdiv, &Writer::set_conic_biarc_subdiv)
        .add_property("conic_line_subdivision", &Writer::get_conic_line_subdiv, &Writer::set_conic_line_subdiv)
        .add_property("cubic_biarc_subdivision", &Writer::get_cubic_biarc_subdiv, &Writer::set_cubic_biarc_subdiv)
        .add_property("cubic_line_subdivision", &Writer::get_cubic_line_subdiv, &Writer::set_cubic_line_subdiv)
        .add_property("extents", &Writer::get_extents)
        .def("setFont", &Writer::set_font_number)
    ;
    bp::class_< NGC_Writer, bp::bases<Writer> >("NGC_Writer")
        .add_property("blockdelete", &NGC_Writer::get_blockdelete, &NGC_Writer::set_blockdelete)
    ;
    bp::class_< PySEG_Writer, bp::bases<Writer> >("SEG_Writer")
        .def( "get_loops", &PySEG_Writer::py_get_loops)
        .def( "get_segments", &PySEG_Writer::py_get_loops) // for backwards compatibility. remove at some point!
     ;

}
