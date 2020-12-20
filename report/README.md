# hw4 report

|Field|Value|
|-:|:-|
|Name|鄭又愷|
|ID|0616330|

## How much time did you spend on this project

大約 30 小時

## Project overview

這次的作業我基於上次 HW3 的 code 繼續寫下去，在 driver 中新增了：`bool opt_dump_symtab = 1;` 提供 dump symbol table 的開關，`std::vector<long> line_head;` 每行 source code 對應的 file offset，方便之後在 error 時讀取對應的位置，`SymbolManager symbol_mgr;` 用來管理 Symbol Table。

> 紀錄 line_head 透過在 lexer 中遇到 \n 就執行 `drv.record_line_head(drv.buf.length() + 1);` 達到

SymbolManager 提供 pushScope、popScope、currentSymTab、dumpSymTab 函式，用來在 program context 切換時管理當前的 Symbol Table，並在建構時會初始化一個 `global` 的 symbolTable。

SymbolTable 提供 insert、lookup、addChild、dump 函式，並保存目前是在第幾層的 symbol table 的資訊與維護 program context 資訊(種類與回傳值)，同時使用指標指向上一層與下一層的 SymbolTable，在輸出時可以比較好操作，最後使用 `vector<std::string>` 管理 symbol 的順序，並使用 unordered_map 存取 symbol 的 instance。

SymbolEntry 保存 source code 的位址、symbol名稱、型別與種類，並提供 pure virtual function `getAttributeStr` 供 child class 做繼承與覆寫。

程式的進入點如下：

```cpp
int main(int argc, const char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./parser <filename> [--dump-ast]\n");
        exit(-1);
    }

    driver drv;
    drv.parse(argv[1]);

    if (argc >= 3 && strcmp(argv[2], "--dump-ast") == 0) {
        AstDumper dp;
        drv.root->accept(dp);
    }
    SemanticAnalyzer analyzer(drv.symbol_mgr, drv.line_head, argv[1]);
    drv.root->accept(analyzer);
    if (drv.opt_dump_symtab) {
        drv.symbol_mgr.dumpSymTab();
    }

    if (!analyzer.hasError()) {
        printf("\n"
            "|---------------------------------------------------|\n"
            "|  There is no syntactic error and semantic error!  |\n"
            "|---------------------------------------------------|\n");
    }

    return 0;
}
```

在 parse 完成後會初始化 analyzer，將 SymbolManager、每行的 offset、source code 檔案名稱帶入進行初始化。接著用 analyzer 遍歷整個 AST。遍歷的過程中會同時建立 symbol table 並進行 Semantic Check 並輸出錯誤。

在錯誤的管理部份，我抽出來寫了一個 `SemaErrHandler` 並使用 MACRO 大幅減少有關輸出到 stderr 這部份程式的重複出現。

```cpp
#define PERR(loc, fmt, ...) \
    fprintf(stderr, "<Error> Found in line %u, column %u: " fmt "    %s\n    %s\n", \
            loc.line, loc.col, \
            ##__VA_ARGS__, \
            getSourceLine(loc.line).c_str(), \
            getErrIndicator(loc.col).c_str())
```

除此之外，我使用 `recordError(loc)` 來紀錄錯誤出現的位置到 `std::unordered_map<long, std::vector<long>> error_list;`。這個 unordered_map 用來管理第 lineno 行對應的錯誤 column，在遇到 child 已經出現錯誤可以省略接下來的檢查的邏輯，可以透過這個資料結構提供的資訊與 `hasErrorAt(loc)` 來實作。

有關型別的 instance，我實作了一個 `TypeManager`，裡面使用 `std::unordered_map<std::string, std::shared_ptr<TypeStruct>>` 來維護相同結構的 `TypeStruct`，節省記憶體浪費。

有關 expression 的型別資訊，在 `ExpressionBase` 透過 `fillAttribute` 函式在當前的 node 進行 Semantic Check 確認無誤後填入，這份資訊的回填可以使 parent node 獲得 child node 的型別資訊。

`SemanticAnalyzer` 的實作，就照著 spec 給的定義一部一部實作就完成這次作業了。

## What is the hardest you think in this project

在存 ConstantNode 相關的資訊時，原本因為一直想存 node 的真實值 (bool, int, string, real)，而卡在要用 union 還是用繼承，但 union 不能存 std::string 所以頭有點痛，後來用繼承的方式，在需要的時候 dynamic_pointer_cast 成想要的 class，但寫完後發現兩種寫法都滿醜的。

後來才想到其實可以這些資訊可以在建 symbol 時存 string 就好，因為型別已經確定了，等到真的要用到的時候再 `stoi`、`stod` 轉一下就好，修改完後程式應該有變得比較好懂一點了，上次就有遇到這個問題了，這次想解決這個問題所以想了滿久，所以應該算是這次作業卡最久的地方。

## Feedback to T.A.s

> Please help us improve our assignment, thanks.
