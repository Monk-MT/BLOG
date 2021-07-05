# 网络抽象层：NAL

H264 基本码流由一系列的 NALU 组成。原始的 NALU 单元组成：

| start code | NALU header | NALU payload |
|:---:|:---:|:---:|
| 3 bytes 或 4 bytes | 1 bytes | RBSP |

## start code

每个 NALU 之间由起始码（Start Code Prefix）分隔，起始码分为两种：0x000001(3 bytes) or 0x00000001(4 bytes)。 如果 NALU 对应的 Slice 为一帧的开始，则用 4 字节表示，即 0x00000001；否则用 3 字节表示，0x000001。

## NALU Header

首先，『NALU Header』只占 1 个字节，即 8 位，其组成如下：

| forbidden_zero_bit | nal_ref_idc | nal_unit_type |
|:---:|:---:|:---:|
| 1 bit | 2 bit | 5 bit |

forbidden_zero_bit 禁止位，初始为 0，当网络识别此单元存在比特错误时，可将其设为 1，以便接收方丢掉该单元。

nal_ref_idc 用来指示该 NALU 的重要性等级，值越大，表示当前 NALU 越重要。

而当 NALU 类型，nal_unit_type 为 6、9、10、11、或 12 时，nal_ref_idc 都为 0

nal_unit_type 指出 NALU 的类型，具体如下图所示：

| nal_unit_type | NALU 类型 | 是否为 VCL |
|:---:|:---|:---:|
| 0  | 未指定 | - |
| 1  | 不分区、非 IDR 图像的片 | 是 |
| 2  | 片数据 A 分区 | 是 |
| 3  | 片数据 B 分区 | 是 |
| 4  | 片数据 C 分区 | 是 |
| 5  | IDR 图像的片 | 是 |
| 6  | 补充增强信息单元（SEI） | 否 |
| 7  | 序列参数集（SPS） | 否 |
| 8  | 图像参数集（PPS） | 否 |
| 9  | 分界符 | 否 |
| 10 | 序列结束 | 否 |
| 11 | 码流结束 | 否 |
| 12 | 填充 | 否 |
| 13..23 | 保留 | - |
| 24..31 | 未指定 | - |

tips:

- 如果不采用 DP（数据分割）机制，则一个片就是一个 NALU，一个 NALU 也就是一个片。否则， 一个片由三个 NALU 组成（即上表中 nal_unit_type 值为 2、3、4 的 三个 NALU 属于 一个片）
- IDR 片不能分割  

## NALU payload

NALU 的主体涉及到三个重要的名词，分别为 EBSP、RBSP 和 SODB。其中 EBSP 完全等价于 NALU 主体，而且它们三个的结构关系为**EBSP 包含 RBSP，RBSP 包含 SODB**。

EBSP = RBSP 中插入防竞争码 0x03:

NALU 针对起始码设计了防止冲突机制，如果出现连续的 0x000000，0x000001,0x000002,0x000003 时，会在两个 0 之间插入 03，如下：

```c
0x00 00 00 -> 0x00 00 03 00
0x00 00 01 -> 0x00 00 03 01
0x00 00 02 -> 0x00 00 03 02
0x00 00 03 -> 0x00 00 03 03
```

RBSP = SODB + RBSP 尾部。

对于 RBSP 尾部分成两种类型：

- 非 1 到 5 的 NALU 类型 SODB+停止位 1+若干个 0 字节对齐
- 条带 NALU 类型  SODB+停止位 1+若干个 0 字节对齐，如果是 CABAC 的编码方式且有更多的数据时，后面再补充一个或多个 0x00。

SODB(String Of Data Bits)，称原始数据比特流，就是最原始的编码/压缩得到的数据。

NALU 的具体结构如下：

<img src="NALU payload.png#pic_center" width="70%">

想理解 H264 是怎么解析二进制的需要了解 h264 句法。H264 的语法采用了类 C 的语言，在标准的第 52 页开始。例如下面是对 NALU 头和 body 部分的解析。

<img src="NALU语法解析.png#pic_center" width="70%">

## H264 单元类型

这节来大概讲一下 NALU payload 中的内容。

根据 NALU header 中的 nal_unit_type 参数，NAL 单元可以被分为很多种，主要的类型有 SPS, PPS, IDR, Slice(I, P, B), SEI 类型。这里简单的介绍一下这几个单元的作用和关键参数，具体的内容单独开章节介绍。

<img src="RBSP描述.png#pic_center" width="70%">

典型的 RBSP 单元序列如图所示

<img src="典型RBSP单元序列.png#pic_center" width="70%">

&nbsp;

参数集机制将一些重要的、改变少的序列参数和图像参数与编码片分离，并在编码片之前传送 至解码端，或者通过其他机制传输。通常，SPS 和 PPS 在片的头信息和数据解码前传送至解码器。在流媒体或直播中 SPS 和 PPS 也会在码流中多次发送。

### SPS

序列参数集 SPS(Sequence Paramater Set) 包含的是针对一连续编码视频序列的参数，如标识 seq_parameter_set_id、帧数及 POC 的约束、参考帧数目、解码图像尺寸和帧场编码模式选择标识等等。

### PPS

图像参数集 PPS(Picture Paramater Set) 对应的是一个序列中某一幅图 像或者某几幅图像 ，其参数如标识符 pic_parameter_set_id、可选的 seq_parameter_set_id、熵编码模式选择标识、片组数目、初始量化参数和去方块滤波系数调整标识等等。

### Slice

Slice 叫做片（条带） ，可分为 I 片，B 片，P 片，后又补充了 SP 片和 SI 片。
一个或多个片组成一帧。

- I 片是帧内编码片，根据本片中的信息即可解码；
- P 片是前向预测编码片，根据前几帧的画面解码；
- B 片是双向预测内插编码片，参考前后两边的帧来解码；
- SP 片是一种特殊的 P 片，可用于不同编码流之间的切换；
- SI 片和 SP 片的原理大致相同。

### IDR

即时解码刷新图像 IDR(instantaneous decoding refresh picture) 是一个特殊的 I 帧，IDR 帧的作用是立刻刷新。在 IDR 帧之后的所有帧都不能引用任何 IDR 帧之前的帧的内容，与此相反，对于普通的 I 帧来说，位于其之后的 B 和 P 帧可以引用位于普通 I 帧之前的 I 帧。使用 IDR 帧可以防止解码错误的持续传播。

关于各帧的详细作用参考如下文章：
[图像和流媒体 -- I 帧，B 帧，P 帧，IDR 帧的区别](https://blog.csdn.net/qq_29350001/article/details/73770702)

### SEI

全称为 Supplemental Enhancement Information ，翻译为补充增强帧。可以增加图像参数，用户信息等，在解码、显示或其它过程中起辅助作用。但 SEI 并非解码过程的必须选项。[SEI 补充知识](https://zhuanlan.zhihu.com/p/33720871)

## 参考文献

[H.264 再学习 -- 详解 H.264 NALU 语法结构](https://blog.csdn.net/qq_29350001/article/details/78226286)

[H.264 编码格式简单分析](https://cloud.tencent.com/developer/article/1461531)

[h.264 判断一帧的结束](https://blog.csdn.net/sstya/article/details/50071369)
