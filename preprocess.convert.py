#!/usr/bin/env python3

from cv2 import imread, imwrite, resize
import os
from scipy.io import loadmat

def main():
    src_img_name = "/mnt/f/BaiduNetdiskDownload/generated_mix/img%d.jpg"
    dst_img_name = "/mnt/f/DL/total-text/preprocess.generated_mix/%08d.png"
    src_poly_name = "/mnt/f/BaiduNetdiskDownload/generated_mix/poly_gt_img%d.mat"
    dst_poly_name = "/mnt/f/DL/total-text/preprocess.generated_mix/%08d.poly"

    fileindex = range(20000)

    def getminfile():
        i = 0
        while True:
            if os.path.exists(dst_img_name%(i,)):
                i += 1
            else:
                yield i
    getminfile = getminfile()

    def process(i):
        dstindex = next(getminfile)
        src_img = src_img_name%(i,)
        dst_img = dst_img_name%(dstindex,)
        src_poly = src_poly_name%(i,)
        dst_poly = dst_poly_name%(dstindex,)

        if not os.path.exists(src_img):
            return

        try:
            print(i)
            img = imread(src_img)
            assert img.shape[0] % 256 == 0 and img.shape[1] % 256 == 0
            height = img.shape[0]
            width = img.shape[1]
            dst_height = round(height/256)*256 or 256
            dst_width = round(width/256)*256 or 256
            # img = resize(img, (dst_width, dst_height))
            imwrite(dst_img, img)

            polygons = loadmat(src_poly)['polygt']
            with open(dst_poly, 'w') as f:
                for polygon in polygons:
                    assert len(polygon[1][0]) == len(polygon[3][0])
                    f.write("n\n")
                    for x, y in zip(polygon[1][0], polygon[3][0]):
                        f.write("p {}, {}\n".format(x/width*dst_width, y/height*dst_height))
                    f.write("e\n")
        except Exception as ex:
            print(ex)
            try:
                os.remove(dst_img)
                os.remove(dst_poly)
            except Exception:
                pass

    for i in fileindex:
        process(i)

if __name__ == '__main__':
    main()