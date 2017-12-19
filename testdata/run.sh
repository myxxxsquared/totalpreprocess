#!/bin/sh

../build/totalpreprocess generator 512 512 ground_truth/poly_gt_img11.mat.output output/img11
../build/totalpreprocess generator 512 512 ground_truth/poly_gt_img12.mat.output output/img12
../build/totalpreprocess generator 512 512 ground_truth/poly_gt_img13.mat.output output/img13
../build/totalpreprocess generator 512 512 ground_truth/poly_gt_img14.mat.output output/img14
../build/totalpreprocess generator 512 512 ground_truth/poly_gt_img15.mat.output output/img15
../build/totalpreprocess generator 512 512 ground_truth/poly_gt_img16.mat.output output/img16
../build/totalpreprocess generator 512 512 ground_truth/poly_gt_img17.mat.output output/img17
../build/totalpreprocess generator 512 512 ground_truth/poly_gt_img18.mat.output output/img18
../build/totalpreprocess generator 512 512 ground_truth/poly_gt_img19.mat.output output/img19
../build/totalpreprocess generator 512 512 ground_truth/poly_gt_img20.mat.output output/img20

../build/totalpreprocess viewer 512 512 ground_truth/poly_gt_img11.mat.output output/img11 image/img11.jpg output/output11.png
../build/totalpreprocess viewer 512 512 ground_truth/poly_gt_img12.mat.output output/img12 image/img12.jpg output/output12.png
../build/totalpreprocess viewer 512 512 ground_truth/poly_gt_img13.mat.output output/img13 image/img13.jpg output/output13.png
../build/totalpreprocess viewer 512 512 ground_truth/poly_gt_img14.mat.output output/img14 image/img14.jpg output/output14.png
../build/totalpreprocess viewer 512 512 ground_truth/poly_gt_img15.mat.output output/img15 image/img15.jpg output/output15.png
../build/totalpreprocess viewer 512 512 ground_truth/poly_gt_img16.mat.output output/img16 image/img16.jpg output/output16.png
../build/totalpreprocess viewer 512 512 ground_truth/poly_gt_img17.mat.output output/img17 image/img17.jpg output/output17.png
../build/totalpreprocess viewer 512 512 ground_truth/poly_gt_img18.mat.output output/img18 image/img18.jpg output/output18.png
../build/totalpreprocess viewer 512 512 ground_truth/poly_gt_img19.mat.output output/img19 image/img19.jpg output/output19.png
../build/totalpreprocess viewer 512 512 ground_truth/poly_gt_img20.mat.output output/img20 image/img20.jpg output/output20.png
