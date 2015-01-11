#ifndef PYTHONBINDING_H
#define PYTHONBINDING_H
#include <boost/python.hpp>
#include "World.h"


using namespace boost::python;
char const* greet()
{
   return "hello, world";
}

BOOST_PYTHON_MODULE(DOT_Py)
{
    class_<Gradient>("Gradient", init<SNibble, UNibble, unsigned int, unsigned int, unsigned int>())
            .addproperty("Delta", &Gradient::Delta)
            .addproperty("LowFactor", &Gradient::LowFactor)
            .addproperty("Q1", &Gradient::Q1)
            .addproperty("Q3", &Gradient::Q3)
            .addproperty("Max", &Gradient::Max)
            ;
    class_<CostBase>("CostBase", init<unsigned int, unsigned int>())
            .addproperty("GlobalAdr", &CostBase::GlobalAdr)
            .addproperty("Value", &CostBase::Value)
            ;
    class_<AdvertisementBase>("AdvertisementBase", init<UNibble , UNibble , UNibble >())
            .def("AddPositiveEffect", &AdvertisementBase::AddPositiveEffect)
            .def("AddNegativeEffect", &AdvertisementBase::AddNegativeEffect)
            ;
    class_<IEntityGroup>("IEntityGroup", init<>())
            .def("AddAttribute",  &IEntityGroup::AddAttribute)
            .def("CreateNewInstance", &IEntityGroup::CreateNewInstance)
            .def("ReduceAdChain", &IEntityGroup::ReduceAdChain)
            .def("Plan", &IEntityGroup::Plan)
            .def("ReturnScore", &IEntityGroup::ReturnScore)
            .def("ComputeInference", &IEntityGroup::ComputeInference)
            .def("Prepare", &IEntityGroup::Prepare)
            .def("ExecuteQueue", &IEntityGroup::ExecuteQueue)
            .addproperty("BlockSize", &IEntityGroup::BlockSize)
            ;
    def("greet", greet);
}

#endif // PYTHONBINDING_H
