1. As the recursion gets deeper and deeper, the heap gets pushed towards higher addresses, as can be seen by the increasing addresses of the malloced variable p_heap_var.
p_heap_var = 0xaaaaaaac4360
p_heap_var = 0xaaaaaaac4480
p_heap_var = 0xaaaaaaac44e0
p_heap_var = 0xaaaaaaac4600

2. To check alignment of heap allocated variables I used the following command - 
For p_heap_var - (gdb) p (unsigned long)p_heap_var % sizeof(int)
Output: $7 = 0. It's aligned.
For p_heap_var3 - (gdb) p (unsigned long)p_heap_var3 % sizeof(short)
Output: $9 = 0.  It's aligned.

3. Stack growth using local variables - 
Recusrsion level = 0:
(gdb) p &i
$3 = (int *) 0xffffffffe990
(gdb) p &j
$4 = (int *) 0xffffffffe994
Recursion level  = 1:
(gdb) p &i
$5 = (int *) 0xffffffffe880
(gdb) p &j
$6 = (int *) 0xffffffffe884
Recursion level = 2:
(gdb) p &i
$7 = (int *) 0xffffffffe770
(gdb) p &j
$8 = (int *) 0xffffffffe774
Recursion level = 3:
(gdb) p &i
$9 = (int *) 0xffffffffe660
(gdb) p &j
$10 = (int *) 0xffffffffe664
Conclusion: Stack is growing towards lower addresses with each recursion.

4. Using cat /proc/<PID>/maps (or pmap) command I could map out the range of addresses for each segment, and cross reference it with -
	info locals command to see the addresses of various variables. For example:
	p_global_var3 = 0xaaaaaaabfd50 is in the data segment. And p_non_const_local = 0xfffff7fff5f8 is located in the stack segment.
