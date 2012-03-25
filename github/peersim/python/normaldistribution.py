# -*- coding: utf-8 -*-
import numpy
import pylab
import matplotlib as mpl
import random
import math
from math import pow
from math import ceil
k=[]
x=[]
length = 4
span = 0.2
rangesize = int(length * 1/span * 2 + 2)
startindex = length * -1
for i in range(rangesize):
	k.append(0)
	x.append(startindex+i*0.2)
z = numpy.random.normal(0,1,size=1000000)
for i in range(len(z)):
	g = int((z[i] + length)/span)+1
	if g<0:
		g= 0
	if g>=rangesize:
		g = rangesize -1
	k[g]+=1
pylab.figure()
pylab.plot(x,k)
pylab.show()