# hw2 report

|Field|Value|
|-:|:-|
|Name|鄭又愷|
|ID|0616330|

## How much time did you spend on this project

7 ~ 8hr

## Project overview

```
.
└── src
    ├── Makefile
    ├── parser.y
    └── scanner.l
```

這次的 Project 使用 yacc 進行語法的定義，新增的 yacc 檔案叫做 `parser.y`，這個檔案的內容就是透過前一次 project 拿到的 token 來定義 context free grammar。

編譯的部份，透過 `bison -d parser.y -o parser` 後可以產生 `parser.c`, `parser.h` 這兩個檔案。

`parser.h` 主要內容是在定義 yacc 檔案最上面 declarations 區域定義的 token 對應的數值，這個 header file 會提供給 scanner 使用，在 token 被 match 到後將回傳這些數值，也就是 yacc 中 yylex() 後會接收到的回傳值。

`parser.c` 就是 bison 透過 context free grammar 所生成的 LALR(1) parsing table、transition rule 等資料結構與狀態轉換邏輯。

> 透過對 bison 加上 `-v` 的參數，可以得到 `parser.output` 的檔案，這個檔案產生的 LALR(1) parsing table 與 transition rule 較易閱讀，且會對 shift/reduce 或是 reduce/reduce conflict 做指示，可以很快了解到是那條 grammar 有寫錯，對於除錯滿方便的。

透過將 flex 產生的 `scanner.c` 與 `parser.c` 一起透過 gcc 編譯後，即可得到 `scannar.o` 與 `parser.o` 的 object file。最後一起將這些 object file 透過 `-lfl -ly` 將 flex 與 yacc 的 library link 進來，即可得到 `parser` 的執行檔。

將檔案輸入進 parser 的執行檔後，就可以檢查輸入的檔案是否語法問題，若語法錯誤時會回報錯誤的行數。

## What is the hardest you think in this project

對 spec 中 integer_constant 的定義不太清楚，不確定到底是語法還是語義問題。

除此之外，因為我上次 lab 把相同類別 token 的 regex 都寫在一起，這次為了要拿到不同 token 的回傳值，要再將原先的 regex 拆開再回傳，所以花滿多時間在改寫 scannar。

## Feedback to T.A.s

如果可以將 Expressions 的那張表格改成：

https://en.cppreference.com/w/c/language/operator_precedence

這種格式說不定會更清楚(?)
