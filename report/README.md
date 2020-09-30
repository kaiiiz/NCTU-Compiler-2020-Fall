# hw1 report

|Field|Value|
|-:|:-|
|Name|鄭又愷|
|ID|0616330|

## How much time did you spend on this project

大約 6 小時

## Project overview

```
.
└── src
    ├── Makefile
    └── scanner.l
```

檔案的部份我只改了 scanner.l。實作就是把 spec 定義的 regex 用 definition 定義好以便之後重複使用。

C code 基本上只有 string 因為需要將字串提取出來所以寫了一點點，其他目前大部分 pattern 都僅使用 MACRO 將 `yytext` 加入 `buffer`。

除了 `INITIAL` State，另外加了兩個 State: `COMMENT_BLK`、`COMMENT_INL` 處理註解的問題。

## What is the hardest you think in this project

我沒注意到 definition 中的 pattern 後面不能放註解，像是：

```
float_frac      {digit}*{nonzero_digit}|0 /* ... */
```

浪費很多時間在找 bug XDD

## Feedback to T.A.s

> Please help us improve our assignment, thanks.
