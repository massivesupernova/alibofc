
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

@PrintTest() byte, byte {
  return 1, 1;
}

struct @Test {
  int size;     // default init to 0
  int offset ?; // need init it manually
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

Func = (int, int) int;

Test.create() int, Func {
  return 1, (x, y) { return x + y; };
}

Test.@create(int a) int, Func {
  return 2, [a](x, y) { return a + x + y; };
}

Test:@create(uptr!int: a, int: b) {
  return;
}
```

# Member reference 
```c
pobj->m, obj.m => pobj.m, obj.m
```

# Uniform function type
```c

```
