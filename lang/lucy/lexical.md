
```c
struct  {
  void* 
};

struct 

concept Readable(T) {
  func get(T) byte?              // ?表示返回值可能无效，必须检查返回值，否则会报错
  func read(T, int bytes) [byte]
}

concept Writeable(T) {
  func put(var T, byte data) int
  func write(var T, [byte] data) int 
}

import io.conc.Readable Writeable
func encode(!I in, !O out) { Readable(T), Writeable(O) |
  
}

func decode(!I in, !O out) { Readable(T), Writeable(O) |

}
```
