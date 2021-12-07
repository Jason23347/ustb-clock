# USTB-CLOCK

哎，就是玩～

bash版看这里：https://github.com/Jason23347/ustb-cli

## Compile
### Compile from master branch
先运行
```sh
autoreconf -i
```
然后
```sh
./configure \
    --enable-detect-winsize \
    # --enable-debug \
    # --disable-colorful-output
```
最后
```sh
make -j8 # 不 -j8 也行
```

本项目的一切参数优先适配了树莓派LCD屏幕，在 [conf.h](./src/conf.h) 修改。
### Compile from released tarball
改完已知bug再发布tarball。

~~求好心人帮我 debug calc_* 函数里的 unsigned long 导致的溢出问题。~~

## Help
编译之后，`./ustb-clock -h`查看编译参数。

Like this:
```
$ ./ustb-clock -h
USTB-CLOCK v0.9
+color -debug -winsize

Copy 2021 Jason <jason23347@gmail.com>

Login host:     202.204.48.82
Login port:     80
Clock color:    BG_DARKGREEN
Dot width:      2
Space width:    3
Condense mode:  FALSE
Info refresh interval: 1 second(s)
```

## Quitting program
退出？什么退出？你只能 Ctrl+C 了！
## TODOs

- [x] 显示时间
- [x] 显示星期
- [x] 显示流量信息
- [x] 颜色
- [x] 时间刷新
- [x] 流量信息刷新
- [x] 监听各种退出信号（退出时至少显示光标...）
- [x] 监听窗口大小变化