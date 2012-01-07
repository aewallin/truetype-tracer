// experimental c++ port of truetype-tracer
// January 2012, anders.e.e.wallin "at" gmail.com
    
#include <boost/python.hpp>
namespace bp = boost::python;

#include "ttt.hpp"
#include "writer.hpp"
#include "ngc_writer.hpp"
#include "dxf_writer.hpp"
#include "segment_writer.hpp"

// this is the default font used if not specified on commandline
#define TTFONT "/usr/share/fonts/truetype/freefont/FreeSerifBoldItalic.ttf"


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
    
std::string version() {
    return "TTT TrueType tracer, c++ port!";
}

static std::string ttt_with_writer(std::string str, Writer* wr) {
    //Writer* wr = new NGC_Writer( TTFONT, str, false, 0.003, false );
    if (str=="")
        str = "Hello world.";
    wr->set_text(str);
    wr->set_font(TTFONT);
    Ttt* ttt = new Ttt(wr, str, false, TTFONT);
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
    
    bp::class_<Writer>("Writer")
        .add_property("arc", &Writer::get_arc, &Writer::set_arc)
        .add_property("conic", &Writer::get_conic, &Writer::set_conic)
        .add_property("cubic", &Writer::get_cubic, &Writer::set_cubic)
        .add_property("scale", &Writer::get_scale, &Writer::set_scale)
        .add_property("extents", &Writer::get_extents)
    ;
    bp::class_< NGC_Writer, bp::bases<Writer> >("NGC_Writer")
        .add_property("blockdelete", &NGC_Writer::get_blockdelete, &NGC_Writer::set_blockdelete)
        
    ;
    bp::class_< SEG_Writer, bp::bases<Writer> >("SEG_Writer")
        .def( "get_segments", &SEG_Writer::get_segments)
    ;
    
    /*
    bp::class_< VoronoiDiagram_py, bp::bases<VoronoiDiagram> >("VoronoiDiagram")
        .def(bp::init<double, unsigned int>())
        .def("addVertexSite",  &VoronoiDiagram_py::insert_point_site1 )
        .def("addVertexSite",  &VoronoiDiagram_py::insert_point_site2 )
        .def("addLineSite",  &VoronoiDiagram_py::insert_line_site2 ) // takes one argument
        .def("addLineSite",  &VoronoiDiagram_py::insert_line_site3 ) // takes two arguments
        .def("getGenerators",  &VoronoiDiagram_py::getGenerators)
        .def("getEdgesGenerators",  &VoronoiDiagram_py::getEdgesGenerators)
        .def("getVoronoiVertices",  &VoronoiDiagram_py::getVoronoiVertices) 
        .def("getFarVoronoiVertices",  &VoronoiDiagram_py::getFarVoronoiVertices)
        .def("getFarRadius",  &VoronoiDiagram_py::get_far_radius)
        .def("getVoronoiEdges",  &VoronoiDiagram_py::getVoronoiEdges)
        .def("getVoronoiEdgesOffset",  &VoronoiDiagram_py::getVoronoiEdgesOffset)
        .def("numPointSites", &VoronoiDiagram_py::num_point_sites)
        .def("numLineSites", &VoronoiDiagram_py::num_line_sites)
        .def("numVertices", &VoronoiDiagram_py::num_vertices)
        .def("numSplitVertices", &VoronoiDiagram_py::num_split_vertices)
        .def("__str__", &VoronoiDiagram_py::print)
        .def("version", &VoronoiDiagram_py::version)
        .def("reset_vertex_count", &VoronoiDiagram_py::reset_vertex_count)
        .def("setEdgePoints", &VoronoiDiagram_py::set_edge_points)
        .def("setEdgeOffset", &VoronoiDiagram_py::set_null_edge_offset)
        .def("debug_on", &VoronoiDiagram_py::debug_on)
        .def("check", &VoronoiDiagram_py::check)
        .staticmethod("reset_vertex_count")
        .def("getStat", &VoronoiDiagram_py::getStat)
        .def("getFaceStats", &VoronoiDiagram_py::getFaceStats)
    ;
    */
}
