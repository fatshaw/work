# -*- coding: utf-8 -*-
import os
import sys
import logging
import pdb
import json

def traverse_tree(tree,node_name_map,id,link,level):
	if len(link) > level :  
		link[level]=(id,node_name_map[id])
	else:
		link.append((id,node_name_map[id]))

	if id not in tree:
		str=''
		for pair in link:
			if pair[0] == '0':
				continue
			str='%s,%s,%s' % (str,pair[0],pair[1])
		str=str.strip(',')
		print str
	else:
		for cid in tree[id]:
			traverse_tree(tree,node_name_map,cid,link,level+1)
			del link[level+1:-1]

	
	
def parser_tree(filename):
	tree={}
	root='0'
	node_name_map={root:"产品库类目根目录"}
	for line in open(filename,'r'):
		line=line.replace('\n','')
		items=line.split('\t')
		cid=items[0]
		if cid.isdigit() == False:
			continue;
		name=items[1]
		parentid=items[2]
		if parentid in tree:
			tree[parentid].append(cid)
		else:
			tree[parentid]=[cid]
		node_name_map[cid]=name
	
	link=[]
	traverse_tree(tree,node_name_map,root,link,0)

def main():
	if len(sys.argv) < 2:
		print 'Usage : %s tree_map_filename' % sys.argv[0]
		return
    
	logging.basicConfig(filename='%s.log' % sys.argv[0],level=logging.DEBUG)
	#pdb.set_trace()
	parser_tree(sys.argv[1])
	

if __name__ == '__main__':
    main()
