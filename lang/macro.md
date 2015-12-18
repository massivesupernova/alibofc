
# Macro

## define, undef

```c
#define identifier 
#define identifier replacement-list
#define identifier(parameters) replacement-list
#define identifier(parameters, ...) replacement-list
#define identifier(...) replacement-list
#undef identifier
```

## condition directives
```c
#if/#ifdef/#ifndef expression [1] must exist
// code block
#elif expression              [2] none or more
// code block
#else                         [3] none or one
// code block
#endif                        [4] must exist

// valid combination
[1][4]:        #if #endif
[1][3][4]:     #if #else #endif
[1][2+][3][4]: #if #elif #elif #else #endif
[1][2+][4]:    #if #elif #elif #endif

// #if #elif can use logic operators !, && and ||
#if !defined(ABCD) && (EFGH == 0 || !IJKL || defined(MNOP))
#endif
```

\#if, #elif

> The expression is a constant expression, using only literals and identifiers, defined using #define directive. 
  Any identifier, which is not literal, non defined using #define directive, evaluates to 0.
> The expression may contain unary operators in form `defined identifier` or `defined(identifier)` which return 1 
  if the identifier was defined using #define directive and 0 otherwise.  
  If any used identifier is not a constant, it is replaced with 0.

这两个预处理命令后面的表达式必须是常量表达式，只有字面常量和#define定义的名称可以用在这个表达式中。
如果出现的名称不是#define定义的名称，这个名称会被解析成0；如果名称在宏替换后不是常量，也会替换成0（编译器可能会报错）。
这两个预处理命令可以使用defined操作符，例如defined identifier或defined(identifier)，含义和#ifdef一样。

\#ifdef, #ifndef

> Checks if the identifier was defined using #define directive.  
> `#ifdef identifier` is essentially equivalent to `#if defined(identifier)`.  
> `#ifndef identifier` is essentially equivalent to `#if !defined(identifier)`.  

检查对应的名称是否已经定义或没有定义，如果为真，这个名称应该是由#define定义过的名称，并且没有被#undef。
另外#ifdef identifier与#if defined(identifier)等价，#ifndef identifier与#if !defined(identifier)等价。

## line and filename
```c
#line lineno
#line lineno "filename"
```

## error
```c
// error_message can consist of several words not necessarily in quotes
#error error_message 
```

## predefinitions
```c

```
