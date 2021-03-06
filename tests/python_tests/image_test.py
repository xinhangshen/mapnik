#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, mapnik
from nose.tools import eq_,raises
from utilities import execution_path, run_all

def setup():
    # All of the paths used are relative, if we run the tests
    # from another directory we need to chdir()
    os.chdir(execution_path('.'))

def test_image_premultiply():
    im = mapnik.Image(256,256)
    eq_(im.premultiplied(),False)
    im.premultiply()
    eq_(im.premultiplied(),True)
    im.demultiply()
    eq_(im.premultiplied(),False)

@raises(RuntimeError)
def test_negative_image_dimensions():
    # TODO - this may have regressed in https://github.com/mapnik/mapnik/commit/4f3521ac24b61fc8ae8fd344a16dc3a5fdf15af7
    im = mapnik.Image(-40,40)
    # should not get here
    eq_(im.width(),0)
    eq_(im.height(),0)

def test_jpeg_round_trip():
    filepath = '/tmp/mapnik-jpeg-io.jpeg'
    im = mapnik.Image(255,267)
    im.background = mapnik.Color('rgba(1,2,3,.5)')
    im.save(filepath,'jpeg')
    im2 = mapnik.Image.open(filepath)
    im3 = mapnik.Image.fromstring(open(filepath,'r').read())
    eq_(im.width(),im2.width())
    eq_(im.height(),im2.height())
    eq_(im.width(),im3.width())
    eq_(im.height(),im3.height())
    eq_(len(im.tostring()),len(im2.tostring()))
    eq_(len(im.tostring('jpeg')),len(im2.tostring('jpeg')))
    eq_(len(im.tostring()),len(im3.tostring()))
    eq_(len(im.tostring('jpeg')),len(im3.tostring('jpeg')))

def test_png_round_trip():
    filepath = '/tmp/mapnik-png-io.png'
    im = mapnik.Image(255,267)
    im.background = mapnik.Color('rgba(1,2,3,.5)')
    im.save(filepath,'png')
    im2 = mapnik.Image.open(filepath)
    im3 = mapnik.Image.fromstring(open(filepath,'r').read())
    eq_(im.width(),im2.width())
    eq_(im.height(),im2.height())
    eq_(im.width(),im3.width())
    eq_(im.height(),im3.height())
    eq_(len(im.tostring()),len(im2.tostring()))
    eq_(len(im.tostring('png')),len(im2.tostring('png')))
    eq_(len(im.tostring('png8')),len(im2.tostring('png8')))
    eq_(len(im.tostring()),len(im3.tostring()))
    eq_(len(im.tostring('png')),len(im3.tostring('png')))
    eq_(len(im.tostring('png8')),len(im3.tostring('png8')))

def test_image_open_from_string():
    filepath = '../data/images/dummy.png'
    im1 = mapnik.Image.open(filepath)
    im2 = mapnik.Image.fromstring(open(filepath,'rb').read())
    eq_(im1.width(),im2.width())
    length = len(im1.tostring())
    eq_(length,len(im2.tostring()))
    eq_(len(mapnik.Image.fromstring(im1.tostring('png')).tostring()),length)
    eq_(len(mapnik.Image.fromstring(im1.tostring('jpeg')).tostring()),length)
    eq_(len(mapnik.Image.frombuffer(buffer(im1.tostring('png'))).tostring()),length)
    eq_(len(mapnik.Image.frombuffer(buffer(im1.tostring('jpeg'))).tostring()),length)

    # TODO - https://github.com/mapnik/mapnik/issues/1831
    eq_(len(mapnik.Image.fromstring(im1.tostring('tiff')).tostring()),length)
    eq_(len(mapnik.Image.frombuffer(buffer(im1.tostring('tiff'))).tostring()),length)

if __name__ == "__main__":
    setup()
    exit(run_all(eval(x) for x in dir() if x.startswith("test_")))
