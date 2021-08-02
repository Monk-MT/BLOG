# Java 集合常用方法

## 总表

| | ArrayList | LinkedList | ArrayDeque | PriorityQueue | HashMap | Set
|---|---|---|---|---|---|---|---|
| addAll        |  添加 |  添加 Collection |   |   |   |  
| add           |  添加 Collection |  添加到链表**尾** |  入队**头** | 入队 |   |  添加 Collection
| addFirst      |   |  添加到链表**头** |  入队**头** |   |   |  添加
| addLast       |   |  添加到链表**尾** |  入队**尾** |   |   |  
| putAll        |   |   |   |   |  添加 map |  
| put           |   |   |   |   |  添加 |  
| peek          |   |  获取链表**头** |  查看队**头** | 查看对头 |   |  
| peekFirst     |   |  获取链表**头** |  查看队**头** |   |   |  
| peekLast      |   |  获取链表**尾** |  查看队**尾** |   |   |  
| poll          |   |  获取并移除链表**头** |  **队头**出队 | 出队 |   |  
| pollFirst     |   |  获取并移除链表**头** |  **队头**出队 |   |   |  
| pollLast      |   |  获取并移除链表**尾** |  **队尾**出队 |   |   |  
| get           |  获取指定位置的元素 |  获取指定位置的元素 |   |   |  根据 Key 取 Value |  
| getOrDefault  |   |   |   |   |  根据 Key 取 Value，没有则返回指定值 |  
| remove        |  移除某元素 |  移除某元素 |  移除某元素 | 移除某元素 |  根据 Key 移除某元素 |  移除某元素
| contains      |  是否包含某元素 |  是否包含某元素 |  是否包含某元素 | 是否包含某个元素 |   |  是否包含某元素
| containsKey   |   |   |   |   |  是否包含此 Key |  
| containsValue |   |   |   |   |  是否包含此 Value |  
| size          |  大小 |  大小 |  大小 | 大小 |  大小 |  大小
| clear         |  清空 |  清空 |  清空 | 清空 |   |  清空
| isEmpty       |  判空 |  判空 |  判空 | 判空 |  判空 |  判空
| toArray       |  返回数组 |  返回数组 |  返回数组 | 返回数组 |  |  返回数组
| values        |   |   |   |   |  返回 Value 列表 |  
| KeySet        |   |   |   |   |  返回 Key 的 Set |  
| entrySet      |   |   |   |   |  返回 Entry 的 Set |  

## List

## ArrayList

|||
|---|---|
| addAll      | 添加 Collection
| add         | 添加
| get         | 返回指定位置上的元素
| remove      | 移除某元素
| contains    | 是否包含
| size        | 大小
| clear       | 清空
| isEmpty     | 判空
| toArray     | 返回数组

## LinkedList

|||
|---|---|
| addAll      | 添加 Collection
| add         | 添加到链表**尾**
| addFirst    | 添加到链表**头**
| addLst      | 添加到链表**尾**
| peek        | 获取链表**头**
| peekFirst   | 获取链表**头**
| peekLast    | 获取链表**尾**
| poll        | 获取并移除链表**头**
| pollFirst   | 获取并移除链表**头**
| pollLast    | 获取并移除链表**尾**
| contains    | 是否包含某元素
| size        | 大小
| clear       | 清空
| isEmpty     | 判空
| toArray     | 返回数组

## Queue

### PriorityQueue

|||
|---|---|
| add   | 入队
| offer | 入队
| peek  | 队头
| poll  | 出队
| remove(Object o) | 移除某个元素
| contains | 是否包含
| size     | 大小
| clear    | 清空
| isEmpty  | 判空
| toArray  | 返回数组

### ArrayDeque

|||
|---|---|
| add       | 入队**头**
| addFirst  | 入队**头**
| addLast   | 入队**尾**
| peek      | 查看队**头**
| peekFirst | 查看队**头**
| peekLast  | 查看队**尾**
| poll      | **队头**出队
| pollFirst | **队头**出队
| pollLast  | **队尾**出队
| remove    | 移除某个元素
| contains  | 是否包含
| size      | 大小
| clear     | 清空
| isEmpty   | 是否为空
| toArray   | 返回数组

## Map

### HashMap

|||
|---|---|
| putAll(Map<> m) | 添加 map
| put             | 添加
| get             | 根据 Key 取 Value
| getOrDefault    | 根据 Key 取 Value，没有则返回指定值
| remove          | 根据 Key 移除某个元素
| containsKey     | 是否包含此 Key
| containsValue   | 是否包含此 Value
| size            | 大小
| isEmpty         | 判空
| values          | 返回 Value 列表
| keySet          | 返回 Key 的 Set
| entrySet        | 返回 Entry 的 Set

### Map.Entry

|||
|---|---|
| setValue | 重设 Value
| getKey   | 获取 Key
| getValue | 获取 Value

## set

|||
|---|---|
| addAll      | 添加 Collection
| add         | 添加
| remove      | 移除某元素
| removeAll   | **移除**和 Collection 的交集
| retainAll   | **保留**和 Collection 的交集
| contains    | 是否包含某元素
| containsAll | 是否包含 Collection
| size        | 大小
| isEmpty     | 判空
| clear       | 清空
| toArray     | 转换为数组

## 字符串

### String

|||
|---|---|
| charAt      | 按位置取字符
| contains    | 是否包含
| startsWith  | 是否以 P 串**开头**
| endsWith    | 是否以 P 串**结尾**
| intern      | 存入堆中
| isEmpty     | 判空
| length      | 长度
| subString   | 返回子串（左闭右开）
| toCharArray | 返回数组
| toLowerCase | 全部小写
| toUpperCase | 全部大写
| trim        | 去除两端空格
| valueOf     | 变为字符串

### StringBuilder

|||
|---|---|
| append       | 添加
| charAt       | 按位置取字符
| delete       | 删除末尾
| deleteCharAt | 按位置删除
| reverse      | 反转
| length       | 长度
| subString    | 返回字串
| toString     | 返回 String
