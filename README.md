# README

This is a tool to split a text novel to chapters. It depends on the regular expression to recogenize the chapter title. And the text file must be unix format, that means `\n` is the new line character instead of `\r\n`.

These regular expressions may useful:

* Chinese: `^ *第[零一二三四五六七八九十百千0123456789]*章 .*\n$`
* English: `^ *[cC][hH][aA][pP][tT][eE][rR] [0123456789]*.* \n$`
