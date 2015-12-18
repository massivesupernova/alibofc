
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
#if expression
#if defined(expression)
#ifdef expression
#ifndef expression
// code block
#elif expression
// code block
#elif expression
// code block ...
#else
// code block
#endif

// #if can use logic operators !, && and ||
#if !defined(ABCD) && (EFGH == 0 || !IJKL)
#endif
```

\#if, #elif

> The expression is a constant expression, using only literals and identifiers, defined using #define directive. 
  Any identifier, which is not literal, non defined using #define directive, evaluates to 0.
> The expression may contain unary operators in form defined identifier or defined (identifier) which return 1 
  if the identifier was defined using #define directive and 0 otherwise. If the expression evaluates to nonzero value, 
  the controlled code block is included and skipped otherwise. If any used identifier is not a constant, it is replaced with ​0​.

\#ifdef, #ifndef

> Checks if the identifier was defined using #define directive.  
> \#ifdef identifier is essentially equivalent to #if defined( identifier).  
> \#ifndef identifier is essentially equivalent to #if !defined( identifier).  

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
