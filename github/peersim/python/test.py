# -*- coding: utf-8 -*-
import numpy
import pylab
import matplotlib as mpl
import random
import math
from math import pow
from math import ceil
mpl.rcParams['font.sans-serif'] = ['SimHei']
prog_total1 = []
prog_incremental1 = []
prog_total = []
prog_incremental = []
prog_remove1=[]
prog_remove2=[]
isOnlineCompareTest = 0
isEnglishVersion = 0
filename="./twofactor.txt"
f = open(filename,"r")
#online rate
y = [0.40,0.58,0.77,0.91,0.99,0.995,0.985,0.974,0.957,0.965,0.97,0.978,0.985,0.91,0.8,0.71,0.62,0.51,0.38,0.28,0.21,0.20,0.22,0.3]
def readProgResult(filename):
	global prog_incremental
	global prog_total
	f=open(filename)
	for line in f:
		index = line.find("remove = ")+len("remove = ")
		end = line.find("]",index)
		prog_remove1.append(int(line[index:end]))
		index = line.find("incrementalVisited",end)+ len("incrementalVisited") + 3
		end = line.find("]",index)
		prog_incremental.append(int(line[index:end]))
		index = line.find("totalVisited",end) + len("totalVisited") + 3
		end = line.find("]",index)
		prog_total.append(int(line[index:end]))

def testFreeScale():
	random.seed()
	u=0.06
	r=0.05
	f = open("./scalefree.txt")
	network=[]
	for line in f:
		array=[]
		line = line.strip("\n,")
		nums = line.split(",")
		for num in nums:
			array.append(int(num))
		network.append(array)
	size = len(network)*1.0
	Susceptable = size
	incremental=[1]
	total=[1]
	delay =1
	vt=[[0]]#store node id of infectious nodes at time t
	s = set()#store the node id of infectious nodes
	for i in range(100):	
		onrate = y[(i-2)%len(y)]
		onrate = 1
		for j in range(delay):
			incremental.append(0)
			vt.append([])
		if Susceptable > 0:	
			cur_vt=vt[i]#node id list at time i
			for v in cur_vt:#node id in list
				prob = 1-math.pow((1-1/size),len(network[v]))
				#prob = 1-math.pow((1-len(network[v])/size),len(network[v]))
				incre = (prob*Susceptable*0.4*onrate)
				k = 0
				for j in range(len(network[v])):
					node = network[v][j]
					d = random.randint(1,delay)
					if node not in s:
						rnd = random.random()
						s.add(node)
						vt[i+d].append(node)
						k+=1
						incremental[i+d]+=1
					if k >=incre:
						break
				Susceptable -=k* onrate
			if(i >1):
				incremental[i+1]*=(1-u)
				Susceptable -= Susceptable * u * onrate
			vt[i]=[]
		if i > 1:
			total.append(total[i]+incremental[i+1]- total[i]*r*onrate)
		else:
			total.append(total[i]+incremental[i+1])
	#array.sort()
	#print incremental
	#print total	
	pylab.figure()
	if isEnglishVersion == 0:
		pylab.plot(incremental,'b*--',label=u'每个时间间隔新增值_理论值',linewidth="1")
		pylab.plot(total,'r*--',label=u'总感染数量_理论值',linewidth="1")
		pylab.plot(prog_incremental,'g*--',label=u'每个时间间隔新增值_模拟值',linewidth="1")
		pylab.plot(prog_total,'c*--',label=u'总感染数量_模拟值',linewidth="1")
		pylab.legend(loc = 0)
		pylab.xlabel(u'时间(一次感染的时间间隔)')
		pylab.ylabel(u'被感染节点数')
		str =u"节点总数=%d"%size
	else:
		pylab.plot(incremental,'b*--',label=u'incremental_theory',linewidth="1")
		pylab.plot(total,'r*--',label=u'total_theory',linewidth="1")
		pylab.plot(prog_incremental,'g*--',label=u'incrememtal_simulation',linewidth="1")
		pylab.plot(prog_total,'c*--',label=u'total_simulation',linewidth="1")
		pylab.legend(loc = 0)
		pylab.xlabel(u'time(a unit for one infection)')
		pylab.ylabel(u'infectious node number')
		str =u"total node number=%d"%size
	pylab.title(str)
	pylab.show()
	exit(0)
	

