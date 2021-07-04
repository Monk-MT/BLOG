由于项目需要使用 ijkplayer, 并且需要修改 ijkplayer 内部的一些解码方法，所以需要先学会编译 so 库。

# 一、准备工作

## 1. 原因和选项

由于 ijkplayer 作者实在 linux 环境下进行编译的，对 windows10 用户说，就需要安装一个 linux 系统。经过一番对比之后这里选择**在虚拟机 VMware Workstation 安装 Ubuntu 系统**。

首先 VMware Workstation 虚拟机和 Ubuntu 系统都是使用较广的软件和系统，网络上教程较多，问题也可以轻松找到答案，对于我这种新手小白来说比较友好，并且用户多也代表稳定性较好；其次选择虚拟机而不是双系统也是因为虚拟机比较方便，不用来回开关机切换系统（毕竟 windows10 用习惯了），并且 VMware 也可以很方便的和 windows 互传文件。下面就开始安装：

## 2. 安装

由于网上已经有非常详细的说明了，这里就不再详细写了，提供两个链接：

[2020 最新版 VMware 安装 Ubuntu20.04 教程（巨细）！](https://zhuanlan.zhihu.com/p/141033713)

[VMware 安装 Ubuntu 虚拟机流程](https://blog.csdn.net/bean_business/article/details/112259813)

## 3.tips（先看再安装，干活不会慌）

- 文章容易过时，可以搜索 **VMware 安装 Ubuntu**，并通过搜索网站的工具/时间来筛选最近的教程
- VMware Workstation Pro 需要序列号，网上有许多，搜索一下即可。
- Ubuntu 安装时**至少需要 30G 的存储空间（不是内存）**，否则你之后可能会崩溃。
- Ubuntu 安装后一定要安装 **VMware Tools** ，可以方便的在 windows 和虚拟机中互传文件。
- 建议安装 Ubuntu 时参考 [这篇文章](https://zhuanlan.zhihu.com/p/38797088)，当然，无脑安装也可以。
- Ubuntu 要配置一下**镜像服务器**，这样安装软件时比较快，具体可以参考 [这篇文章](https://zhuanlan.zhihu.com/p/38797088)（上边这篇）。

# 二、配置环境

## 1. 本次编译需要的环境如下

- JDK
- SDK
- NDK
- Git
- Make
- vim
- yasm

## 2. 命令行工具

在 Ubuntu 中命令行工具叫做终端，大部分操作都是通过终端进行的，有两种打开终端的方法。

一是通过应用中心打开。

<img src="https://img-blog.csdnimg.cn/2021031322525253.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhc2VtYXRl,size_16,color_FFFFFF,t_70#pic_center" width="70%" >

&nbsp;

二是在文件夹中打开。

<img src="https://img-blog.csdnimg.cn/20210313225313561.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhc2VtYXRl,size_16,color_FFFFFF,t_70#pic_center" width="70%">

&nbsp;&nbsp;

## 3.JDK

这里还是给出参考链接：

[Linux 之 Ubuntu18.04 安装 Java JDK8 的三种方式](https://blog.csdn.net/zbj18314469395/article/details/86064849)

这里我选择的是命令行方式安装 oracle Java JDK，并且安装的最新版。安装完成后不需要配置环境变量。

查看 JDK 是否配置成功：

```shell
java -version
java
javac
```

&nbsp;

## 4.SDK

### 1）安装方式一

SDK 的安装我也是采用不用配置环境变量的方式，先安装 Android Studio，AS 在安装时自动会安装 SDK。参考链接如下：

[ubuntu16.04 安装 android studio](https://blog.csdn.net/zrf1994/article/details/103918600)

#### tips

- 解压： ```tar 文件名```
- 选择 SDK 安装目录时要注意，最好安装在/opt 文件夹内，避免文件夹权限影响使用

### 2）安装方式二

#### 下载 SDK

去 [http://tools.android-studio.org/](http://tools.android-studio.org/) 网站查看要下载的 sdk 的名字，之后即可用终端命令下载

```shell
wget  https://dl.google.com/android/android-sdk_r24.2-linux.tgz
```

#### 解压 SDK

```shell
tar xvzf android-sdk_r24.2-linux.tgz
```

#### 移动到/opt 目录下

```shell
sudo mv android-sdk-linux /opt
```

#### 打开环境变量配置文件

```shell
sudo gedit ~/.bashrc
```

#### 添加环境变量

添加如下环境变量到文件最后，依据具体的位置添加

```shell
export ANDROID_SDK_HOME=/opt/android-sdk-linux
export PATH=$PATH:${ANDROID_SDK_HOME}/tools
export PATH=$PATH:${ANDROID_SDK_HOME}/platform-tools
```

#### 保存文件并使之生效

```shell
source  ~/.bashrc
```

#### 重启一个终端，输入 android 打开打开 Android SDK Manager

```shell
android
```

选择 Android SDK Platform-tools、Samples for SDK 等等下载安装，注意在 extra 中选择 Android Support Repository（gradle 需要）

## 5.NDK

ijkplayer 官方推荐的编译版本为 NDK r10e 所以本文采用的环境也相同。

NDK 没有方便的安装方案，具体如下：

### 1）下载方式一

#### 下载 Android-NDK

```shell
wget -c http://dl.google.com/android/ndk/android-ndk64-r10b-linux-x86_64.tar.bz2
```

#### 解压文件

```shell
sudo tar -C /解压路径 -xvf android-ndk64-r10b-linux-x86_64.tar.bz2
```

### 2）下载方式二

#### 下载 Android-NDK

```shell
wget -c http://dl.google.com/android/ndk/android-ndk-r10e-linux-x86_64.bin
```

#### 执行 bin 文件（即解压）

```shell
./android-ndk-r10c-linux-x86_64.bin
```

#### 移动到/opt 文件夹下

```shell
sudo mv android-ndk-r10e /opt
```

#### 要想使用 Android-NDK，还需要进行环境变量的配置

```shell
sudo gedit ~/.bashrc
```

#### 在文件末尾添加以下内容（具体的路径为准）

```shell
export ANDROID_NDK=/opt/android-ndk-r10e
export PATH=${PATH}:$ANDROID_NDK
```

#### 保存文件并使之生效

```shell
source  ~/.bashrc
```

### 安装并配置完成 Android-NDK 之后，需要进行安装验证，以确认正确安装并配置

```shell
ndk-build
```

出现下图即为正常

<img src="https://img-blog.csdnimg.cn/20210313225804947.png#pic_center" width="70%">

&nbsp;

## 6.Git

```shell
sudo apt-get install git
```

## 7.ake

```shell
sudo apt-get install ubuntu-make
```

## 8.vim

```shell
sudo apt install vim
```

## 9.yasm

```shell
sudo apt-get install yasm
```

# 三、编译

## 1. 准备源码

### 把源代码克隆到本地

```shell
git clone https://github.com/Bilibili/ijkplayer.git ijkplayer-android
```

### 下载好后进入源代码的主目录

```shell
cd ijkplayer-android
```

### 切换到最新版本（到 [github](https://github.com/bilibili/ijkplayer) 上查看最新版本，好像不更新了）

```shell
git checkout -B latest k0.8.8
```

## 2. 配置编译支持所有格式

ijkplayer 默认编译配置在 config/目录下，默认使用的是 module-default.sh，可以使用 vim 命令打开里面内容查看具体支持的播放音视频类型等。

官方提供的四个编译脚本分别为：

- module-default.sh：最多的编码解码支持。
- module-lite-hevc.sh：主流的编码解码支持（支持 hevc），更小的体积。
- module-lite.sh：主流的编码解码支持，更小的体积。（默认编译使用的是这个脚本）

本文以编译最全的 so 为例，需要执行的操作如下：

### 进入到 config 目录

```shell
cd config
```

### 先打开 module-default.sh

```shell
vi module-default.sh
```

### 在尾部添加下面代码，要不然会编译失败，这里是一个大坑

```shell
export COMMON_FF_CFG_FLAGS="$COMMON_FF_CFG_FLAGS --disable-linux-perf"
export COMMON_FF_CFG_FLAGS="$COMMON_FF_CFG_FLAGS --disable-bzlib"
```

- 需要学习怎么使用 vim
- 若想能处理 H.264 文件此处还需要改动，看这篇：[ijkplayer 支持播放 h264 本地文件](https://blog.csdn.net/sinat_35990829/article/details/105363812)

### 选择编译格式

```shell
rm module.sh
ln -s module-default.sh module.sh
cd ..
cd android/contrib
sh compile-ffmpeg.sh clean
```

## 3. 开始编译

### 回到项目根目录 ijkplayer-android

```shell
cd ../../
```

### 可选的配置项

#### 建议一

修改 `ijkplayer-android` 目录下的 `init-android.sh` 、 `init-android-openssl.sh` 的编译脚本，选择想要编译所有版本的 so 库，当然，不选也没关系，默认是全部编译的。关于构架的选择看这篇：[为何大厂 APP 如微信、支付宝、淘宝、手 Q 等只适配了 armeabi-v7a/armeabi](https://juejin.im/post/6844904148589084680#heading-4)

init-android.sh 和 init-android-openssl.sh 一样，我就只贴一个供参考就行了，可以结合建议二一起更改

```shell
#把不用的注释掉就好了
#pull_fork "armv5"
#pull_fork "armv7a"
pull_fork "arm64"
pull_fork "x86"
#pull_fork "x86_64"

```

#### 建议二

修改一下`ijkplayer-android` 目录下的 `init-android.sh` 、 `init-android-openssl.sh` 、 `init-android-libyuv.sh` 和 `init-android-soundtouch.sh` 中所有的 github 项目地址，这样 clone 的时候比较快，并提高成功率，反正我替换前没成功几次

下面是我用我的 gitee 库替换的，不一定长期有效，你们也可以自己 clone github 项目到自己的 gitee 上

ijkplayer-android

```shell
# IJK_FFMPEG_UPSTREAM=git://git.videolan.org/ffmpeg.git

#IJK_FFMPEG_UPSTREAM=https://github.com/Bilibili/FFmpeg.git
#IJK_FFMPEG_FORK=https://github.com/Bilibili/FFmpeg.git

IJK_FFMPEG_UPSTREAM=https://gitee.com/be_a_monk/FFmpeg.git
IJK_FFMPEG_FORK=https://gitee.com/be_a_monk/FFmpeg.git
```

init-android-openssl.sh

```shell
#IJK_OPENSSL_UPSTREAM=https://github.com/openssl/openssl

#IJK_OPENSSL_UPSTREAM=https://github.com/Bilibili/openssl.git
#IJK_OPENSSL_FORK=https://github.com/Bilibili/openssl.git

IJK_OPENSSL_UPSTREAM=https://gitee.com/be_a_monk/openssl.git
IJK_OPENSSL_FORK=https://gitee.com/be_a_monk/openssl.git
```

init-android-libyuv.sh

```shell
#IJK_LIBYUV_UPSTREAM=https://github.com/Bilibili/libyuv.git
#IJK_LIBYUV_FORK=https://github.com/Bilibili/libyuv.git

IJK_LIBYUV_UPSTREAM=https://gitee.com/be_a_monk/libyuv.git
IJK_LIBYUV_FORK=https://gitee.com/be_a_monk/libyuv.git
```

init-android-soundtouch.sh

```shell
#IJK_SOUNDTOUCH_UPSTREAM=https://github.com/Bilibili/soundtouch.git
#IJK_SOUNDTOUCH_FORK=https://github.com/Bilibili/soundtouch.git

IJK_SOUNDTOUCH_UPSTREAM=https://gitee.com/be_a_monk/soundtouch.git
IJK_SOUNDTOUCH_FORK=https://gitee.com/be_a_monk/soundtouch.git
```

### 配置 ssl，https 协议

```shell
./init-android-openssl.sh
./init-android.sh
```

这里下载 ffmpeg 和预编译内容，比较耗时，可以喝一杯咖啡先。

### 进入到 ijkplayer-android/android/contrib

```shell
cd android/contrib
```

### 编译 openssl

编译 openssl、ffmpeg、so 文件都需要选择 CPU 架构

这里的话看和上边一样，如果想编译所有版本的 so 库，就输入 all，比如`./compile-openssl.sh all`，如果是特定 CPU 架构就输入 cpu 架构，比如：`./compile-openssl.sh armv7a`，上边选择过了这里就一定要选，不然会报错。

我这里选择`armv7a` 和 `x86_64` 分别给手机和模拟器用

```shell
./compile-openssl.sh clean
./compile-openssl.sh arm64 
./compile-openssl.sh x86 
```

### 编译 ffmpeg

```shell
./compile-ffmpeg.sh clean
./compile-ffmpeg.sh arm64
./compile-ffmpeg.sh x86
```

这里的编译也比较耗时，不过等待的就是成功的喜悦啦

结束后，编译就完成了，接下来就是生成 so 文件

### 生成 so 文件

```shell
cd ..
./compile-ijk.sh arm64
./compile-ijk.sh x86
```

生成过程不会太久，会在/ijkplayer-android/android/ijkplayer 目录下生成我们所想要的包，生成的包还挺大的。

每一个步骤都是比较重要的，落掉某个步骤可能就会导入生成失败，每次的编译生成过程也是比较耗时的，所以少踩点坑就可以节约很多时间。

# 下篇

[ijkplayer 学习二：运行 Demo 并集成到项目](https://blog.csdn.net/casemate/article/details/114767343)

# 参考

[【Linux】Ubuntu 下安装并配置 Android-NDK（附详细过程）](https://blog.csdn.net/qq_38410730/article/details/94151172)

[ijkplayer 编译的踩坑之路](https://www.jianshu.com/p/54c8b01df832)

[ijkplayer 编译 so 库真没那么难](https://blog.csdn.net/coder_pig/article/details/79134625)
