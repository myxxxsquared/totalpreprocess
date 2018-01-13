#!/usr/bin/env python3

import os
from itertools import count

def main():
    image_file_name = "/mnt/f/BaiduNetdiskDownload/seglink_on_curve/generated/img%d.jpg"
    poly_file_name = "/mnt/f/BaiduNetdiskDownload/seglink_on_curve/generated/poly_gt_img%d.mat.output"
    filenumber = range(5000)
    binfile = "/mnt/z/source/thin/build/totalpreprocess"
    output_file_name = "/mnt/f/DL/total-text/preprocess.generated/"

    outputfile = "/mnt/f/DL/total-text/preprocess.generated/%08d.poly"

    def getminfile():
        i = 0
        while True:
            if os.path.exists(outputfile%(i,)):
                i += 1
            else:
                yield i
    getminfile = getminfile()

    for i in filenumber:
        image_file = image_file_name%(i,)
        poly_file = poly_file_name%(i,)
        if not os.path.exists(image_file):
            continue
        curminfile = next(getminfile)
        print(i, curminfile)
        os.system("%s resize 512 %s %s %s %d"%(binfile, image_file, poly_file, output_file_name, curminfile))


if __name__ == "__main__":
    main()