def testPowerLaw():
	e = numpy.random.exponential(4,size=10000)
	f = open("./scalefree.txt")
	array=[]
	for line in f:
		line = line.strip("\n,")
		nums = line.split(",")
		array.append(len(nums))
	array.sort()
	e.sort()
	pylab.figure()
	pylab.plot(array,'r-',label=u'2',linewidth="2")
	#pylab.plot(e,'b-',label=u'1',linewidth="2")
	pylab.show()
	exit(0)
	
#test for freescale
readProgResult("./output.txt")
testFreeScale()


def testROC():
	f = open("./roc.txt")
	last=0.0
	guest_correct=[]
	guest_missing=[]
	correct=[0,0.8529340935056788,0.9359556665695916,0.9900927440130828,0.9918848988552611,1]
	missing=[0,0.1970264896121409,0.007050945078051791,0.00906223573240692,0.016661714625282837,1]
	mmc_correct=[0,0.8696431371670512,0.9344015322923901,0.9875305226371559,0.9949511637805506,1]
	mmc_missing=[0,0.20067195392315956,0.1972893287317441,0.010067881059584486,0.008765113249377185,1]
	grid_correct=[0,0.9416793610967987,0.9425250341629516,0.9955392145881404,0.9937414592620802,1]
	grid_missing=[0,0.0051425045139761845,0.004639681850387402,0.0067538225950220555,0.013919045551162206,1]
	i=1
	for line in f:
		nums=line.split(",")
		try:
			if float(nums[0]) < last:
				break
			#correct.append(float(nums[0])*1.0/357112.0)
			#missing.append(float(nums[1])*1.0/87506.0)
			guest_correct.append(i/444618.0)
			guest_missing.append(i/444618.0)
			i+=1
			last = float(nums[0])
		except Exception:
			print nums[0]
	#print guest_correct
	pylab.figure()
	pylab.plot(missing,correct,'r-',label=u'原始',linewidth="2")
	pylab.plot(guest_missing,guest_correct,'b-',label=u'猜测',linewidth="2")
	pylab.plot(mmc_missing,mmc_correct,'g-',label=u'多线程',linewidth="2")
	pylab.plot(grid_missing,grid_correct,'c-',label=u'网格',linewidth="2")
	#pylab.plot(correct,"c^--",label=u"correct")
	pylab.legend(loc = 0)
	pylab.xlabel(u'missing')
	pylab.ylabel(u'correct')
	pylab.show()
	exit(0)

m=19
for line in f:	
	index = line.find("remove = ")+len("remove = ")
	end = line.find("]",index)
	prog_remove1.append(int(line[index:end]))
	index = line.find("incrementalVisited",end)+ len("incrementalVisited") + 3
	end = line.find("]",index)
	prog_incremental1.append(int(line[index:end]))
	index = line.find("totalVisited",end) + len("totalVisited") + 3
	end = line.find("]",index)
	prog_total1.append(int(line[index:end]))
f.close()

