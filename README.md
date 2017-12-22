
# totalpreprocess

## 依赖

- libz-dev
- libopencv-dev
- libcgal-dev
- libcgal-qt5-dev

## 生成与测试

- `mkdir build`
- `cd build`
- `cmake ..`
- `make`
- `cd ../testdata`
- `./run.sh`

## 输出格式

- `output_%d.npy`
- `shape=[height, width, 19]`
- channels
    - 0 vaild
    - 1 angle
    - 2 deltax
    - 3 deltay
    - 4 width
    - 5 height
    - 6 curvature
    - 7-14 inlayer
    - 15-18 crosslayer

### inlayer channel 顺序

```
0 7 6
1 x 5
2 3 4
```

### crosslayer channel 顺序

```
0 2
1 3
```

## 使用方法

### 预处理

1. 将图像和多边形Groundtruth处理成每个边长为256的整数倍。
2. 在多边形的目录下使用matlab运行`matlab/convert.m`，将matlab格式转化为文本格式。
3. 运行编译好的`totalpreprocess`程序，格式为`totalpreprocess generator [图像高度] [图像宽度] [文本格式的多边形文件] [输出文件]`。

### 结果查看

- 运行编译好的`totalpreprocess`程序，格式为`totalpreprocess viewer [图像高度] [图像宽度] [文本格式的多边形文件] [预处理输出结果] [输入图像文件] [输出图像文件]`。
