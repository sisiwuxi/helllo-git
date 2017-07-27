将porting文件夹放到kitkat-M-master/hardware路径下，编译前需先source env
1.新加模块在porting/src/Android.mk中添加编译规则方法如下：
在LOCAL_SRC_FILES :=\的最后面加上要编译的文件
2.将porting放到kitkat-M-master\hardware下，进入porting下编译：mm -B


