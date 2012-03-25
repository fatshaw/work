list=[33,2,333,222,22,3331,222]

def insertsort():
	i = 1 
	while i < len(list):
		j = i-1
		tmp = list[i]
		while j >=0 and list[j]>tmp:
			list[j+1] = list[j]
			j-=1
		list[j+1]=tmp  
		i+=1

def bubblesort():
	for i in range(len(list)):
		for j in range(len(list)-i-1):
			if list[j]>list[j+1]:
				tmp = list[j]
				list[j] = list[j+1]
				list[j+1] = tmp

def merge(l,r):
	m = (l+r)/2
	i = l
	j = m+1
	k=0
	ret=[]
	while i<=m and j<=r:
		while i <=m and j <=r and list[i]<=list[j]:
			ret.append(list[i])
			i+=1
		while i <=m and j <=r and list[i] > list[j]:
			ret.append(list[j])
			j+=1
	while i<=m:
		ret.append(list[i])
		i+=1
	while j<=r:
		ret.append(list[j])
		j+=1
	

	for k in range(len(ret)):
		list[l+k]=ret[k]


def mergesort(l,r):
	if l>=r:	
		return
	mergesort(l,(l+r)/2)
	mergesort((l+r)/2+1,r)
	merge(l,r)


def oddEvenSort():
	flag = 0
	span = 2
	i = 0
	while 1:
		flag = 0
		while i < len(list)-1:
			if list[i]>list[i+1]:
				tmp=list[i]
				list[i] = list[i+1]
				list[i+1]=tmp
				flag = 1
			i+=span
		i = (i+1)%2
		if flag == 0:
			break
		
print list
oddEvenSort()
print list


	

