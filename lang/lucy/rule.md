
# Primary Types
```c
byte/int8 - unsigned/signed 8-bit integer
uint/int - unsigned/signed integer at least with pointer-size
real      - double float pointer number
uptr      - uint
uint16 int16 - unsigned/signed 16-bit integer
uint32 int32 - unsigned/signed 32-bit integer
uint64 int64 - unsigned/signed 64-bit integer
```

# Uniform call syntex
```c
f(x,y) if not found then try x.f(y)
x.f(y) if not found then try f(x,y) 
```

# Data and its method
```c
space lucy.test;
using lucy.core.Print Scanf;
using lucy.core.Fopen as FileOpen;

// global prefix, multiple defines can be group in { and }
// - space
// - using
// - def
// - enum
// - struct
// - struct name

def Func = (int, int) int;
def a = 3;

def @PrintTest() byte, byte {
  return 1, 1;
}

enum PI = 3.1415926;
enum Color = Red, Yellow, Blue;

struct @Test {
  int size;   // default init to 0
  int offset? // need init it manually
}

struct DefaultInit {
  equal (int, int) byte = UserEqual;
  int mask = 0xFFFF;
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

Test.@start(int a) int num, Func sum {
  return 2, [a](x, y) { return a + x + y; };
}

Test:@start(uptr!int: a, int: b) {
  return;
}

Test_create(int a, int b) {
  return Test(1, 2);
}

Test_@create(int a, int b) {
  return Test(1, 2);
}
```

# Member reference 
```c
pobj->m, obj.m => pobj.m, obj.m
```

# Uniform function type
```c

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

