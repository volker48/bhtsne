#!/usr/bin/env python

'''
A simple Python wrapper for the bh_tsne binary that makes it easier to use it
for TSV files in a pipeline without any shell script trickery.

Note: The script does some minimal sanity checking of the input, but don't
    expect it to cover all cases. After all, it is a just a wrapper.

Example:

    > echo -e '1.0\t0.0\n0.0\t1.0' | ./bhtsne.py -d 2 -p 0.1
    -2458.83181442  -6525.87718385
    2458.83181442   6525.87718385

The output will not be normalised, maybe the below one-liner is of interest?:

    python -c 'import numpy;  from sys import stdin, stdout;
        d = numpy.loadtxt(stdin); d -= d.min(axis=0); d /= d.max(axis=0);
        numpy.savetxt(stdout, d, fmt="%.8f", delimiter="\t")'

Authors:     Pontus Stenetorp    <pontus stenetorp se>
             Philippe Remy       <github: philipperemy>
Version:    2016-03-08
'''

# Copyright (c) 2013, Pontus Stenetorp <pontus stenetorp se>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

from argparse import ArgumentParser, FileType
from os.path import abspath, dirname, isfile, join as path_join
from shutil import rmtree
from struct import calcsize, pack, unpack
from subprocess import Popen
from sys import stderr, stdin, stdout
from tempfile import mkdtemp
from platform import system
from os import devnull
import numpy as np


def _read_unpack(fmt, fh):
    return unpack(fmt, fh.read(calcsize(fmt)))



def read_output(fn):
    with open(fn) as output_file:
        # The first two integers are just the number of samples and the
        #   dimensionality
        result_samples, result_dims = _read_unpack('ii', output_file)
        # Collect the results, but they may be out of order
        results = [_read_unpack('{}d'.format(result_dims), output_file)
            for _ in xrange(result_samples)]
        # Now collect the landmark data so that we can return the data in
        #   the order it arrived
        results = [(_read_unpack('i', output_file), e) for e in results]
        # Put the results in order and yield it
        results.sort()
        for _, result in results:
            yield result

if __name__ == '__main__':
    from sys import argv
    print np.vstack(read_output(argv[1]))
