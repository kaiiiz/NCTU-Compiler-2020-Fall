#include <string>
#include <memory>

#include "type/void.hpp"

VoidType::VoidType() {}

VoidType::~VoidType() {}

std::string VoidType::getTypeName() { return "void"; }
