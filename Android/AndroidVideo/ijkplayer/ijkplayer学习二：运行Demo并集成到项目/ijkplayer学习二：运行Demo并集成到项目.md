接上一篇[编译篇](https://blog.csdn.net/casemate/article/details/114766740)：

这里接着讲怎么运行播放ijkplayer的sample中自带的Demo并把ijkplayer集成到项目中。

# 运行环境

首先我的运行环境如下

Android Studio版本：4.1.2
buildToolsVersion "30.0.3"

gradle版本：
<img src="https://img-blog.csdnimg.cn/20210313230711869.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhc2VtYXRl,size_16,color_FFFFFF,t_70#pic_left" width="70%">

&nbsp;

# 拷贝文件
把ijkplayer-android\android\ijkplayer从Ubuntu中拷贝到windows中。

如果之前安装了VMware Tools则把ijkplayer文件夹压缩后从Ubuntu拖出来到Windows再解压即可。

#### ijkplayer的目录如下：

<img src="https://img-blog.csdnimg.cn/20210313231027616.png#pic_center" width="25%">

&nbsp;

# 再AndroidStudio中打开ijkplayer

<img src="https://img-blog.csdnimg.cn/20210313231058919.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhc2VtYXRl,size_16,color_FFFFFF,t_70#pic_center" width="70%">

&nbsp;

# 更改ijkplayer项目中的build.gradle

可以根据自己AS和gradle版本来进行更改，注意代码中的汉字注释

```java
// Top-level build file where you can add configuration options common to all sub-projects/modules.

buildscript {
    repositories {
        // 镜像地址，便于构建项目
        maven{url 'http://maven.aliyun.com/nexus/content/groups/public/'}
        maven{url "https://jitpack.io"}
        google()
    }
    dependencies {
        // 根据自己的具体情况更改
        classpath "com.android.tools.build:gradle:4.1.2"

        classpath 'com.github.dcendents:android-maven-gradle-plugin:1.4.1'
        classpath 'com.jfrog.bintray.gradle:gradle-bintray-plugin:1.7'
        // NOTE: Do not place your application dependencies here; they belong
        // in the individual module build.gradle files
    }
}

allprojects {
    repositories {
    	// 镜像地址，便于构建项目
        maven{url 'http://maven.aliyun.com/nexus/content/groups/public/'}
        google()
    }
}

ext {
    compileSdkVersion = 30 //根据自己的具体情况更改
    buildToolsVersion = "30.0.3" //根据自己的具体情况更改

    targetSdkVersion = 30 //根据自己的具体情况更改

    versionCode = 800800
    versionName = "0.8.8"
}

wrapper {
    gradleVersion = '6.5' //根据自己的具体情况更改
}
```

&nbsp;

# 更改ijkplayer-example中的build.gradle

主要是主要将complie修改成implementation。

添加```flavorDimensions "default"```避免```Error:All flavors must now belong to a named flavor dimension.```报错

修改```all32 { minSdkVersion 21 } ```避免 ```The minSdk version should not be declared in the android manifest file.  ```报错

```java
apply plugin: 'com.android.application'

android {
    // http://tools.android.com/tech-docs/new-build-system/tips
    //noinspection GroovyAssignabilityCheck
    compileSdkVersion rootProject.ext.compileSdkVersion
    //noinspection GroovyAssignabilityCheck
    buildToolsVersion rootProject.ext.buildToolsVersion

    lintOptions {
        abortOnError false
    }
    defaultConfig {
        applicationId "tv.danmaku.ijk.media.example"
        minSdkVersion 9
        targetSdkVersion rootProject.ext.targetSdkVersion
        versionCode rootProject.ext.versionCode
        versionName rootProject.ext.versionName
        // 添加
        flavorDimensions "default"
    }
    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'
        }
    }
    productFlavors {
        all32 { minSdkVersion 21 } // 修改
        all64 { minSdkVersion 21 }
        // armv5 {}
        // armv7a {}
        // arm64 { minSdkVersion 21 }
        // x86 {}
    }
}

dependencies {
    // 下边的所有compile修改成implementation
    implementation fileTree(include: ['*.jar'], dir: 'libs')
    implementation 'com.android.support:appcompat-v7:23.0.1'
    implementation 'com.android.support:preference-v7:23.0.1'
    implementation 'com.android.support:support-annotations:23.0.1'

    implementation 'com.squareup:otto:1.3.8'

    implementation project(':ijkplayer-java')
    implementation project(':ijkplayer-exo')

    all32Implementation project(':ijkplayer-armv5')
    all32Implementation project(':ijkplayer-armv7a')
    all32Implementation project(':ijkplayer-x86')

    all64Implementation project(':ijkplayer-armv5')
    all64Implementation project(':ijkplayer-armv7a')
    all64Implementation project(':ijkplayer-arm64')
    all64Implementation project(':ijkplayer-x86')
    all64Implementation project(':ijkplayer-x86_64')

    // compile 'tv.danmaku.ijk.media:ijkplayer-java:0.8.8'
    // compile 'tv.danmaku.ijk.media:ijkplayer-exo:0.8.8'

    // all32Compile 'tv.danmaku.ijk.media:ijkplayer-armv5:0.8.8'
    // all32Compile 'tv.danmaku.ijk.media:ijkplayer-armv7a:0.8.8'
    // all32Compile 'tv.danmaku.ijk.media:ijkplayer-x86:0.8.8'

    // all64Compile 'tv.danmaku.ijk.media:ijkplayer-armv5:0.8.8'
    // all64Compile 'tv.danmaku.ijk.media:ijkplayer-armv7a:0.8.8'
    // all64Compile 'tv.danmaku.ijk.media:ijkplayer-arm64:0.8.8'
    // all64Compile 'tv.danmaku.ijk.media:ijkplayer-x86:0.8.8'
    // all64Compile 'tv.danmaku.ijk.media:ijkplayer-x86_64:0.8.8'

    // armv5Compile project(':player-armv5')
    // armv7aCompile project(':player-armv7a')
    // arm64Compile project(':player-arm64')
    // x86Compile project(':player-x86')
    // x86_64Compile project(':player-x86_64')
}
```

&nbsp;

# 其他修改 

这时候你可以试着运行下，根据AS的报错信息进行修改

每个模块下的build.gradle，需要将complie修改成implementation。（不该不一定会报错）

错误之一及解法

<img src="https://img-blog.csdnimg.cn/20210313231728947.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhc2VtYXRl,size_16,color_FFFFFF,t_70#pic_center" width="70%">

&nbsp;

# 编译成功后，运行例子

模拟器界面

<img src="https://img-blog.csdnimg.cn/20210316210705237.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhc2VtYXRl,size_16,color_FFFFFF,t_70#pic_center" width="30%">

&nbsp;

点击Sample，然后点击其中一个例子。

<img src="https://img-blog.csdnimg.cn/20210316210818952.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhc2VtYXRl,size_16,color_FFFFFF,t_70#pic_center" width="30%">

&nbsp;

有的不成功，都试试，有能播的

<img src="https://img-blog.csdnimg.cn/20210316210838358.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhc2VtYXRl,size_16,color_FFFFFF,t_70#pic_center" width="30%">

&nbsp;

# 集成进项目

由于有很好的博文来讲这个了，就不自己写了（套娃）

[ijkplayer demo编译后导入到项目中](https://blog.csdn.net/weixin_38072112/article/details/109731705?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522161554827516780255217930%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=161554827516780255217930&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_v2~times_rank-16-109731705.pc_search_result_no_baidu_js&utm_term=ijkPlayer)

>集成参考：
>
>[Ijkplayer demo 基本使用](https://www.jianshu.com/p/5d1d46aa721d)
>
>按照上面步骤一步一步完成后，发现了Androidx和support冲突的问题，下面是解决方案：
>
>[Android 迁移到 Androidx](https://www.jianshu.com/p/773e231daed4)
>
>需要一步一步对照表来更改库名称：
>
>[google官方库映射表](https://developer.android.google.cn/jetpack/androidx/migrate/class-mappings#androidsupportv7)

# 参考博客
[ijkplayer的编译和使用](https://blog.csdn.net/qq_41979349/article/details/105227770)