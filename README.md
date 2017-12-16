
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
- `./totalpreprocess`

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
