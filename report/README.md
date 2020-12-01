# hw3 report

|Field|Value|
|-:|:-|
|Name|鄭又愷|
|ID|0616330|

## How much time did you spend on this project

研究怎麼用純 C++ 寫，花了大概 2 - 3 小時

剩下大概花了 20 小時

## Project overview

```
.
├── include
│   ├── AST
│   │   ├── AstDumper.hpp
│   │   ├── ast.hpp
│   │   ├── base
│   │   │   ├── ExpressionBase.hpp
│   │   │   └── StatementBase.hpp
│   │   ├── decl.hpp
│   │   ├── expression
│   │   │   ├── BinaryOperator.hpp
│   │   │   ├── ConstantValue.hpp
│   │   │   ├── FunctionInvocation.hpp
│   │   │   ├── UnaryOperator.hpp
│   │   │   └── VariableReference.hpp
│   │   ├── function.hpp
│   │   ├── program.hpp
│   │   ├── statement
│   │   │   ├── assignment.hpp
│   │   │   ├── CompoundStatement.hpp
│   │   │   ├── for.hpp
│   │   │   ├── FunctionCall.hpp
│   │   │   ├── if.hpp
│   │   │   ├── print.hpp
│   │   │   ├── read.hpp
│   │   │   ├── return.hpp
│   │   │   └── while.hpp
│   │   └── variable.hpp
│   ├── driver
│   │   └── driver.hpp
│   ├── type
│   │   ├── array.hpp
│   │   ├── base.hpp
│   │   ├── scalar.hpp
│   │   └── void.hpp
│   └── visitor
│       └── AstNodeVisitor.hpp
├── lib
│   ├── AST
│   │   ├── ast.cpp
│   │   ├── AstDumper.cpp
│   │   ├── base
│   │   │   ├── ExpressionBase.cpp
│   │   │   └── StatementBase.cpp
│   │   ├── decl.cpp
│   │   ├── expression
│   │   │   ├── BinaryOperator.cpp
│   │   │   ├── ConstantValue.cpp
│   │   │   ├── FunctionInvocation.cpp
│   │   │   ├── UnaryOperator.cpp
│   │   │   └── VariableReference.cpp
│   │   ├── function.cpp
│   │   ├── program.cpp
│   │   ├── statement
│   │   │   ├── assignment.cpp
│   │   │   ├── CompoundStatement.cpp
│   │   │   ├── for.cpp
│   │   │   ├── FunctionCall.cpp
│   │   │   ├── if.cpp
│   │   │   ├── print.cpp
│   │   │   ├── read.cpp
│   │   │   ├── return.cpp
│   │   │   └── while.cpp
│   │   └── variable.cpp
│   ├── driver
│   │   └── driver.cpp
│   ├── type
│   │   ├── array.cpp
│   │   ├── base.cpp
│   │   ├── scalar.cpp
│   │   └── void.cpp
│   └── visitor
│       └── AstNodeVisitor.cpp
├── Makefile
├── parser.yy
└── scanner.ll
```

最核心的檔案是 lib/driver/driver.cpp，driver 會封裝 Scannar 與 Parser 的 Context，像是 token location、string buffer、AST、parsing entrypoint 等。

在 parser.yy 中的 main 會呼叫 `drv.parse(argv[1])` 開始對檔案進行處理。

```cpp
int driver::parse(const std::string &f) {
    file = f;
    location.initialize(&file);
    scan_begin();
    yy::parser parser(*this);
    int res = parser.parse();
    scan_end();
    return res;
}
```

因為在 yacc 啟用了 type variants，所以 %type 定義的型別可以不必要是 Plain Old Data，可以直接使用 STL 的物件，例如：

```
%type <std::vector<std::shared_ptr<DeclNode>>> Declarations;
```

Scannar 需要修改的部份，首先需要定義在 scannar 回傳的資訊的型別，在這邊也可以使用 STL 物件，例如：

```
%token <std::string> ID "id"
```

在 return 時也需要將先前的 MACRO 換成回傳 symbol_type 物件，可以透過 `yy::parser::make_XXX` 來做到，以 identifier 為例：

```
[a-zA-Z][a-zA-Z0-9]* {
    TOKEN_STRING(id, yytext);
    return yy::parser::make_ID(yytext, loc);
}
```

在定義好需要的 Terminal 與 Nonterminal 的型別後，就可以開始撰寫 semantic rules，在得到所有資訊後，即可建成一棵 AST：

```
Program:
    ProgramName SEMICOLON
    /* ProgramBody */
    DeclarationList FunctionList CompoundStatement
    /* End of ProgramBody */
    END {
        drv.root = std::make_shared<ProgramNode>(@1.begin.line, @1.begin.column, $1, "void", $3, $4, $5);
    }
;
```

接著就繼續補全 AST Library 中的 class 資訊就完成這次的 Project 了。

### Class Structure

為了要將 expression 與 statement 抽象化，我把兩種類型抽象化成 ExpressionBase 與 StatementBase，Expression 會繼承 ExpressionBase，Statement 會繼承 StatementBase，方便在 return 時使用 dynamic_pointer_cast 轉換成同一種介面。

型別的定義在 type/ 目錄下，共有 ArrayType、ScalarType、VoidType 三種，這些 class 會繼承相同的介面 BaseType，提供函數 `getTypeName`。

### AstDumper

AstNode 會有兩個 virtual function：

```cpp
virtual void dump(AstDumper &dp) = 0;
virtual void dumpChildNodes(AstDumper &dp) = 0;
```

在各個 Node 會 override 這兩個 function：

```cpp
void dump(AstDumper &dp) override;
void dumpChildNodes(AstDumper &dp) override;
```

dump 會使用 AstDumper 的 visit 將 this 帶入參數以執行正確的函數，以 IfNode 為例：

```cpp
void IfNode::dump(AstDumper &dp) {
    dp.visit(*this);
}
```

dumpChildNodes 則會定義各個 node 在輸出 ChildNodes 時的順序

```cpp
void IfNode::dumpChildNodes(AstDumper &dp) {
    condition->dump(dp);
    body->dump(dp);
    if (body_else != nullptr) {
        body_else->dump(dp);
    }
}
```

在 parsing 完成 (AST 同時也建立完成) 後，若參數有加 `--dump-ast` 即可以 pre-order 遍歷整棵 AST 並輸出各個節點的資訊。

```cpp
if (argc >= 3 && strcmp(argv[2], "--dump-ast") == 0) {
    AstDumper dp;
    drv.root->dump(dp);
}
```

## What is the hardest you think in this project

因為覺得要用 C++ 就全部都統一都用 C++ 寫會比較好，所以就花了一點時間研究 C++ 版本的 Bison 怎麼寫。

最後整份作業感覺也比較有架構，除了整份 code 看起來都像是 C++，不會有混了一些 C 的情況 (像是 %union 裡面還要放 STL 物件的指標)；OOP 與 Visitor Pattern 也讓程式較易閱讀；除此之外，用 C++ 11 的一些實用功能像是 `shared_ptr` 也能讓指標操作比較安心一點。

## Feedback to T.A.s

> Please help us improve our assignment, thanks.
