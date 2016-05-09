
- http://stackoverflow.com/questions/25846561/swift-printing-optional-variable

```c
You have to understand what an Optional really is.
Many Swift beginners think `var age: Int?` means that age is an Int which may or may not has a value.
But it means that age is an Optional which may or may not hold an Int.
Inside your `description()` function you don't print the Int, but instead you print the Optional.
If you want to print the Int you have to unwrap the Optinal.
You can use "optional binding" to unwrap an Optional: `if let a = age { /* a is an Int */ }`
If you are sure that the Optional holds an object, you can use "forced unwrapping": `let a = age!`
```
