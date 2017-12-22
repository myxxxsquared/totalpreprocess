#!/usr/bin/env python3

import os
from itertools import count


def main():
    image_file_name = "/mnt/f/DL/total-text/Images/Train/img%d.jpg"
    poly_file_name = "/mnt/f/DL/total-text/Groundtruth/Polygon/Train/poly_gt_img%d.mat.output"
    filenumber = range(2000)
    binfile = "/mnt/z/source/thin/build/totalpreprocess"
    output_file_name = "/mnt/f/DL/total-text/preprocess/"

    outputfile = "/mnt/f/DL/total-text/preprocess/%08d.poly"

    def getminfile():
        for i in count():
            if not os.path.exists(outputfile%(i,)):
                return i

    for i in filenumber:
        image_file = image_file_name%(i,)
        poly_file = poly_file_name%(i,)
        if not os.path.exists(image_file):
            continue
        print(i)
        print(getminfile())
        os.system("%s resize 512 %s %s %s %d"%(binfile, image_file, poly_file, output_file_name, getminfile()))


if __name__ == "__main__":
    main()