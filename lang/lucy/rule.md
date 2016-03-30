
# Primary Types
```c
byte/int8 - unsigned/signed 8-bit integer
uint/tint - unsigned/signed integer at least with pointer-size
real      - double float pointer number
uptr      - uint
uint16 tint16 - unsigned/signed 16-bit integer
uint32 tint32 - unsigned/signed 32-bit integer
uint64 tint64 - unsigned/signed 64-bit integer
```

# Uniform call syntex
```c
f(x,y) if not found then try x.f(y)
x.f(y) if not found then try f(x,y) 
```

# Data and its method
```c
space lucy.test;
using lucy.core Print;
using lucy.core Print Scanf;

struct Test {
  tint size;
}

Test.print() -> void {
  Print(self.size);
}
```

# Member reference 
```c
pobj->m, obj.m => pobj.m, obj.m
```