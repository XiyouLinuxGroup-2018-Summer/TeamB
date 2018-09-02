---起先是对Makefile这个东东并不感冒,觉得很麻烦啊,好像也没什么用啊.直到真正写了个对于自己来讲算大型的项目,才真正知道了Makefile的好处.

起初的缘分来源于觉得需要编译运行时,需要写一长串东西很麻烦,就在Makefile文件里写了个编译运行的shell命令,(这个操作被学长们称作很骚...) 只需make一下,一步搞定~

后来还是要了解一下为什么可以这么做,原来
>make只是一个根据指定的Shell命令进行构建的工具.
只要 你规定好: 构建哪个文件,它依赖哪些源文件,当那些文件有变动时,如何重新构建它.
当只要任何某个文件有变化时,需要重新构建的项目,都可以使用make构建.

代码变成可执行文件，叫做编译（compile）；先编译这个，还是先编译那个（即编译的安排），叫做构建（build）。Make是最常用的构建工具.

注意需要讲清楚依赖和调用



想要真正写出完整的Makefile文件,首先得了解Makefile文件构建规则

##规则如下
```
<target> : <prerequisites>
[tab] <commands>
```
注意:
1."目标"(target)是必须的,不可省略.
"前置条件"(prerequisites)和"命令"(commands)都是可选的,但是两者之中必须存在一个.
2.每条规则就明确两件事:构建目标的前置条件是什么,以及如何构建.

---
###目标(target)
一个目标(target)就构成一条规则.
目标通常是文件名,指明make命令所要构建的对象.
ps:目标可以使一个文件名,也可以是多个文件名,之间用空格分隔,除了文件名,目标还可以是某个操作的名字,称为"伪目标"(phony target)
```
clean:
        rm *.o
```
上边代码的目标是clean,他不是文件名,,而是一个操作的名字,这就属于伪目标.
上例作用对象是删除文件.

但是,如果当前目录中,正好有一个文件叫做clean,那么这个命令就不会被执行.
因为make发现clean文件已经存在,就认为没有重新构建的必要了,就不会执行rm的命令.

有问题就有解决方法!
为了避免这种情况,可以声明clean是"伪目标"
```
.PHONY: clean
clean:
        rm*.o temp
```
声明clean是"伪目标"后,make就不会去检查是否存在一个叫做clean的文件,而是每次运行都执行对应的命令.

注:如果make命令运行时没有指定目标,默认会执行Makefile文件的第一个目标

---
###前置条件
前置条件通常是一组文件名,之间用空格分隔.他指定了"目标"是否重新构建的判断标准:只要有一个前置文件不存在,或者有过更新,"目标"就需要重新构建.
```
result.txt: source.txt
    cp source.txt result.txt
```
上边代码中,构建result.txt的前置条件是source.txt.如果当前目录中,source.txt已经存在,那么 `make result.txt`可以正常运行,否则必须再写一条规则,来生成source.txt
```
source.txt:
    echo "this is the source">source.txt
```
上边代码中,source.txt后边没有前置条件,就意味着他跟其他文件都无关,只要这个文件还不存在,每次调用make source.txt,它都会生成.
```
make result.txt
make result.txt
```
上边连续执行两次命令`make result.txt`.那么result.txt到底是第几次命令生成的呢?
当然还是第一次啦.
第一次执行会先新建source.txt,然后新建result.txt.第二次执行,make发现source.txt没有变动(时间戳晚于result.txt),就不会再执行任何操作,result.txt也不会重新生成.

如果偶需要生成多个文件,往往采用下面的写法
```
source:file1 file2 file3
```
上边的代码中,source是一个伪目标,只有三个前置文件,没有任何对应的命令
```
make source
```
执行`make source`命令后,就会一次性生成file1,file2,file3三个文件.这比下面的写法要方便很多
```
make file1
make file2
make file3
```
---
###命令(commands)
命令(commands) 表示如何更新目标文件,由一行或多行的Shell命令组成.他是构成"目标"的具体指令,他的运行结果通常就是生成目标文件.

