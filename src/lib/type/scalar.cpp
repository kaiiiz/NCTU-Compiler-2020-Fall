#include <string>

#include "type/base.hpp"
#include "type/scalar.hpp"

ScalarType::ScalarType(scalar_type_t t) : type(t) {}

ScalarType::~ScalarType() {}

std::string ScalarType::getTypeName() { return getScalarTypeName(); }

scalar_type_t ScalarType::getType() { return type; }

std::string ScalarType::getScalarTypeName() { 
    switch (type) {
        case scalar_type_t::integer:
            return "integer";
        case scalar_type_t::string:
            return "string";
        case scalar_type_t::boolean:
            return "boolean";
        case scalar_type_t::real:
            return "real";
    }
    return "";
}
