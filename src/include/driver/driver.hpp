#ifndef __DRIVER_HPP
#define __DRIVER_HPP
#include <string>
#include <cstdint>
#include <memory>

#include "parser.hh"
#include "sema/SymbolManager.hpp"

#define YY_DECL yy::parser::symbol_type yylex(driver& drv)

YY_DECL;

class AstNode;

class driver {
   public:
    driver();
    virtual ~driver();

    // Scannar Context
    void scan_begin();
    void scan_list(std::string str);
    void clear_buf();
    void scan_end();
    yy::location location; // The token's location used by the scanner.
    std::string buf;
    int32_t opt_src = 1;
    int32_t opt_tok = 1;

    // Parser Context
    int parse(const std::string &f);
    std::string file; // file name
    std::shared_ptr<AstNode> root;
    void error(const yy::location &l, const std::string &m, const std::string yytext);

    // Semantic Check Context
    SymbolManager symbol_mgr;
};

#endif  // __DRIVER_HPP