每行命令前必须要有一个tab键.如果想用其他键,可以用内置变量.RECIPEPRFIX声明.
```
.RECIPEPREFIX = >
all:
> echo hello, world
```
上边代码用.RECIPEPRFIX指定,大于号(>)代替tab键.所以,每一行命令的起首变成了大于号(>),而不是tab键.
需要注意的是:每行命令在一个单独的shell中执行,这些shell之间没有继承关系.
```
var-lost:
    export foo=bar
    echo "foo=[$$foo]"
```
上边的代码执行后(make var-lost),取不到foo的值.因为两行命令在两个不同的进程中执行.一个解决办法是将两行命令卸载一行,中间用分号分隔.
```
var-kept:
    export foo=bar; echo "foo=[$$foo]"
```
另一个办法是在换行前加反斜杠转义.
```
var-kept:
    export foo=bar; \
    echo "foo=[$$foo]"
```
最后一个方法是加上.ONESHELL:命令
```
.ONESHELL:
var-kept:
    export foo=bar;
    echo "foo=[$$foo]"
```
说了这么多种方法,其实我最常见到的还是加反斜杠定义的那种方法啦
<br>

---
<br>
##Makefile文件的语法
###注释
井号(#)在Makefile中表示注释
```
# 这是注释
result.txt: source.txt
    # 这也是注释
    cp source.txt result.txt # 这还是注释
```

###回声(echoing)
正常情况下,make会打印每条命令,然后再执行,这就叫回声(echoing).
```
test:
    # 这是测试
```
执行上边的规则,会得到下边的结果
```
make test
# 这是测试
```
在命令的前边加上@,就可以关闭回声
```
test:
    @#这是测试
```
现在如果再执行`make test`,就不会有任何的输出.
由于在构建过程中,需要了解当前在执行那条命令,所以通常只会在注释和纯显示的echo命令前加上@.
```
test:
    @# 这是测试
    @echo T0D0
```

###通配符
通配符(wildcard)用来指定一组符合条件的文件名.Makefile的通配符与Bush一致,主要有星号( * ),问号(?)和[...]
比如  *.o 表示所有后缀名为o的文件.(就跟bush一样啦)
```
clean:
    rm -f *.o
```
###模式匹配
make命令允许对文件名,进行类似正则运算的匹配,主要用到的匹配符是%.
比如,假定当前目录下有f1.c和f2.c两个源码文件,需要将他们编译为对应的对象文件.
```
%o:%.c
```
等同于下面的写法
```
f1.o: f1.c
f2.o: f2.c
```
使用匹配符%,可以将大量同类型的文件,只用一条规则就完成构建

###变量和赋值符
Makefile 允许使用等号自定义变量
```
txt = Hello World
test:
    @echo $(txt)
```

上边代码中,变量txt等于 Hello World.
注意: 调用时,变量需要放在$()之中

调用Shell变量,需要在美元符号前,再加一个美元符号,这是因为make命令会对美元符号进行转义
```
test:
    @echo $$HOME
```
有时,变量的值可能指向另一个变量.
```
v1 = $(v2)
```
上边的代码中,变量v1的值是另一个变量v2.
这时会产生一个问题,v1的值到底在定义时扩展(静态扩展),还是在运行时扩展(动态扩展)?
如果v2的值是动态的,这两种扩展方式的结果可能会差异很大.

为了解决类似问题,Makefile一共提供了四个赋值运算符(= := ?= +=)
```
VARIABLE = value
# 在执行时扩展,允许递归扩展

VARIABLE := value
# 在定义时扩展

VARIABLE ?= value
# 只有在该变量为空时才设置值

VARIABLE += value
# 将值追加到变量的尾端
```

###内置变量(Implicit Variables)
make命令提供一系列内置变量,比如,$(MAKE)指向当前使用make工具.这主要是为了跨平台的兼容性
```
output:
    $(CC) -o output input.c
```

###自动变量(Autuomatic Variables)
make还提供一些自动变量,他们的值与当前规则有关,主要由以下几个
####$@
$@指代当前目标,就是make命令当前构建的那个目标.比如 `make foo`的$@就指代foo
```
a.txt b.txt
    touch $@
```
等同于下面的写法
```
a.txt:
    touch a.txt
b.txt
    touch b.txt
```

#### $<

\$<指代第一个前置条件. 
比如,规则为 t:p1 p2, 那么\$<就指代p1
```
a.txt: b.txt c.txt
    cp $< $@
```
等同于下面的写法
```
a.txt: b.txt c.txt
    cp b.txt a.txt
```

####\$?
\$?指代比目标更新的所哟偶前置条件,之间以空格分隔.比如,规则为 t: p1 p2,其中p2的时间戳比t新,$?就指代p2

####\$^
\$^指代所有前置条件,之间以空格分隔.比如,规则为 t:p1 p2,那么$^就指代 p1 p2.

####\$*
\$*指代匹配符%匹配的部分,比如%匹配f1.txt中的f1,$*就是f1

####\$(@D)和\$(@F)
\$(@D)和\$(@F)分别指向$@的目录名和文件名.
比如,$@是src/input.c.那么\$(@D)的值为src, \$(@F)是input.c

####\$(<D)和\$(<F)
分别指向 \$<的目录名和文件名
自定变量的一个例子如下:
```
dest/%.txt: src/%.txt
    @[ -d dest ] || mkdir dest
    cp $< $@
```
上边代码将src目录下的txt文件,拷贝到dest目录下.首先判断dest目录是否存在,如果不存在就新建,然后,$<指代前置文件(src/%.txt), $@指代目标文件(dest/%.txt)

###判断和循环
Makefile使用bush语法,完成判断和循环
```
ifeq ($(CC),gcc)
    libs=$(libs_for_gxx)
else
    libs=$(normal_libs)
endif
```
上边的代码判断当前的编译器是否是gcc,然后指定不同的库文件
```
LIST = one two three
all:
    for i in $(LIST); do \
        echo $$i; \
    done
    
# 等同于

all:
    for i in one two three; do \
        echo $i; \
    done
```
上边代码运行结果
```
one
two
three
```

###Makefile还可以使用函数
```
$(function arguments)
# 或者
${function arguments}
```

####shell 函数
shell函数用来执行shell命令

```
srcfiles := $(shell echo src/{00..99}.txt)
```
####wildcard 函数

wildcard 函数用来在Makefile中,替换Bush的通配符.
```
srcfiles := $(wildcard src/*.txt)
```
####subst函数
subst函数用来进行文本替换
```
$(subst from,to,text)
```
下边的例子将字符串"feet on the street"替换成"fEEt on the strEEt"
```
$(subst ee,EE,feet on the street)
```
####patsubst函数
patsubst 函数用于模式匹配的替换,模式如下
```
$(patsubst pattern,replacement,text)
```
下面的栗子将文件名"x.c.c bar.c",替换成"x.c.o bar.o"
```
$(patsubst %.c,%.o,x.x.x bar.c)
```
####替换后缀名名
替换后缀名的写法是:变量名+冒号+后缀名替换规则.它实际上是patsubst函数的一种间歇形式
```
min: $(OUTPUT:.js=min.js)
```
上边代码意思是,将变量OUTPUT的后缀名.js全部替换成.min.js
<br>

---
<br>
##Makefile的实例
####执行多个目标
```
.PHONY: cleanall cleanobj cleandiff

cleanall: cleanobj cleandiff
    rm program
cleanobj:
    rm *.o
cleandiff
    rm *.diff
```
上边代码可以调用不同的目标,删除不同后缀名的文件,也可以调用一个目标删除所有指定类型的文件.
####编译c语言项目
```
edit : main.o kbd.o command.o display.o
    cc -o edit main.o kbd.o command.o display.o
    
main.o : main.c defs.h
    cc -c main.c
kbd.o : kbd.c defs.h command.h
    cc -c kbd.c
command.o : command.c defs.h command.h
    cc -c command.c
display.o : display.c defs.h
    cc 0c display.c
    
clean :
    em edit main.o kdb.o command.o diaplay.o
.PHONY: edit clean
```
