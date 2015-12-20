
# C语言宏

## define和undef

```c
#define identifier 
#define identifier replacement-list
#define identifier(parameters) replacement-list
#define identifier(parameters, ...) replacement-list
#define identifier(...) replacement-list
#undef identifier
```

宏可以使用...传入不定个数参数，...只能作为最后一个形参。
在Visual Studio 2013上测试，只能在...形参处传入__VA_ARGS__。
宏如果扩展成多条语句，为了避免出错，应该用do { ... } while (0)包裹起来。
宏的每个参数，为了避免副作用，应该都只使用一次，且每个参数都用小括号括起。

stringification #

In function-like macros, a # operator before an identifier in the replacement-list 
runs the identifier through parameter replacement and encloses the result in quotes, 
effectively creating a string literal. 
In addition, the preprocessor adds backslashes to escape the quotes surrounding embedded string literals, if any, 
and doubles the backslashes within the string as necessary. 
All leading and trailing whitespace is removed, and any sequence of whitespace in the middle of the text 
(but not inside embedded string literals) is collapsed to a single space. 
This operation is called "stringification". 
If the result of stringification is not a valid string literal, the behavior is undefined.

```c
#define showlist(...) puts(#__VA_ARGS__)
showlist(1, "x", int); // expands to puts("1, \"x\", int")
```

token-pasting ##

A ## operator between any two successive identifiers in the replacement-list 
runs parameter replacement on the two identifiers and then concatenates the result. 
This operation is called "concatenation" or "token pasting". 
Only tokens that form a valid token together may be pasted: 
identifiers that form a longer identifier, digits that form a number, or operators + and = that form a +=. 
A comment cannot be created by pasting / and * because comments are removed from text before macro substitution is considered.
If the result of concatenation is not a valid token, the behavior is undefined.

