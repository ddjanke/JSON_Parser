#include <pybind11/pybind11.h>
#include "PhoneumJsonParser.h"

namespace py = pybind11;

//using namespace pybind11::literals;

PYBIND11_MODULE(json_parser, m) {
    py::class_<PhoneumJsonParser>(m, "PhoneumJsonParser")
        .def(py::init<const std::string &>())
        .def("parse", &PhoneumJsonParser::parse)
        .def_property("app", nullptr, &PhoneumJsonParser::set_app)
        .def_property_readonly("output_json", &PhoneumJsonParser::getOutputJson);
}
/*
<%
setup_pybind11(cfg)
cfg['include_dirs'] = ['/usr/include', '/usr/include/python3.6m']
cfg['sources']  = ['json_navigator.cpp', 'PhoneumJsonParser.cpp', 'phoneum_json_parser.cpp']
%>
*/