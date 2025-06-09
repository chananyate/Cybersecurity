1. int a = 3;
   int foo();

a. expression a;
type: int(l-value)
value: 3
side effect: none (changes do not occur outside the local scope of the operation).

b. expression foo;
type: function
value: address of foo function
side effect: none

c. expression &foo
type: function pointer
value: the address of foo function
side effect: none (simply taking the address of the function does not alter it)

d. expression foo()
type: int(non l-value)
value: the result of the function definition.
side effect: depends on the function definition whether it has a print command or if the variable in the function is a global variable or pointer. 

e. a + foo()
type: int(non l-value)
value: 3 + return value of the function.
side effect: none.

f. &a
type: int*
value: the address of a
side effect: none (simply taking the address of variable does not alter it).
