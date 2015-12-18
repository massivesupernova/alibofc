
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