Note: some compilers offer an extension that allows ## to appear after a comma and before __VA_ARGS__, 
in which case the ## does nothing when __VA_ARGS__ is non-empty, but removes the comma when __VA_ARGS__ is empty: 
this makes it possible to define macros such as fprintf (stderr, format, ##__VA_ARGS__).

The problem is that when you have a macro replacement, 
the preprocessor will only expand the macros recursively if neither the stringizing operator # nor the token-pasting operator ## are applied to it. So, you have to use some extra layers of indirection, you can use the token-pasting operator with a recursively expanded argument.

6.10.3.1 Argument substitution

After the arguments for the invocation of a function-like macro have been identified, argument substitution takes place. 
A parameter in the replacement list, unless preceded by a # or ## preprocessing token or followed by a ## preprocessing token,
is replaced by the corresponding argument after all macros contained therein have been expanded. 
Before being substituted, each argument’s preprocessing tokens are completely macro replaced 
as if they formed the rest of the preprocessing file; no other preprocessing tokens are available.

If, in the replacement list of a function-like macro, 
a parameter is immediately preceded or followed by a ## preprocessing token, 
the parameter is replaced by the corresponding argument’s preprocessing token sequence.

For both object-like and function-like macro invocations, before the replacement list is reexamined for more macro names to replace, each instance of a ## preprocessing token in the replacement list (not from an argument) is deleted and the preceding preprocessing token is concatenated with the following preprocessing token.

```c
// the standard says that there must be at least one argument for the ellipsis of `...`

#define _FIRSTARG_HELPER(first, ...) first
#define _RESTARGS_OF_ONE1ARG(first) 
#define _RESTARGS_OF_MOREARG(first, ...) , __VA_ARGS__
#define _RESTARGS_HELPER(tail, ...) _RESTARGS_OF_##tail(__VA_ARGS__)
#define _RESTARGS_HELPER_EX(tail, ...) _RESTARGS_HELPER(tail, __VA_ARGS__)

#define _VARGS_MAX_16_ARGS(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac, ad, ae, af, ...) af
#define _VARGS_MORETOKS_14(m) m, m, m, m, m, m, m, m, m, m, m, m, m, m, 
#define _VARGS_ONE_OR_MORE(...) _VARGS_MAX_16_ARGS(__VA_ARGS__, _VARGS_MORETOKS_14(MOREARG), ONE1ARG, ellipsis)

#define VARGS_FIRSTARG(...) _FIRSTARG_HELPER(__VA_ARGS__, ellipsis)
#define VARGS_RESTARGS(...) _RESTARGS_HELPER_EX(_VARGS_ONE_OR_MORE(__VA_ARGS__), __VA_ARGS__)

1. VARGS_RESTARGS(1, 2, 3) 
2. _RESTARGS_HELPER_EX(_VARGS_ONE_OR_MORE(1, 2, 3), 1, 2, 3)
3. _RESTARGS_HELPER_EX(_VARGS_MAX_16_ARGS(1, 2, 3, _VARGS_MORETOKS_14(MOREARG), ONE1ARG, ellipsis), 1, 2, 3)
4. _RESTARGS_HELPER_EX(_VARGS_MAX_16_ARGS(1, 2, 3, MOREARG, MOREARG, ..., ONE1ARG, ellipsis), 1, 2, 3)
5. _RESTARGS_HELPER_EX(MOREARG, 1, 2, 3)
6. _RESTARGS_HELPER(MOREARG, 1, 2, 3)

#define VARGS_COUNT(...) _VARGS_COUNT_HELPER(__VA_ARGS__, _VARGS_ARGSEQ_NUM())
#define _VARGS_ARGSEQ_NUM() 3, 2, 1, 0
#define _VARGS_COUNT_HELPER(...) _VARGS_COUNT_NUM(__VA_ARGS__)
#define _VARGS_COUNT_NUM(_1, _2, _3, N, ...) N

VARGS_COUNT() 

1. VARGS_COUNT(1, 2, 3) 
2. _VARGS_COUNT_HELPER(1, 2, 3, 63, 62, ..., 0)
3. _VARGS_COUNT_NUM(1, 2, 3, 63, 62, ..., 0)
4. 3
```
I know I can do this:

```c
#define MACRO(api, ...) \
  bool ret = api(123, ##__VA_ARGS__);
```

This is just an example, it's part of a more complicated solution. The point is that I need to append the variable number of arguments to the first 123. The ## makes the compiler strip out the comma after the 123 argument if no arguments were passed into MACRO.

But now I want to append arguments to api, like so:

```c
#define MACRO(api, ...) \
  bool ret = api(__VA_ARGS__##, 456);
```

Nocando. One solution is to have two macros, MACRO and MACRO_V, say, and make the _V version not process any arguments. But is there a way to make it work with one macro?

Yes, you can. The following supports up to 4 arguments, but it can be trivially expanded to support more:

```
#define MACRO(api, ...) \
    bool ret = api(__VA_ARGS__ VA_COMMA(__VA_ARGS__) 456)

/*
 * VA_COMMA() expands to nothing if given no arguments and a comma if
 * given 1 to 4 arguments.  Bad things happen if given more than 4
 * arguments.  Don't do it.
 */
#define VA_COMMA(...) GET_6TH_ARG(,##__VA_ARGS__,COMMA,COMMA,COMMA,COMMA,)
#define GET_6TH_ARG(a1,a2,a3,a4,a5,a6,...) a6
#define COMMA ,

/* EXAMPLES */
MACRO(foo)                       /* bool ret = foo( 456)              */
MACRO(foo,1)                     /* bool ret = foo(1 , 456)           */
MACRO(foo,1,2,3,4)               /* bool ret = foo(1,2,3,4 , 456)     */
/* uh oh, too many arguments: */
MACRO(foo,1,2,3,4,5)             /* bool ret = foo(1,2,3,4,5 5 456)   */
MACRO(foo,1,2,3,4,5,6)           /* bool ret = foo(1,2,3,4,5,6 5 456) */
```



http://gcc.gnu.org/onlinedocs/gcc-4.5.1/gcc/Variadic-Macros.html#Variadic-Macros

gcc does support argument counting macros with zero arguments with the ## __VA_ARGS__ convention. The following works compiled with gcc:

```c
#include <stdio.h>

#define NARGS(...) __NARGS(0, ## __VA_ARGS__, 5,4,3,2,1,0)
#define __NARGS(_0,_1,_2,_3,_4,_5,N,...) N

int main()
{
  printf("%d\n", NARGS());     // prints 0
  printf("%d\n", NARGS(1));    // prints 1
  printf("%d\n", NARGS(1, 2)); // prints 2
  return 0;
}
```

Is there an equivalent for VisualC++ 2010 that will work with zero arguments macros? Non standard extensions or tricks accepted.

The following example works fine in VisualStudio 2010 and newer, gcc and clang with non standard extensions enabled. In Microsoft compilers it assumes the trailing comma in the AUGMENTER macro will be removed by the preprocessor when arguments count is zero. This is non standard and it has been also reported elsewere. In gcc and clang it uses the widely known ## __VA_ARGS__ non standard extension.

```c
#include <stdio.h>

#ifdef _MSC_VER // Microsoft compilers

#define EXPAND(x) x
#define __NARGS(_1, _2, _3, _4, _5, VAL, ...) VAL
#define NARGS_1(...) EXPAND(__NARGS(__VA_ARGS__, 4, 3, 2, 1, 0))

#define AUGMENTER(...) unused, __VA_ARGS__
#define NARGS(...) NARGS_1(AUGMENTER(__VA_ARGS__))

#else // Others

#define NARGS(...) __NARGS(0, ## __VA_ARGS__, 5,4,3,2,1,0)
#define __NARGS(_0,_1,_2,_3,_4,_5,N,...) N

#endif

int main()
{
  // NARGS
  printf("%d\n", NARGS());          // Prints 0
  printf("%d\n", NARGS(1));         // Prints 1
  printf("%d\n", NARGS(1, 2));      // Prints 2
  fflush(stdout);

#ifdef _MSC_VER
  // NARGS minus 1
  printf("\n");
  printf("%d\n", NARGS_1(1));       // Prints 0
  printf("%d\n", NARGS_1(1, 2));    // Prints 1
  printf("%d\n", NARGS_1(1, 2, 3)); // Prints 2
#endif

return 0;
}
```

Macros were tested with real compilers, Wandbox and Webcompiler

	 	
this version does not work properly with GCC. – BLUEPIXY Nov 1 '14 at 1:38
  	 	
@BLUEPIXY: I also wrote the relevant macro that works with GCC C/C++ compilers with GNU extensions enabled. – ceztko Dec 15 '14 at 11:17
  	 	
Nice: someone downvoted and didn't write why. These macros costed hours of work so I would be pleased to know if they don't work for someone. – ceztko Apr 25 at 15:35 
  	 	
woaaa! EXPAND(x) x seems to be the one thing that I was missing to get my argument counter macro working in Visual Studio. I don't understand why this doesn't work for non-gnu gcc/clang. Either way, I'm upvoting this answer for the visual studio solution which saved me hours of research. Thank you! – mchiasson Jun 4 at 14:51 
  	 	
EXPAND(x) x need is actually a bug in the VS2010 preprocessor that is fixed in the following releases. – ceztko Jun 5 at 21:45.

@mchiasson You may have a look at my complete answer here that comes with 2 versions that will respectively work with msvc and gcc/clang with extensions enabled. They can be easily ifdef-ed for wide compilers support. – ceztko Jun 4 at 14:18 

## 条件预处理命令
```c
#if #ifdef #ifndef expression [1] must exist
// code block
#elif expression              [2] none or more
// code block
#else                         [3] none or one
// code block
#endif                        [4] must exist

// valid combination
[1][4]:          #if #endif
[1][3][4]:       #if #else #endif
[1][2]...[3][4]: #if #elif ... #else #endif
[1][2]...[4]:    #if #elif ... #endif

// #if #elif can use logic operators !, && and ||
#if !defined(ABCD) && (EFGH == 0 || !IJKL || defined(MNOP))
#endif

#undef ABCD
#if defined(ABCD)  // false
#if !defined(ABCD) // true
#if (ABCD == 0)    // true - undefined macro evaluates to 0
#if (ABCD)         // fasle
#if (!ABCD)        // true - undefined macro evaluates to 0

const int num = 23;
#define NUM 23
#if (num)          // false - num is not a macro and not a literal, evaluates to 0
#if (num == 0)     // true  - num is not a macro and not a literal, evaluates to 0
#if (NUM)          // true  - NUM is a macro and can evaluate to a integer literal 23
#if (NUM == 0)     // false - NUM is a macro and can evaluate to a integer literal 23
```

\#if, #elif

> The expression is a constant expression, using only literals and identifiers, defined using #define directive. 
  Any identifier, which is not literal, non defined using #define directive, evaluates to 0.
> The expression may contain unary operators in form `defined identifier` or `defined(identifier)` which return 1 
  if the identifier was defined using #define directive and 0 otherwise.
  If any used identifier is not a constant, it is replaced with 0.

这两个预处理命令后面的表达式必须是常量表达式，只有字面常量和#define定义的名称可以用在这个表达式中。
如果出现的名称不是#define定义的名称，这个名称会被解析成0；如果名称在宏替换后不是字面常量，也会被解析成0。
这两个预处理命令可以使用defined操作符，例如defined identifier或defined(identifier)，含义和#ifdef一样。

\#ifdef, #ifndef

> Checks if the identifier was defined using #define directive.  
> `#ifdef identifier` is essentially equivalent to `#if defined(identifier)`.  
> `#ifndef identifier` is essentially equivalent to `#if !defined(identifier)`.  

检查对应的名称是否已经定义或没有定义，如果为真，这个名称应该是由#define定义过的名称，并且没有被#undef。
另外#ifdef identifier与#if defined(identifier)等价，#ifndef identifier与#if !defined(identifier)等价。

## 预处理命令line
```c
#line lineno
#line lineno "filename"

#line 123
assert(__LINE__ == 123);
```
> Changes the current preprocessor line number to lineno. 
  Occurrences of the macro \_\_LINE__ beyond this point will expand to lineno plus 
  the number of actual source code lines encountered since.
  It can also change the current preprocessor file name to filename. 
  Occurrences of the macro \_\_FILE__ beyond this point will produce filename.
  Note that: the line number following the directive `#line __LINE__` is implementation-defined.

> This directive is used by some automatic code generation tools which produce C source files from a file written in another language. In that case, #line directives may be inserted in the generated C file referencing line numbers and the file name of the original (human-editable) source file.

可以将当前预处理行号修改成lineno，这个命令之后的\_\_LINE__会以这个行号为基准来表示（这个行号是该命令下一行的行号）。
也可以将当前预处理文件名修改成filename，这个命令之后的\_\_FILE__会用这个文件名表示。
注意的是`#line __LINE__`与编译器实现相关，对应的行号可能是line命令所在行的行号或前一行的行号，应该避免使用。

这个预处理命令可以用在代码自动生成工具中，生成的代码会插入line命令用来表示当前生成的代码对应的源代码的行数。
在处理生成的代码时如果发生错误，处理工具可以将错误发生的地点对应到源代码中的行数，指示出源代码中的错误。

## 预处理命令error
```c
// error_message can consist of several words not necessarily in quotes
#error error_message 

#if MAX_BUFFER_SIZE < 1024
#error "max buffer size too small"
#endif
```

这个预处理命令用于引发一个编译错误，会使编译器停止编译并显示错误消息error_message。
这个error_message是错误信息字符串，可以不用双引号引起。其用法一般是与条件预处理命令一起使用。

## 预定义宏
```c
__STDC__ // the integer constant 1 if the compiler conforms the c standard
__FILE__ // a string literal of current file name
__LINE__ // an integer constant of current line number
__DATE__ // a string literal as form "Mmm dd yyyy"
__TIME__ // a string literal as form "hh:mm:ss"

// a long integer constant to denotes the version of C++ standard the compiler used: 
// 199711L(until C++11), 201103L(C++11), or 201402L(C++14) 
__cplusplus // note that c++ may has __STDC__ equal to 1 if the compiler also conforms the c standard
```
