#include <string>
#include <memory>

#include "type/void.hpp"

VoidType::VoidType() : TypeBase(TypeKind::void_) {}

VoidType::~VoidType() {}

std::string VoidType::getTypeStr() { return getTypeKindStr(); }
