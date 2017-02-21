##用法
###Step 1.
在文件中包含 memcheck.h文件
```
#include "memcheck.h"
```

###Step 2.
在main或只调用一次的函数中调用my_malloc_ini()，从这个函数开始，后面的malloc函数都会被`my_malloc_hook`函数，会print出相关信息，如果想要打印出注释信息，以*开头

```
int main(){
    my_init();
    char* ptr = (char*)malloc(4696); 
    //to do
    print("****start main****\n");
}
```

###Step 3.
运行程序，将信息打印到指定的日志文件中，修改check.sh中的$log配置，指向你的日志文件

```
logs=~/logs/20170220.out //your log file
```
可以看到日志如下：

```
malloc 0x83c2128 from   0x804b418 size 4696
****start main****
malloc 0x83c2140 from   0xb79b29ad size 11
free   0x83bf518 from   0x4ee876
free   0x83bd510 from   0xb79b26b8
free   0x83bf680 from   0xb79b2fcc
malloc 0x83bd510 from   0xb79b3da6 size  7
realloc0x83bba60 from   0xb79c31be size  117 oldptr  0x83bba60
free   0x83bd510 from   0xb78ed599
free   0x83c2140 from   0xb79c3b6a
```

###Step 4.
运行check.sh进行解析，会打印出有malloc/realloc但是没有对应free的地方，像这样：
```
0x8425f00 6201 0x804b418  4696
****start main****
```
每一行分别代表 %内存地址% %日志行数% %调用函数地址% %内存大小%
可以看出在代码0x804b418处，声明了大小为4696的内存，但是没有free


###Step 5. 
调用addr2line，查看0x804b418对应的源代码
[cindyhua@dev-SpyTux-68 tool-memcheck]$addr2line -e a.out -f 0x804b418
main                       
/main.c:4