if isOnlineCompareTest == 1:
	prog_incremental2=[]
	prog_total2=[]
	filename="./online.txt"
	f = open(filename,"r")
	for line in f:	
		index = line.find("remove = ")+len("remove = ")
		end = line.find("]",index)
		prog_remove2.append(int(line[index:end]))
		index = line.find("incrementalVisited",end)+ len("incrementalVisited") + 3
		end = line.find("]",index)
		prog_incremental2.append(int(line[index:end]))
		index = line.find("totalVisited",end) + len("totalVisited") + 3
		end = line.find("]",index)
		prog_total2.append(int(line[index:end]))
	f.close()

	pylab.figure()
	if isEnglishVersion == 0:
		#pylab.plot(prog_total1,'r*--',label=u'总感染数量_无在线率')
		#pylab.plot(prog_total2,"c^--",label=u"总感染数量_有在线率")
		
		#pylab.plot(prog_incremental1,'bo--',label=u'每个时间间隔新增值_无在线率')
		#pylab.plot(prog_incremental2,"go--",label=u"每个时间间隔新增值_有在线率")
		pylab.plot(prog_remove1,'bo--',label=u'总被移除数量_无在线率')
		pylab.plot(prog_remove2,"go--",label=u"总被移除数量_有在线率")
	
		#pylab.plot(prog_total1,'r*--',label=u'总感染数量_Kad')
		#pylab.plot(prog_total2,"c^--",label=u"总感染数量_Chord")
		#pylab.plot(prog_incremental1,'bo--',label=u'每个时间间隔新增值_Kad')
		#pylab.plot(prog_incremental2,"go--",label=u"每个时间间隔新增值_Chord")
		#pylab.plot(prog_remove1,'bo--',label=u'总被移除数量_Kad')
		#pylab.plot(prog_remove2,"go--",label=u"总被移除数量_Chord")
		
		pylab.xlabel(u'时间(一次感染的时间间隔)')
		#pylab.ylabel(u'被感染节点数')
		pylab.ylabel(u'被移除节点数量')
		str =u"节点总数=%d"%pow(2,m)
		pylab.title(str)
	else:
		#pylab.plot(prog_total1,'r*--',label=u'total_without_onlinerate')
		#pylab.plot(prog_total2,"c^--",label=u"total_with_onlinerate")
		
		#pylab.plot(prog_incremental1,'bo--',label=u'每个时间间隔新增值_无在线率')
		#pylab.plot(prog_incremental2,"go--",label=u"每个时间间隔新增值_有在线率")
		#pylab.plot(prog_remove1,'bo--',label=u'removed_without_onlinerate')
		#pylab.plot(prog_remove2,"go--",label=u"removed_with_onlinerate")
		
		pylab.plot(prog_total1,'r*--',label=u'total_Kad')
		pylab.plot(prog_total2,"c^--",label=u"total_Chord")
		#pylab.plot(prog_incremental1,'bo--',label=u'每个时间间隔新增值_Kad')
		#pylab.plot(prog_incremental2,"go--",label=u"每个时间间隔新增值_Chord")
		#pylab.plot(prog_remove1,'bo--',label=u'总被移除数量_Kad')
		#pylab.plot(prog_remove2,"go--",label=u"总被移除数量_Chord")
		pylab.xlabel(u'time(a unit for one infection)')
		pylab.ylabel(u'infectious node number')
		#pylab.ylabel(u'removed node number')
		str =u"total node number=%d"%pow(2,m)
		pylab.title(str)
		
	pylab.legend(loc = 0)
	pylab.title(str)
	pylab.show()
	exit(0)

if 1:
	prog_total = prog_total1	
	prog_incremental = prog_incremental1
else:
	prog_total[0:] = prog_total1[17:25]
	prog_total[len(prog_total):] = prog_total1[0:17]
	prog_total[len(prog_total):] = prog_total1[25:]
	prog_incremental[0:] = prog_incremental1[17:25]
	prog_incremental[len(prog_incremental):] = prog_incremental1[0:17]
	prog_incremental[len(prog_incremental):] = prog_incremental1[25:]
	
T  = 97
m = 19
nodesnum = pow(2,m)

#test for exponential distribution. not necessary here
z=[]
nodesnum = pow(2,m)
for i in range(m):
	z.append(0)
e = numpy.random.exponential(1.5,size=nodesnum)
for i in range(len(e)):
	if e[i] <0:
		continue
	if e[i] >=m:
		continue
	z[int(e[i])]+=1
z.reverse()
if 0:
	print z
	pylab.figure()
	pylab.plot(z,"k-")
	pylab.show()
	exit(0)
#test for exponential distribution. not necessary here


total=[1]
incremental=[1]

