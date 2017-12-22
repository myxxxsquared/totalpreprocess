#!/usr/bin/env python3

import os
from itertools import count

def main():
    binfile = "/mnt/z/source/thin/build/totalpreprocess"
    poly_file_name = "/mnt/f/DL/total-text/preprocess/%08d.poly"
    image_file_name = "/mnt/f/DL/total-text/preprocess/%08d.png"
    output_file_name = "/mnt/f/DL/total-text/preprocess/%08d"
    output_image_name = "/mnt/f/DL/total-text/preprocess/%08d_o.png"
    filenumber = range(4827)

    for i in filenumber:
        print(i)
        poly_file = poly_file_name%(i,)
        image_file = image_file_name%(i,)
        output_file = output_file_name%(i,)
        output_image = output_image_name%(i,)
        os.system("%s generator 512 512 %s %s"%(binfile, poly_file, output_file))
        os.system("%s viewer 512 512 %s %s %s %s"%(binfile, poly_file, output_file, image_file, output_image))

if __name__ == "__main__":
    main()
