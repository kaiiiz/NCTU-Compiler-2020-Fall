#include <algorithm>

#include "driver/driver.hpp"
#include "parser.hh"

driver::driver() {
}

driver::~driver() {
}

void driver::parse_cmd_args(int argc, const char *argv[]) {
    auto print_usage_and_exit = [](){
        fprintf(stderr, "Usage: ./compiler <filename> --save-path <save_path> [--dump-ast]\n");
        exit(-1);
    };

    if (argc < 4) {
        print_usage_and_exit();
    }
    // parse file name from path
    in_file_path = argv[1];
    in_file_name = in_file_path.substr(in_file_path.rfind("/") + 1);

    const char **opt_begin = argv + 2;
    const char **opt_end = argv + argc;
    const char **arg_pos;
    auto cmd_exist = [&arg_pos, &opt_begin, &opt_end](const std::string &opt_str) {
        arg_pos = std::find(opt_begin, opt_end, opt_str);
        return arg_pos != opt_end;
    };

    if (!cmd_exist("--save-path")) {
        print_usage_and_exit();
    } else if (arg_pos + 1 == opt_end) {
        print_usage_and_exit();
    } else {
        save_path = *(arg_pos + 1);
        // remove extension from in_file_name
        auto dot = in_file_name.rfind(".p");
        std::string in_file_name_without_ext;
        if (dot != std::string::npos && dot == in_file_name.length() - 2) {
            in_file_name_without_ext = in_file_name.substr(0, dot);
        } else {
            in_file_name_without_ext = in_file_name;
        }
        // concat save_path and file_name
        if (*(save_path.end() - 1) != '/') {
            out_file_path = save_path + "/" + in_file_name_without_ext + ".S";
        } else {
            out_file_path = save_path + in_file_name_without_ext + ".S";
        }
    }

    if (cmd_exist("--dump-ast")) {
        opt_dump_ast = 1;
    }
}

int driver::parse(const std::string &f) {
    file = f;
    location.initialize(&file);
    line_head.push_back(0);
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
