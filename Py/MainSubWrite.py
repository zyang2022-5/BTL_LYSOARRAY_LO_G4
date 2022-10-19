#!/usr/bin/env python3

if __name__ == '__main__':
    import sys
    import os
    import numpy as np
    from argparse import ArgumentParser

    parser = ArgumentParser(description='Create input file for NSGAII')
    parser.add_argument('-o','--output', default=None, help='output file name', required=True)
    parser.add_argument('-pop','--RealN', type=int, default=1, help='Number of real variables')
    parser.add_argument('-RealN','--RealN', type=int, default=1, help='Number of real variables')
    args = parser.parse_args()

    f = open("demofile2.txt", "a")
    f.write("Now the file has more content!")
    f.close()
