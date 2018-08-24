# README

This is a tool to split a text novel to chapters. It depends on the regular expression to recogenize the chapter title.

```
chapter-split: used to split novel to chapters.
  args:
  -i: specify input file
  -o: specify output directory
      default ./results/, but it should exists before execute
  -r: specify title regular expression
      default "^ *第[零一二三四五六七八九十百千0123456789]*章 .*\n$"
      English novel "^ *[cC][hH][aA][pP][tT][eE][rR] *[0123456789]*.* \n$"
```
