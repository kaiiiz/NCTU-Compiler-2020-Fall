#include <string>

#include "type/base.hpp"
#include "type/scalar.hpp"

ScalarType::ScalarType(TypeKind t) : TypeBase(t) {}

ScalarType::~ScalarType() {}

std::string ScalarType::getTypeStr() { return getTypeKindStr(); }
