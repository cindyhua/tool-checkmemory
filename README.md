#tool-checkmemory

##How to Use it
###Step 1.
include memcheck.h in file

    #include "memcheck.h"

###Step 2.
in main function,load `my_init()`;
you can also print your own info,but make sure start with *
```
#include "memcheck.h"
    int main(){
    	my_init();
        char* ptr= (char*)malloc(4696);
    	//to do
    	print("****start main****\n");
    }
```

###Step 3.
run program,make sure the the infomation printed on a log file, then modify the parameter $logs to your log file name in check.sh

`logs=~/logs/20170220.out //your log file`

the logs will like:

```
malloc 0x83c2128 from   0x804b418 size 4696
****start main****
malloc 0x83c2140 from   0xb79b29ad size 11
free   0x83bf518 from   0x4ee876
free   0x83bd510 from   0xb79b26b8
free   0x83bf680 from   0xb79b2fcc
malloc 0x83bd510 from   0xb79b3da6 size  7
realloc0x83bba60 from   0xb79c31be size117 oldptr   0x83bba60
free   0x83bd510 from   0xb78ed599
free   0x83bba50 from   0xb79c3b6a
```


###Step 4.
run check.sh,then it will print where malloc/realloc but without free,like this:

```
0x8425f00 6201 0x804b418  4696
****start main****
```

every column present 
%addr% %log line% %caller% %size%

it means that code at 0x804b418,where malloc heap size of 4696 without free


###Step 5.
use addr2line ,find the source line

```
[cindyhua@dev-SpyTux-68 tool-memcheck]$addr2line -e a.out -f 0x804b418
main                       
/main.c:4
```

