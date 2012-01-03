// experimental c++ port of truetype-tracer
// January 2012, anders.e.e.wallin "at" gmail.com
    


#include <boost/python.hpp>
namespace bp = boost::python;

#include "ttt.hpp"

#include "ttt.hpp"
#include "writer.hpp"
#include "ngc_writer.hpp"



static std::string ngc1(std::string str) {
    Writer* wr = new NGC_Writer( TTFONT, str, false, 0.003, false );
    Ttt* ttt = new Ttt(wr, str);
    std::string outp = ttt->get_output();
    delete ttt;
    delete wr;
    return outp;
}

static std::string ngc0() {
    return ngc1("Hello World.");
}    
    
std::string version() {
    return "TTT TrueType tracer, c++ port!";
}

BOOST_PYTHON_MODULE(ttt) {

    bp::def("version", version);
    bp::def("ngc", ngc0); // zero arguments. "hello world" with all else defaults
    bp::def("ngc", ngc1);
    /*
    bp::class_< ttt , boost::noncopyable >("ttt")
        .def("get_output", &Ttt::get_output )
    ;*/
}
