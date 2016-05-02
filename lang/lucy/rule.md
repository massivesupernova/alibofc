
- https://developer.apple.com/swift/

## Primary Types
```c
void
bool - true false
char/utf8/utf16/utf32 - unsigned type
byte/int8 - unsigned/signed 8-bit integer
uint/int - unsigned/signed integer at least with pointer-size
uptr      - uint
uint16 int16 - unsigned/signed 16-bit integer
uint32 int32 - unsigned/signed 32-bit integer
uint64 int64 - unsigned/signed 64-bit integer
float/double
string

postfix: 23ub  23b    8-bit
         23ui  23     machine word size
         23us  23s    16-bit
         23ul  23L    32-bit
         23ull 23LL   64-bit
         23f   23.0   float double

user defined postfix: operator'kg(int a) int      56'kg + 32'pd

int literal: 0b1101 0o775 0xFA 42   0b1101_1100 0xFFBB_FFFE 430_323_1234
char literal: ' ', 'Space', 'Tab', 'Enter', 'a', 'b', 'c', 42c, 0xF3c
unicode literal: \u0A11 (utf-16), \u0000_0A11 (utf-32), \uu7F (utf-8)

???如何尽量消除指针
```

## Standard Container
```c
var a = 3.0
var str = "double = {{a}}"

var a1 = [int]                   // empty array
var a2 = [int].(size=3,value=0)  // array with 3 elements
var a3 = [1, 2, 3]

var t1 = [int:string]            // empty table
var t2 = [int:string].(size=128) // table with 128 elements space
var t3 = [1:"a", 2:"b"]

var s1 = [|int]               // empty set
var s2 = [|int].(4)           // set with 5 elements space
var s3 = [|3, 6, 9]

func initIntArray(int size, value) void | size.elems.count.num { 
  /* ... */
}

// call function that has only one argument with string/array/table/set type:
Test.print(string s) void { /* ... */ }
Test.print([|int] a) void { /* ... */ }
Test.print([int] a) void { /* ... */ }
Test.print([int:string] t) void { /* ... */ }

var test = Test()
test.print"abcd"
test.print[|1, 2, 3]
test.print[23, 12,]
test.print[12:"a", 23:"b"]

var obj = new ClassTest()
var obj2 = new Child() as BaseClass
```

## User defined type
```c
1. 真正的值传递 - 只要赋值就会拷贝一份   // 如swift中的struct以及基本类型，struct只能实现接口不能继承
2. 优化的值传递 - 只有在修改时才拷贝     // 如swift中的string, array, table, set
3. 引用传递                              // 如swift中的class

常量不可修改，只能使用常量表达式在初始化时赋值一次；
常量可以赋值给变量，赋值后变量当前的值为这个常量值；
变量可以暂时赋予不可修改属性，在一定范围内限制对它的修改；
常量不应占用struct/class类空间和对象空间；
值类型赋给不可修改变量会发生一次拷贝；
引用类型对象赋给不可修改变量不发送拷贝，不能重新引用一个新的引用对象，也不能对引用对象进行修改
常量/计算属性可以使用在enum/struct/class中，存储属性只能使用在struct/class中
常量使用const和enum定义，变量使用var定义，不可修改变量使用immutable定义，还有两个修饰符是static和threadlocal
@[?]引用类型声明为const无需监控其占用内存，实现是可以将引用类型的内容保存在常量中，不使用保存对象地址的方式
@[?]但如果两个引用类型常量相同，一个常量仅保存指针指向另外一个常量

全局作用域 {
  // 常量
  const PI = 3.14
  enum Color {
    Red = 3, Yellow, Blue,
  }

  // 一般变量
  var a = 3.14      // global shared variable
  var _a = 6.28     // global variable only used in current file

  // 一旦初始化就不能修改的变量
  immutable b = a   // global shared variable
  immutable _b = b  // global immutable only used in current file

  // 线程存储变量
  threadlocal t = 0 // thread local storage variableS
}

struct/class作用域 {
  // 常量：不占用对象空间
  const PI = 3.14

  // 变量
  var a = 0        // public variable
  var _a = 0       // private variable, only available to current file
  var aa = int?    // 不进行初始化的变量

  // 不可修改变量
  immutable b = a  // public immutable
  immutable _b = b // private immutable, only available to current file

  // static变量: 不占用对象空间
  static var sa = 0         // public static variable
  static var _sa = 0        // private static variable, only available to current file
  static immutable sb = sa  // public static immutable
  static immutable _sb = sb // private static immutable, only available to current file
}

局部作用域之函数参数 {
  // 传引用还是传值由类型决定，不需要额外的关键字说明
  (int a, b, c, var double d) 默认是immutable的，使用var可以指定对应变量可以修改

  // immutable 值类型：不会修改外部传入值，相当于C里面的声明(int a)但a的值不能修改
  // variable 值类型：如 var int a相当于C里面的(int a)但是a的值可以修改，
  //                  var& int a相当于C里面的(int* a)其值可以修改
  // immutable引用类型：不会修改外部传入值
  // variable引用类型：可能会修改外部传入值

  func modify(int a) { //在C实现中自动根据对应值类型的大小决定传值还是传const指针
    a = 3 // error
  }

  func modify(var int a) { //在C实现中传值
    a = 3 // ok, does't affect outside value
  }

  func modify(var& int a) { //&只能用来修饰值类型，在C实现中传指针
    a = 3 // ok, affect outside value
  }
}

局部作用域之函数体内 {
  // 常量
  const PI = 3.14

  // 变量
  var a = 0

  // 不可修改变量
  immutable b = a
  
  // static变量
  static var sa = 0
  static immutable sb = 0
}

// 小值转换成大值，可以直接使用后缀；否则必须进行强制转换，使用后缀还是会报错
var a = Test() as Base
var b = int(32.2)
var c = double(32)
var d = 32.2
var e = 23f
var f = 23ull

var addFunc0 = (int a, b) int { 
  return a + b 
}

var addFunc1 = addFunc

typedef Func = (int a, b) int

var addFunc2 = (lhs, rhs) as Func {
  return lhs == rhs
}

var a = 0

var addFunc3 = [a](x, y) as Func { 
  return a + x + y
}

var addFunc4 = [var a](x, y) as Func {
  a += 1
  return a + x + y
}
```

