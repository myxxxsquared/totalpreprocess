
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
