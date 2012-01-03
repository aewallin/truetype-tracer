// experimental c++ port of truetype-tracer
// January 2012, anders.e.e.wallin "at" gmail.com
    


#include <boost/python.hpp>
namespace bp = boost::python;

#include "ttt.hpp"

std::string version() {
    return "TTT TrueType tracer, c++ port!";
}

BOOST_PYTHON_MODULE(ttt) {

    bp::def("version", version);
    
    bp::class_< Ttt >("ttt")

    ;
}
