from math import sqrt
import random
def sim_distance(data1,data2):
	return sqrt(sum([pow(data1[i] - data2[i],2) for i in range(len(data1))]))

def scaledown(data,distance=sim_distance,rate=0.01):
	n = len(data)
	realdist = [[distance(data[i],data[j]) for j in range(n)] for i in range(n)]
	outersum = 0.0
	loc = [[random.random(),random.random()] for i in range(n)]
	fakedist = [[0.0 for j in range(n)] for i in range(n)]
	lasterror = None
	for m in range(0,1000):
		for i in range(n):
			for j in range(n):
				fakedist[i][j] = sim_distance(loc[i],loc[j])
		grad=[[0.0,0.0] for i in range(n)]
		totalerror=0
		for k in range(n):
			for j in range(n):
				if j == k:continue
				#my version
				errorterm = (fakedist[k][j]-realdist[k][j])/fakedist[k][j]
				grad[k][0] += ((loc[k][0]-loc[j][0]))*errorterm
				grad[k][1] += ((loc[k][1]-loc[j][1]))*errorterm
				
				"""
				#book version
				errorterm = (fakedist[k][j] - realdist[k][j])/realdist[j][k]
				grad[k][0] += ((loc[k][0]-loc[j][0])/fakedist[j][k])*errorterm
				grad[k][1] += ((loc[k][1]-loc[j][1])/fakedist[j][k])*errorterm
				"""
				totalerror+=abs(errorterm)
		#print totalerror
		#if lasterror and lasterror<totalerror:break
		if lasterror and lasterror<0.01:break
		lasterror = totalerror

		for k in range(n):
			loc[k][0]-=rate*grad[k][0]
			loc[k][1]-=rate*grad[k][1]

	print "m=",m
	return loc

data=[[1,2],[2,3],[3,2],[5,3]]	#it seems that my version works better in this case
loc = scaledown(data)

for i in range(len(data)):
	for j in range(len(loc)):
		if i == j:continue
		dis1=sim_distance(data[i],data[j])
		dis2=sim_distance(loc[i],loc[j])
		print dis1,dis2

