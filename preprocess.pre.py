#!/usr/bin/env python3

import os
from itertools import count
import multiprocessing
from cv2 import imread

def processfile(i):
    print(i)

    binfile = "/mnt/z/source/thin/build/totalpreprocess"
    poly_file_name = "/mnt/f/DL/total-text/preprocess.generated_mix/%08d.poly"
    image_file_name = "/mnt/f/DL/total-text/preprocess.generated_mix/%08d.png"
    output_file_name = "/mnt/f/DL/total-text/preprocess.generated_mix/%08d"
    output_image_name = "/mnt/f/DL/total-text/preprocess.generated_mix/%08d_o.png"

    poly_file = poly_file_name%(i,)
    image_file = image_file_name%(i,)
    output_file = output_file_name%(i,)
    output_image = output_image_name%(i,)

    if not os.path.exists(image_file):
        return

    img = imread(image_file)
    height = img.shape[0]
    width = img.shape[1]

    os.system("%s generator %d %d %s %s"%(binfile, height, width, poly_file, output_file))
    os.system("%s viewer %d %d %s %s %s %s"%(binfile, height, width, poly_file, output_file, image_file, output_image))


def main():
    filenumber = range(0, 5000)
    pool = multiprocessing.Pool(6)
    pool.map(processfile, filenumber)

    # for i in filenumber:
    #     print(i)
    #     poly_file = poly_file_name%(i,)
    #     image_file = image_file_name%(i,)
    #     output_file = output_file_name%(i,)
    #     output_image = output_image_name%(i,)
    #     os.system("%s generator 512 512 %s %s"%(binfile, poly_file, output_file))
    #     os.system("%s viewer 512 512 %s %s %s %s"%(binfile, poly_file, output_file, image_file, output_image))

if __name__ == "__main__":
    main()
