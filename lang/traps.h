
# traps

```c
//error C4146: unary minus operator applied to unsigned type, result still unsigned
int32_t a = -2147483648; // 2147483648 is not int (max 2147483647) but a unsigned int, then apply unary minus
```
