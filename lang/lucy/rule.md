
# Primary Types
```c
bool - true false
char/utf8/utf16/utf32 - unsigned type
byte/int8 - unsigned/signed 8-bit integer
uint/int - unsigned/signed integer at least with pointer-size
uptr      - uint
uint16 int16 - unsigned/signed 16-bit integer
uint32 int32 - unsigned/signed 32-bit integer
uint64 int64 - unsigned/signed 64-bit integer
float/double

postfix: 23ub  23b    8-bit
         23ui  23     machine word size
         23us  23s    16-bit
         23ul  23L    32-bit
         23ull 23LL   64-bit
         23f   23.0   float double

user defined postfix: operator'kg(int a) int      56'kg + 32'pd

int literal: 0b1101 0o775 0xFA 42   0b1101_1100 0xFFBB_FFFE 430_323_1234
char literal: ' ', 'Space', 'Tab', 'Enter', 'a', 'b', 'c', 42c, 0xF3c
unicode literal: 0u0A11 (utf-16), 0u0000_0A11 (utf-32), 0uu7F (utf-8)

???如何尽量消除指针
```

# Standard Container
```c
string: "abcd"
array: [23, 46, 72,]
table: [23:"abcd", 42:"def"]
set: { 1, 2, 3, 4, 5}

// call function that has only one argument with string/array/table/set type:
Test.print(string s) void { /* ... */ }
Test.print({int} a) void { /* ... */ }
Test.print([int] a) void { /* ... */ }
Test.print([int:string] t) void { /* ... */ }

def test = Test();
test.print"abcd";
test.print{1, 2, 3,};
test.print[23, 12,];
test.print[12:"a", 23:"b"];

def obj = new ClassTest();
def obj2 = new Child() as BaseClass;
```

# Uniform call syntex
```c
f(x,y) if not found then try x.f(y)
x.f(y) if not found then try f(x,y) 
Test.print(t) will seek Test|print(Test: test)
```

# Data and its method
```c
space lucy.test;
using lucy.core.Print, Scanf; //只需要导入struct/class名称，文件作用域内的全局变量名、函数名、常量名、函数类型名称
using lucy.base.Fopen as FileOpen, Fclose as FileClose, Fread as BaseFileRead;
using lucy.stream.*; //提示所有同名标识符

// possible global prefix, multiple defines can be group in { and }
// - space
// - using
// - ftype
// - def
// - const
// - class
// - class name

ftype Func = (int, int) int;

def printTest = (int a) byte {
  return byte(a);
}

def _printTest = () byte, byte {
  return 1, 1;
}

def:Func aFunc = (a, b) { 
  return a + b;
}

def x = 3;
def:Func bFunc = [x](a, b) {
  return x + a + b;
}

def a = 3;

def {
  aa = 3s;
  bb = 4LL;
  cc = 23.0;
}

def:float f1 = 23;

def:float {
  fa = 234;
  fb = 232;
  fc = 231f;
}

const PI = 3.1415926;
const {
  MaxSize = 128;
  Tag = "abcd";
  GoldenSeq = [1, 3, 5, 7];
  Color: Red = 3LL, Yellow, Blue;
}
const:byte {
  B1 = 23;
  B2 = 23b;
  Color2: Red, Yellow, Blue;
}
const Color4: Red = 3, Yellow, Blue; // enum no need to use form - const:type

class _Test {
  int size;   // default init to 0
  int offset? // need init it manually
} 

class DefaultInit {
  ftype Equal = (int, int) byte;
  Equal equal = UserEqual; // member is private
  int mask = 0xFFFF;       // member is private
}

Test.setter {

}

Test.getter {

}

// const version
Test:print() void {
  Print(self.size);
}

// normal version
Test.print() void {
  Print(self.size);
}

Test.start() int, Func {
  return 1, (x, y) { return x + y; };
}

Test._start(int a) int num, Func sum {
  return 2, [a](x, y) { return a + x + y; };
}

Test:_start(int a, int b) {
  return;
}

Test|create(int a, int b) {
  return Test(1, 2);
}

Test|_create() {
  return Test.create(0, 1);
}
```

# Member reference 
```c
pobj->m, obj.m => pobj.m, obj.m
```

# Uniform function type
```c
ftype AddFunc = (int, int) int;
def:AddFunc accu = (x, y) { return x + y; };
def a = 2;
def:AddFunc acc2 = [a](x, y) { return a + x + y; };
```

# Meta Programming
```c
// ... ..< ..=
def sum(int...) int {
  #if @args == 0
    return 0;
  #elif @args == 1
    return @arg1;
  #else
    return @arg1 + sum(@arg2...);
  #endif
}
```