random.seed();
i =0
u =0.06
r = 0.05
def kad_twofactor():
	hasvisited = 1
	w =1 
	l = 16
	global m
	global u
	global r
	u =0.06
	r =0.05
	m = 19
	while l < m :
		l+=1
		w += 0.1
	delay =1
	global total
	global incremental
	d = 0
	while d<delay:
		incremental.append(0)
		d+=1
	i = 0
	#T=77
	while i < T:
		g = i % len(y)
		onrate =y[g]
		onrate =1.0
		canspread = incremental[i]
		incre = 0.0
		j = 0
		#if  i > 1 and incremental[i-1] != 0 :
			#hasvisited = (hasvisited *w+(   )  * incremental[i-1]+ total[i-2])  *(1-w))
		if 1:
			hasvisited = total[i-1]*w
		if incremental[i-1] != 0 and 0:
			hasvisited = (1-w)*(total[i-2] + incremental[i]*incremental[i]*1.0 / (m * incremental[i-1] * (1-u)))
		while j < canspread: 
			k = 0
			t = 0
			while k < m:
				prob = 0
				if i > 0:			
					prob = hasvisited * 1.0 / pow(2,k)
				if prob > 1:
					prob = 1
				prob = 1 - prob
				k+=1
				t += prob
			hasvisited += t/m * onrate
			incre +=t 
			j+=1
		if i > 0:
			incre *= 1.0*(1-u*onrate)
		#incre *= onrate
		tempdelay  = (1+delay)/2
		#tempdelay = random.randint(1,delay)
		d = 0
		while d < tempdelay:
			delonrate= y[(i+d+1)%len(y)]
			delonrate = 1
			if i+d+1 >= len(incremental):
					incremental.append(1.0*incre/tempdelay*delonrate)
			else:
				incremental[i+d+1] += 1.0*incre/tempdelay*delonrate
			d+=1
		#incremental.append(incre*(1-u));
		if i > 0:
			total.append(total[i]+incremental[i+1]*onrate-total[i]*r*onrate)
		else:
			total.append(total[i]+incremental[i+1])
		i+=1

#work for both two factor and online 
def kad_twofactor_online():
	hasvisited = 1
	w =0.95	
	l = 14	#w = 14 for no delay ,l= 16 for with delay
	global m
	global u
	global r
	u =0.06
	r =0.05
	m = 19
	while l < m :
		l+=1
		w += 0.04	#0.04 for no delay and 0.05 for with delay
	delay =1
	global total
	global incremental
	d = 0
	while d<delay:
		incremental.append(0)
		d+=1
	i = 0
	#T=77
	while i < T:
		g = i % len(y)
		onrate =y[g]
		#onrate =1.0
		canspread = incremental[i]
		incre = 0.0
		j = 0
		#if  i > 1 and incremental[i-1] != 0 :
			#hasvisited = (hasvisited *w+(   )  * incremental[i-1]+ total[i-2])  *(1-w))
		if 1:
			hasvisited = total[i-1]*w
		if incremental[i-1] != 0 and 0:
			hasvisited = (1-w)*(total[i-2] + incremental[i]*incremental[i]*1.0 / (m * incremental[i-1] * (1-u)))
		while j < canspread: 
			k = 0
			t = 0
			while k < m:
				prob = 0
				if i > 0:			
					prob = hasvisited * 1.0 / pow(2,k)
				if prob > 1:
					prob = 1
				prob = 1 - prob
				k+=1
				t += prob
			hasvisited += t/m * onrate
			incre +=t 
			j+=1
		if i > 0:
			incre *= 1.0*(1-u*onrate)
		#incre *= onrate
		tempdelay  = (1+delay)/2
		#tempdelay = random.randint(1,delay)
		d = 0
		while d < tempdelay:
			delonrate= y[(i+d+1)%len(y)]
			delonrate = 1
			if i+d+1 >= len(incremental):
					incremental.append(1.0*incre/tempdelay*delonrate)
			else:
				incremental[i+d+1] += 1.0*incre/tempdelay*delonrate
			d+=1
		#incremental.append(incre*(1-u));
		if i > 0:
			total.append(total[i]+incremental[i+1]*onrate-total[i]*r*onrate)
		else:
			total.append(total[i]+incremental[i+1])
		i+=1
def chord():
	i = 0
	global m
	print u
	m =20
	global total
	delay = 11
	global incremental
	while i < T:
		if i>=m:
			incremental.append(0)
		else:
			#maybe we can modify the dividor (i+1) because not all the emunerations can be used in the last step propagation
			#just use it for now.if i have more time i can modify it later
			incremental.append(incremental[i] * (m-(i)) / (i+1))
		total.append(incremental[i+1]+total[i])
		i+=1
	