## 函数重载规则

```c
1. 参数只区分类型，不区分immutable还是variable
2. 
```

## Uniform call syntex
```c
f(x,y) if not found then try x.f(y)
x.f(y) if not found then try f(x,y) 
Test.print(t) will seek Test|print(Test: test)
```

## Data and its method
```c
space lucy.test
using lucy.core.Print, Scanf //只需要导入struct/class名称，文件作用域内的全局变量名、函数名、常量名、函数类型名称
using lucy.base.Fopen as FileOpen, Fclose as FileClose, Fread as BaseFileRead
using lucy.stream.* //提示所有同名标识符，using只能占用一行

// possible global prefix, multiple defines can be group in { and }
// - space
// - using
// - typedef
// - var
// - func
// - enum
// - const
// - class

typedef Func = (int, int) int

func printTest(int a) byte {
  return byte(a)
}

func _printTest() byte, byte {
  return 1, 1
}

var addFunc = (a, b) as Func {
  return a + b
}
var x = 3
var addFunc2 = [x](a, b) as Func {
  return x + a + b
}
var addFunc3 = addFunc //函数类型可以自动推导出来

var a = 3
var aa = 3s
var bb = 4LL
var cc = 23.0

var f1 = float(23)
var fa = 234f

const PI = 3.1415926
const MaxSize = 128
const Tag = "abcd"
const GoldenSeq = [1, 3, 5, 7]
const B1 = byte(23)
cosnt B2 = 23b

enum Color {
  Red = 3LL, 
  Yellow, 
  Blue
}

enum Color2 {
  Red = byte(3),
  Yellow,
  Blue,
}

//数据成员、参数声明列表使用Type name形式
class _Test {
  var size = 0
  var offset = int? // need init it manually
} 

class DefaultInit: Base {
  var equal = UserEqual // member is private
  var mask = 0xFFFF     // member is private
}

func setter(Test) {

}

func getter(Test) {

}

// const version
func print(Test) void {
  Print(.size)
}

// mutable version
func print(var Test) void {
  Print(.size)
}

func @override start(Test) int, Func {
  return 1, (x, y) { return x + y }
}

func _start(Test, int a) int num, Func sum {
  return 2, [a](x, y) { return a + x + y }
}

func _start(var Test, int a, b) {
  return
}

// Class static functions

func Test.create(int a, b) {
  return Test(1, 2)
}

func Test._create() {
  return Test.create(0, 1)
}

```

## Member reference 
```c
pobj->m, obj.m => pobj.m, obj.m
```

## 不定长参数
```c
// ... ..< ..=
func sum(int... args) int { 
  var total = 0
  for (elem in args) {
    total += args
  }
  return total
}
```

## Meta Programming

```c
```
