#include "driver/driver.hpp"
#include "parser.hh"

driver::driver() {
}

driver::~driver() {
}

int driver::parse(const std::string &f) {
    file = f;
    location.initialize(&file);
    scan_begin();
    yy::parser parser(*this);
    int res = parser.parse();
    scan_end();
    return res;
}

void driver::error(const yy::location &l, const std::string &m, const std::string yytext) {
    std::cerr << "\n"
                 "|--------------------------------------------------------------------------\n"
                 "| Error found in Line #" << l.begin.line << ": " << buf << "\n"
                 "|\n"
                 "| Unmatched token: " << yytext << "\n"
                 "|--------------------------------------------------------------------------\n";
    exit(EXIT_FAILURE);
}
