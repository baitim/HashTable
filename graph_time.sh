#!/bin/bash

gnuplot << EOP

datafile = "output_file_time_3.txt"

set terminal jpeg size 640,480
set output "output_file_time_3.jpg"

plot datafile with lines

EOP