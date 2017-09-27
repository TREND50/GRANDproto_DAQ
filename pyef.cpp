#include <boost/ref.hpp>
#include <boost/utility.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/def.hpp>
#include <boost/python/pure_virtual.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/call.hpp>
#include <boost/python/call_method.hpp>
#include <boost/python/manage_new_object.hpp>
#include <boost/python/return_opaque_pointer.hpp>
#include <boost/operators.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/overloads.hpp>
#include <vector>
#include <mutex>
#include <fstream>


using namespace boost::python;
using namespace boost;

#include "data_file.hpp"

BOOST_PYTHON_MODULE (native)
{
    class_<std::vector<std::uint32_t>> ("vec_u32", boost::python::init<size_t> ())
    .def (boost::python::init<>())
    .def (vector_indexing_suite<std::vector<std::uint32_t>> ())
    .def ("append", (void (std::vector<std::uint32_t>::*) (const std::uint32_t &)) & std::vector<std::uint32_t>::push_back);

    class_<std::vector<std::uint16_t>> ("vec_u16", boost::python::init<size_t> ())
    .def (boost::python::init<>())
    .def (vector_indexing_suite<std::vector<std::uint16_t>> ())
    .def ("append", (void (std::vector<std::uint16_t>::*) (const std::uint16_t &)) & std::vector<std::uint16_t>::push_back);

    class_<std::vector<local_station_t>> ("vec_ls", boost::python::init<size_t> ())
    .def (boost::python::init<>())
    .def (vector_indexing_suite<std::vector<local_station_t>> ())
    .def ("append", (void (std::vector<local_station_t>::*) (const local_station_t &)) & std::vector<local_station_t>::push_back);

    class_<std::vector<event_t>> ("vec_event", boost::python::init<size_t> ())
    .def (boost::python::init<>())
    .def (vector_indexing_suite<std::vector<event_t>> ())
    .def ("append", (void (std::vector<event_t>::*) (const event_t &)) & std::vector<event_t>::push_back);


    class_<file_header_t>("file_header", boost::python::init<>())
    .def("length", &file_header_t::length)
    .def("runnr", &file_header_t::runnr)
    .def("run_mode", &file_header_t::run_mode)
    .def("serial", &file_header_t::run_mode)
    .def("first_event", &file_header_t::first_event)
    .def("first_event_sec", &file_header_t::first_event_sec)
    .def("last_event", &file_header_t::last_event)
    .def("last_event_sec", &file_header_t::last_event_sec)
    .def("additional_header", &file_header_t::get_additional_header)
    ;

    class_<event_header_t>("event_header", boost::python::init<>())
    .def_readonly("header_length", &event_header_t::header_length)
    .def_readonly("runnr", &event_header_t::runnr)
    .def_readonly("eventnr",&event_header_t::eventnr)
    .def_readonly("t3eventnr",&event_header_t::t3eventnr)
    .def_readonly("first_ls",&event_header_t::first_ls)
    .def_readonly("event_sec",&event_header_t::event_sec)
    .def_readonly("event_nsec",&event_header_t::event_nsec)
    .def_readonly("event_type",&event_header_t::event_type)
    .def_readonly("event_vers",&event_header_t::event_vers)
    .def_readonly("ad1",&event_header_t::ad1)
    .def_readonly("ad2",&event_header_t::ad2)
    .def_readonly("ls_cnt",&event_header_t::ls_cnt)
    ;

    class_<local_station_header_t>("local_station_header", boost::python::init<>())
    .def_readonly("length", &local_station_header_t::length)
    .def_readonly("event_nr", &local_station_header_t::event_nr)
    .def_readonly("ls_id", &local_station_header_t::ls_id)
    .def_readonly("header_length", &local_station_header_t::header_length)
    .def_readonly("GPSseconds", &local_station_header_t::GPSseconds)
    .def_readonly("GPSnanoseconds", &local_station_header_t::GPSnanoseconds)
    .def_readonly("trigger_flag", &local_station_header_t::trigger_flag)
    .def_readonly("trigger_pos", &local_station_header_t::trigger_pos)
    .def_readonly("sampling_freq", &local_station_header_t::sampling_freq)
    .def_readonly("channel_mask", &local_station_header_t::channel_mask)
    .def_readonly("ADC_resolution", &local_station_header_t::ADC_resolution)
    .def_readonly("trace_length", &local_station_header_t::trace_length)
    .def_readonly("version", &local_station_header_t::version)
    ;    

    class_<local_station_t>("local_station", boost::python::init<>())
    .def_readonly("header", &local_station_t::header)
    .def_readonly("header_data", &local_station_t::header_data)
    .def_readonly("adc_buffer", &local_station_t::adc_buffer)
    ;

    class_<event_t>("event", boost::python::init<>())
    .def_readonly("header", &event_t::header)
    .def_readonly("local_station_list", &event_t::local_station_list)
    ;

    class_<event_file>("event_file", boost::python::init<>())
    .def_readonly("header", &event_file::header)
    .def_readonly("event_list", &event_file::event_list)
    ;

    def("read_event_file", read_event_file);
}
