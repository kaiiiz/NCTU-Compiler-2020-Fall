#include <string>

#include "type/scalar.hpp"

ScalarType::ScalarType(std::string name) : BaseType{name} {}

ScalarType::~ScalarType() {}

std::string ScalarType::getTypeName() { return name; }
