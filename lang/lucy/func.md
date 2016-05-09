

## 函数参数

```c
//[1] 值类型参数

//1.1 不可修改参数{{/*
int ia;
int ib = ia + 5;
const int aa = 1;
const int bb = ia + ib;
int addOne(const int a) {
  return a + 1;
}
int addOne(const BigData* a) {
  return *a + 1;
}
int a = 3;
int b = a * 5;
addOne(1);    //可以传递常量
addOne(b+a);  //可以传递变量
*/}}
//相当于C中的(const int a)
func addOne(int a) int {
  return a + 1 // 写成++a或a+=1会报错
}
func addOne(immutable int a) int {
  return a + 1
}

//1.2 可修改参数
func addTwo(int a) int {
  var b = a
  return b += 2
}
//1.2.1 取消var的使用{{/*
int addTwo(const int a) {
  int b = a;
  return b += 2;
}
void modify(const BigData data) {
  BigData tmp = data; //影响性能
  tmp.value = 1;
}
//改进
void modify(const BigData* data) {
  BigData tmp = *data;
  tmp.value = 1;
}
*/}}


//[2] 对于引用类型

/*2.1 不可修改参数
func calc(RefData r) int { // 相当于int calc(const RefData*const r)
  r = new RefData() //报错
  r.value = 3       // 报错
  return r.value + 1 
  var r1 = r       // 报错（也可以拷贝一份给r1，但不符合引用语义），const RefData*不能转换成RefData*
}

/*2.2 可修改参数
func calc(inout RefData r) int { //相当于RefData*const r
  r = new RefData() //报错
  r.value = 3       //ok
  return r.value + 1
  var r1 = new RefData() //相当于RefData* r1
  r1 = r
}

## 示例
func addOne(int a) int {
  return a + 1
}
func addOne(inout int a) int {
  return a += 1
}
var ival = 0
addOne(ival)  //ival不变
addOne(&ival) //ival会改变

func calc(RefData r) int {
  return r.value + 1
}
func calc(inout RefData r) int {
  return r.value += 1
}
val rval = new RefData()
calc(rval)  //rval保证不会改变
calc(&rval) //rval会改变
rval = createRefData() //rval引用到新的对象

## 函数参数语法
*parameter表示形参，argument表示实参

func foo(int a, double b) void { /**/ }
func foo(immutable int a, immutable double b) void { /**/ }
func foo(int a, b, double c) void { /**/ }
func bar(inout int a, b, int c, d, double e) void { /**/ }

Parameter:
  ParameterQualifier ParameterDefinition |
  ParameterDefinition |
  "@"

ParameterWithDefaultValue:
  ParameterDefinition "=" Expression

ParameterQualifier:
  "immutable" | 
  "inout"

// TypeName cannot be "void"
ParameterDefinition:
  TypeName ParameterName |
  ParameterName

VariableParameterDefinition:
  "..." ParameterName

// TypeName cannot be "void"
SameTypeVariableParamerDefinition:
  TypeName "..." ParameterName

ParameterList:
  Parameter |
  ParameterList "," Parameter

VariableParameterList:
  VariableParameterDefinition |
  SameTypeVariableParamerDefinition |
  ParameterList "," VariableParameterDefinition |
  ParameterList "," SameTypeVariableParamerDefinition

ParameterPart:
  "(" ")" |
  "(" ParameterList ")" |
  "(" VariableParameterList ")"
  

```