def chord_twofactor():
	i = 0
	global m
	m =21
	global total
	global incremental
	while i < T:
		g  = i%len(y)
		if i>=m:
			incremental.append(0)
		else:
			#maybe we can modify the dividor (i+1) because not all the emunerations can be used in the last step propagation
			#just use it for now.if i have more time i can modify it later
			if i > 0:
				incremental.append(incremental[i] *(m-(i*(1-u/i))) / (i+1) *(1-u))
			else:
				incremental.append(incremental[i] *(m) / (i+1) *(1-u))
		if i >0:
			total.append(incremental[i+1]+total[i]*(1-r))
		else:
			total.append(incremental[i+1]+total[i])
		i+=1
		
def chord_twofactor_online():
	i = 0
	global m
	m = 21
	global total
	global incremental
	while i < T:
		g  = (i-1)%len(y)
		if i>=m:
			incremental.append(0)
		else:
			#maybe we can modify the dividor (i+1) because not all the emunerations can be used in the last step propagation
			#just use it for now.if i have more time i can modify it later
			if i > 0:
				incremental.append(incremental[i]*(m-(i*(1-u*y[g]/i))) / (i+1) *(1-u*y[g]))
			else:
				incremental.append(incremental[i] *(m) / (i+1) *(1-u))
		if i > 0:
			total.append(incremental[i+1]*y[g]+total[i]-total[i]*r*y[g])
		else:
			total.append(incremental[i+1]+total[i])
		i+=1

#random network
def twofactor_onlinerate():
	susceptible = 100000
	b=1.0/80000
	i = 0
	T =100
	u=0.006
	r=0.005
	onrate = 0
	while i < T:
		#b = 1.0/susceptible
		incre = 0
		if i>=0:
			onrate = y[(i-0)%len(y)]
		else:
			onrate = 1
		if susceptible > 0:			
			incre = b*total[i]*onrate*onrate*(susceptible)-total[i]*r*onrate
			print b*total[i]*onrate*onrate*(susceptible)
		else:
			incre -=total[i]*r*onrate
			#incre = 0
		print incre,susceptible
		incremental.append(incre)
		total.append(total[i]+incre)
		if susceptible > 0:
			susceptible -= (b*total[i]*onrate*onrate*(susceptible) + susceptible * u*onrate)
		i+=1

#nodesnum = 100000

#kad_time() #deprecated
#chord_time() #deprecated
#chord_online()#deprecated


#kad_twofactor_online()#done w = 0.95 for with delay and w=1 for no delay
#kad_twofactor()#done #work for no delay
#chord() #done
#chord_twofactor()#done
#twofactor_onlinerate()#done
chord_twofactor_online()#done


nodesnum = pow(2,m)
print "total = ", len(total)
print "prog_total",len(prog_total)

print "incremental = ", len(incremental)
print "prog_incremental", len(prog_incremental)

print total
print prog_total

i = 0
y=[]
for i in range(len(total)):
	g = (i)
	y.append(g)
	
k = len(y)
incrlen = len(incremental)
print len(y),len(incremental)
while k < incrlen:
	incremental.pop();
	k+=1

pylab.figure()
if isEnglishVersion == 0:
	pylab.plot(y,total,'r*--',label=u'总感染数量_理论值')
	pylab.plot(y,prog_total,"c^--",label=u"总感染数量_模拟值")
	pylab.plot(y,incremental,'b*--',label=u'每个时间间隔新增值_理论值')
	pylab.plot(y,prog_incremental,"g^--",label=u"每个时间间隔新增值_模拟值")
	pylab.xlabel(u'时间(一次感染的时间间隔)')
	pylab.ylabel(u'被感染节点数')
	str =u"节点总数=%d"%nodesnum
	pylab.title(str)
else:
	pylab.plot(y,total,'r*--',label=u'total_theory')
	pylab.plot(y,prog_total,"c^--",label=u"total_simulation")
	pylab.plot(y,incremental,'b*--',label=u'incremental_theory')
	pylab.plot(y,prog_incremental,"g^--",label=u"incrememtal_simulation")
	pylab.xlabel(u'time(a unit for one infection)')
	pylab.ylabel(u'infectious node number')
	str =u"total node number=%d"%nodesnum
	pylab.title(str)
	
pylab.legend(loc = 0)
pylab.title(str)
pylab.show()
