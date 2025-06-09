cat /proc/51327/maps (or another option is (gdb) info proc mappings
)

/*code segment*/
aaaaaaaa0000-aaaaaaaa4000 r-xp 00000000 103:06 528232                    /home/chananyatempleman/Downloads/memory_mapping
/*read-only data segment*/
aaaaaaabc000-aaaaaaac0000 r--p 0000c000 103:06 528232                    /home/chananyatempleman/Downloads/memory_mapping
aaaaaaac0000-aaaaaaac4000 rw-p 00010000 103:06 528232                    /home/chananyatempleman/Downloads/memory_mapping
/*heap segment*/
aaaaaaac4000-aaaaaaae8000 rw-p 00000000 00:00 0                          [heap]
fffff7de0000-fffff7f7c000 r-xp 00000000 103:06 473781                    /usr/lib/aarch64-linux-gnu/libc.so.6
fffff7f7c000-fffff7f8c000 ---p 0019c000 103:06 473781                    /usr/lib/aarch64-linux-gnu/libc.so.6
fffff7f8c000-fffff7f90000 r--p 0019c000 103:06 473781                    /usr/lib/aarch64-linux-gnu/libc.so.6
fffff7f90000-fffff7f94000 rw-p 001a0000 103:06 473781                    /usr/lib/aarch64-linux-gnu/libc.so.6
fffff7f94000-fffff7fa0000 rw-p 00000000 00:00 0 
fffff7fbc000-fffff7fe4000 r-xp 00000000 103:06 473775                    /usr/lib/aarch64-linux-gnu/ld-linux-aarch64.so.1
fffff7fec000-fffff7ff4000 r--p 00000000 00:00 0                          [vvar]
fffff7ff4000-fffff7ff8000 r-xp 00000000 00:00 0                          [vdso]
fffff7ff8000-fffff7ffc000 r--p 0002c000 103:06 473775                    /usr/lib/aarch64-linux-gnu/ld-linux-aarch64.so.1
fffff7ffc000-fffff8000000 rw-p 00030000 103:06 473775                    /usr/lib/aarch64-linux-gnu/ld-linux-aarch64.so.1
/*stack segment*/
fffffffdc000-1000000000000 rw-p 00000000 00:00 0                         [stack]
/*kernel space*/
higher than this.