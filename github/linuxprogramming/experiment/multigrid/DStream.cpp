#include "DStream.h"
bool Dstream::createGlobalGrid(const string & key, const string & localKey, const string & borderKey, DataPoint & dp) {
	GlobalGrid g(key, dp.getData(), localKey, borderKey);
	pair<hashMapIt, bool> p = globalGridHash.insert(hashMap::value_type(key, g));
	if (p.second == true) {
		p.first->second.addPoint(dp);
		if (borderKey != "")
			borderGridHash.insert(borderHashMap::value_type(key, borderKey));//borderGridHash is not used now
	}
	return p.second;
}

bool Dstream::createLocalGrid(const string & localKey, const string & globalKey, vector<int>&coordinate) {
	LocalGrid localGrid(localKey, globalKey, coordinate);
	pair<localHashMapIt, bool> p = localGridHash.insert(localHashMap::value_type(localKey, localGrid));
	return p.second;
}

bool Dstream::createBorderGrid(const string & borderKey, const string & globalKey) {
	list<string> set;
	set.push_back(globalKey);
	pair<borderHashIt, bool> p = borderGrid.insert(borderHash::value_type(borderKey, set));
	return p.second;
}

bool Dstream::addPoint(DataPoint & dp) {
	bool result = false;
	double timeArrive = dp.getTimeArrive();
	string globalKey;
	string localKey;
	string borderKey;
	vector<int> coordinate;
	//os.writeStream(out1,dp.getData());
	dp.getCoordinate(globalKey, localKey, borderKey, coordinate);
	hashMapIt it = hasGlobalGrid(globalKey);
	if (it == globalGridHash.end()) {
		result = createGlobalGrid(globalKey, localKey, borderKey, dp);
		gridnum++;
	} else {
		hitnum++;
		result = it->second.addPoint(dp);
	}

	localHashMapIt localGridIt = hasLocalGrid(localKey);
	if (localGridIt == localGridHash.end()) {
		createLocalGrid(localKey, globalKey, coordinate);
	} else {
		if (it == globalGridHash.end())
			localGridIt->second.addGlobalGrid(globalKey);
	}

	if (borderKey != "") {
		borderHashIt borderIt = hasBorderGrid(borderKey);
		if (borderIt == borderGrid.end()) {
			createBorderGrid(borderKey, globalKey);
		} else {
			if (it == globalGridHash.end()) {
				borderIt->second.push_back(globalKey);
			}
		}
	}

	if (timeArrive - gap == 0) {
		initCluster(timeArrive);
	} else if ((int) timeArrive % gap == 0 || (int)timeArrive > MAXLINES)  {
		if((int)timeArrive > MAXLINES){
			cout<<"maxlines"<<endl;
		}
		for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end();) {
			if (it->second.getLabel() == SPORADIC) {
				if ((timeArrive - it->second.getLastDeletedTime()) > (1 + beta) * gap) {
					deleteGlobalGrid(it->second);
					globalGridHash.erase(it++);
					gridnum--;
					//cout << "delete grid" << endl;
				} else {
					++it;
				}
			} else {
				++it;
			}
		}
		//adjustCluster(timeArrive);
		//incrementalAdjust(timeArrive);
		//cout<<"global"<<endl;
		//double time = CommonUtil::getMicroSecond();
		cudaGlobalIncrementalAdjust(timeArrive);
		//GlobalIncrementalAdjust(timeArrive);
		//time = CommonUtil::getMicroSecond() - time;
		//cout << "global ajust" << time / 1000 << endl;
	}
	return result;
}

void Dstream::deleteGlobalGrid(GlobalGrid & globalGrid) {
	string localKey = globalGrid.getLocalGridKey();
	string borderKey = globalGrid.getBorderGridKey();
	localHashMapIt localit = hasLocalGrid(localKey);
	borderHashIt borderit = hasBorderGrid(borderKey);
	if (localit != localGridHash.end()) {
		localit->second.deleteGlobalKey(localKey);
	}
	if (borderit != borderGrid.end()) {
		borderit->second.remove(borderKey);
	}
}

int Dstream::incrementalMergeCluster(GlobalGrid & g1, GlobalGrid & g2) {
	int clusterid1 = g1.getClusterId();
	int clusterid2 = g2.getClusterId();
	vector<string> cluster1;
	vector<string> cluster2;
	for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
		if (it->second.getClusterId() == clusterid1)
			cluster1.push_back(it->first);
		else if (it->second.getClusterId() == clusterid2)
			cluster2.push_back(it->first);
	}

	hashMapIt globalIt;

	if (cluster1.size() > cluster2.size()) {
		for (vector<string>::iterator it = cluster2.begin(); it != cluster2.end(); ++it) {
			globalIt = globalGridHash.find(*it);
			globalIt->second.setClusterId(clusterid1);
			globalIt->second.setIsChanged(false);
		}
	} else {
		for (vector<string>::iterator it = cluster1.begin(); it != cluster1.end(); ++it) {
			globalIt = globalGridHash.find(*it);
			globalIt->second.setClusterId(clusterid2);
			globalIt->second.setIsChanged(false);
		}
	}

	return 0;
}
bool Dstream::incrementalExpand(GlobalGrid & g) {
	double distance = 0;
	vector<string> seedlist;
	for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
		if (it->second.getClusterId() == g.getClusterId() || it->second.getGridStatus() == SPARSE)
			continue;
		distance = g.distance(it->second);
		if (distance <= eps && distance > 0) {
			seedlist.push_back(it->second.getGridKey());
			it->second.setClusterId(g.getClusterId());
			it->second.setIsChanged(false);
		}
	}

	for (vector<string>::iterator it = seedlist.begin(); it != seedlist.end(); ++it) {
		if (globalGridHash.find(*it)->second.getGridStatus() == DENSE) {
			incrementalExpand(globalGridHash.find(*it)->second);
		}
	}
	return true;
}

bool Dstream::incrementalRangeQuery(GlobalGrid & g, list<string>&seedlist, int type) {
	double distance = 0;
	seedlist.clear();
	bool flag = false;
	for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
		if (it->second.getGridStatus() == SPARSE)
			continue;
		switch (type) {
		case DENSE_WITH_NOCLASS:
			if (it->second.getClusterId() > UNCLASSIFIED && it->second.getGridStatus() == DENSE) {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					g.setClusterId(it->second.getClusterId());//find dense neighbor then join to it
					//incrementalExpand(g);
					flag = true;
				}
			} else {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					seedlist.push_back(it->second.getGridKey());
				}
			}
			break;
		case DENSE_WITH_CLASS:
			//cout << "dense" << endl;
			if (it->second.getGridStatus() == DENSE && it->second.getClusterId() > UNCLASSIFIED && it->second.getClusterId() != g.getClusterId()) {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					incrementalMergeCluster(g, it->second);
					flag = true;
				}
			} else if (it->second.getClusterId() <= UNCLASSIFIED && it->second.getGridStatus() != SPARSE) {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					it->second.setClusterId(g.getClusterId());
					it->second.setIsChanged(false);
				}
			}
			break;
		case TRANSITIONAL_WITH_CLASS:
			if (it->second.getClusterId() > UNCLASSIFIED && it->second.getGridStatus() == DENSE && it->second.getClusterId() != g.getClusterId()) {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					g.setClusterId(it->second.getClusterId());
					//	cout << "g set id" << endl;
					flag = true;
				}
			}
			break;
		case TRANSITIONAL_WITH_NO_CLASS:
			if (it->second.getClusterId() > UNCLASSIFIED && it->second.getGridStatus() == DENSE) {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					//	cout << "dense set id" << endl;
					g.setClusterId(it->second.getClusterId());
					flag = true;
				}
			} else {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					seedlist.push_back(it->second.getGridKey());
				}
			}
			break;
		}
		if (flag == true)
			break;

	}
	return flag;
}

void Dstream::GlobalIncrementalAdjust(double timeArrive) {
	updateDensity(timeArrive);
	list<string> globalKeyVector;
	hashMapIt it;
	list<string> seedlist;
	for (localHashMapIt lit = localGridHash.begin(); lit != localGridHash.end(); ++lit) {
		globalKeyVector = lit->second.getGlobalKey();
		for (list<string>::iterator git = globalKeyVector.begin(); git != globalKeyVector.end(); ++git) {
			it = hasGlobalGrid(*git);
			if (it == globalGridHash.end())
				continue;
			if (it->second.isChanged() == true) {
				if (it->second.getGridStatus() == DENSE) {
					if (it->second.getClusterId() <= UNCLASSIFIED) {//noise or unclssified
						/*find neigh
						 * if(neigh is dense and it has cluster )join to neigh
						 * else if(neigh has not cluster and neigh size > minpts)then create
						 * else if(neigh has not cluster and neight size < minpts)then as noise;
						 * */
						GlobalIncrementalRangeQuery(it->second, globalKeyVector, seedlist, DENSE_WITH_NOCLASS);
						if (it->second.getClusterId() == UNCLASSIFIED) {
							if ((int) seedlist.size() < minPts) {
								it->second.setClusterId(NOISE);
							} else {
								//		cout << "increCreate" << endl;
								if (localExpandCluster(it->second, seedlist, lit->second, INCREMENTALEXPAND))
									++clusterID;
							}
						}
					} else if (it->second.getClusterId() > UNCLASSIFIED) {
						/*find neigh
						 * if(neigh has cluster and is not the same as this)then join small one to bigger one
						 * else if(neigh has not cluster and has not dense grid but has transitional grid , add transitional grid to this cluster)
						 * */
						GlobalIncrementalRangeQuery(it->second, globalKeyVector, seedlist, DENSE_WITH_CLASS);
					}
				} else if (it->second.getGridStatus() == SPARSE && it->second.getClusterId() > UNCLASSIFIED) {
					//incrementalRangeQuery(it->second, seedlist,
					//	DELETE_SPARSE);
					it->second.setClusterId(UNCLASSIFIED);
					/*set<string>seedSet;
					 if(seedlist.size()!=0){
					 for(list<string>::iterator sit = seedlist.begin();sit!=seedlist.end();++sit){
					 seedSet.insert(*sit);
					 }
					 incrementalIsConnected(seedSet);
					 }*/
				} else if (it->second.getGridStatus() == TRANSITIONAL) {
					if (it->second.getClusterId() <= UNCLASSIFIED) {
						GlobalIncrementalRangeQuery(it->second, globalKeyVector, seedlist, TRANSITIONAL_WITH_NO_CLASS);
						if (it->second.getClusterId() == UNCLASSIFIED) {
							if ((int) seedlist.size() < minPts) {
								it->second.setClusterId(NOISE);
							}
						}
					} else if (it->second.getClusterId() > UNCLASSIFIED) {
						GlobalIncrementalRangeQuery(it->second, globalKeyVector, seedlist, TRANSITIONAL_WITH_CLASS);
					}
				}
			}
		}
	}

	borderCluster(true);
}

bool Dstream::GlobalIncrementalRangeQuery(GlobalGrid & g, list<string>&globalKeyVector, list<string>&seedlist, int type) {
	double distance = 0;
	seedlist.clear();
	bool flag = false;
	hashMapIt it;
	for (list<string>::iterator git = globalKeyVector.begin(); git != globalKeyVector.end(); ++git) {
		it = hasGlobalGrid(*git);
		if (it == globalGridHash.end())
			continue;
		if (it->second.getGridStatus() == SPARSE)
			continue;

		switch (type) {
		case DENSE_WITH_NOCLASS:
			if (it->second.getClusterId() > UNCLASSIFIED && it->second.getGridStatus() == DENSE) {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					g.setClusterId(it->second.getClusterId());
					GlobalIncrementalExpand(g, globalKeyVector);
					flag = true;
				}
			} else {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					seedlist.push_back(it->second.getGridKey());
				}
			}
			break;
		case DENSE_WITH_CLASS:
			if (it->second.getGridStatus() == DENSE && it->second.getClusterId() > UNCLASSIFIED && it->second.getClusterId() != g.getClusterId()) {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					incrementalMergeCluster(g, it->second);
					flag = true;
				}
			} else if (it->second.getGridStatus() == TRANSITIONAL && it->second.getClusterId() == UNCLASSIFIED) {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					it->second.setClusterId(g.getClusterId());
					it->second.setIsChanged(false);
					//	cout << "neighbor transitional set id" << endl;
					flag = true;
				}
			}
			break;
		case TRANSITIONAL_WITH_CLASS:
			if (it->second.getClusterId() > UNCLASSIFIED && it->second.getGridStatus() == DENSE && it->second.getClusterId() != g.getClusterId()) {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					g.setClusterId(it->second.getClusterId());
					//	cout << "g set id" << endl;
					flag = true;
				}
			}
			break;
		case TRANSITIONAL_WITH_NO_CLASS:
			if (it->second.getClusterId() > UNCLASSIFIED && it->second.getGridStatus() == DENSE) {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					//	cout << "dense set id" << endl;
					g.setClusterId(it->second.getClusterId());
					flag = true;
				}
			} else {
				distance = g.distance(it->second);
				if (distance <= eps && distance > 0) {
					seedlist.push_back(it->second.getGridKey());
				}
			}
			break;
		}
		if (flag == true)
			break;

	}
	return flag;
}

bool Dstream::GlobalIncrementalExpand(GlobalGrid & g, list<string>&globalKeyVector) {
	//	cout << "global expand" << endl;
	double distance = 0;
	vector<string> seedlist;
	hashMapIt it;
	for (list<string>::iterator git = globalKeyVector.begin(); git != globalKeyVector.end(); ++git) {
		it = hasGlobalGrid(*git);
		if (it == globalGridHash.end())
			continue;
		if (it->second.getGridStatus() == SPARSE)
			continue;
		if (it->second.getClusterId() == g.getClusterId())
			continue;
		//if(it->second.getClusterId()>UNCLASSIFIED)continue;
		distance = g.distance(it->second);
		if (distance <= eps && distance > 0) {
			seedlist.push_back(it->second.getGridKey());
			it->second.setClusterId(g.getClusterId());
			it->second.setIsChanged(false);
		}
	}

	for (vector<string>::iterator it = seedlist.begin(); it != seedlist.end(); ++it) {
		if (globalGridHash.find(*it)->second.getGridStatus() == DENSE) {
			GlobalIncrementalExpand(globalGridHash.find(*it)->second, globalKeyVector);
		}
	}
	return true;
}

int Dstream::GlobalIncrementalMergeCluster(GlobalGrid & g1, GlobalGrid & g2, list<string>&globalKeyVector) {
	int clusterid1 = g1.getClusterId();
	int clusterid2 = g2.getClusterId();
	vector<string> cluster1;
	vector<string> cluster2;
	hashMapIt it;
	for (list<string>::iterator git = globalKeyVector.begin(); git != globalKeyVector.end(); ++git) {
		it = hasGlobalGrid(*git);
		if (it->second.getClusterId() == clusterid1)
			cluster1.push_back(it->first);
		else if (it->second.getClusterId() == clusterid2)
			cluster2.push_back(it->first);
	}

	if (cluster1.size() > cluster2.size()) {
		for (vector<string>::iterator it = cluster2.begin(); it != cluster2.end(); ++it) {
			globalGridHash.find(*it)->second.setClusterId(clusterid1);
		}
	} else {
		for (vector<string>::iterator it = cluster2.begin(); it != cluster2.end(); ++it) {
			globalGridHash.find(*it)->second.setClusterId(clusterid1);
		}
	}

	return 0;
}

void Dstream::incrementalAdjust(double timeArrive) {
	updateDensity(timeArrive);
	list<string> seedlist;
	for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
		if (it->second.isChanged() == true) {
			if (it->second.getGridStatus() == DENSE) {
				if (it->second.getClusterId() <= UNCLASSIFIED) {//noise or unclssified
					incrementalRangeQuery(it->second, seedlist, DENSE_WITH_NOCLASS);
					if (it->second.getClusterId() == UNCLASSIFIED) {
						if ((int) seedlist.size() < minPts) {
							it->second.setClusterId(NOISE);
						} else {
							//	cout << "increCreate" << endl;
							if (expandCluster(it->second, seedlist))
								++clusterID;
						}
					}
				} else if (it->second.getClusterId() > UNCLASSIFIED) {
					/*find neigh
					 * if(neigh has cluster and is not the same as this)then join small one to bigger one
					 * else if(neigh has not cluster and has not dense grid but has transitional grid , add transitional grid to this cluster)
					 * */
					incrementalRangeQuery(it->second, seedlist, DENSE_WITH_CLASS);
				}
			} else if (it->second.getGridStatus() == SPARSE && it->second.getClusterId() > UNCLASSIFIED) {
				//incrementalRangeQuery(it->second, seedlist,
				//	DELETE_SPARSE);
				it->second.setClusterId(UNCLASSIFIED);
				/*set<string>seedSet;
				 if(seedlist.size()!=0){
				 for(list<string>::iterator sit = seedlist.begin();sit!=seedlist.end();++sit){
				 seedSet.insert(*sit);
				 }
				 incrementalIsConnected(seedSet);
				 }*/
			} else if (it->second.getGridStatus() == TRANSITIONAL) {
				if (it->second.getClusterId() <= UNCLASSIFIED) {
					incrementalRangeQuery(it->second, seedlist, TRANSITIONAL_WITH_NO_CLASS);
					if (it->second.getClusterId() == UNCLASSIFIED) {
						if ((int) seedlist.size() < minPts) {
							it->second.setClusterId(NOISE);
						}
					}
				} else if (it->second.getClusterId() > UNCLASSIFIED) {
					incrementalRangeQuery(it->second, seedlist, TRANSITIONAL_WITH_CLASS);
				}
			}
		}
	}
}

//assign clusterid of all grids that was in the same cluster as g to oldClusterid
bool Dstream::incrementalCreate(GlobalGrid & g, int oldClusterid) {
	list<string> seedlist;
	double distance = 0;
	for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
		if (it->second.getClusterId() == oldClusterid) {
			distance = g.distance(it->second);
			if (distance <= eps && distance > 0) {
				seedlist.push_back(it->second.getGridKey());
			}
		}
	}
	for (list<string>::iterator it = seedlist.begin(); it != seedlist.end();) {
		globalGridHash.find(*it)->second.setClusterId(clusterID);
		if (globalGridHash.find(*it)->second == g) {
			it = seedlist.erase(it);
		} else {
			++it;
		}
	}

	for (list<string>::iterator it = seedlist.begin(); it != seedlist.end(); ++it) {
		incrementalCreate(globalGridHash.find(*it)->second, oldClusterid);
	}
	return false;
}

bool Dstream::incrementalFindNeighbor(GlobalGrid & g, set<string>&neighbor, set<string>&denseGrid) {
	for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
		if (it->second.distance(g) <= eps) {
			if (denseGrid.find(it->second.getGridKey()) != denseGrid.end())
				return true;
			else if (neighbor.find(it->second.getGridKey()) != neighbor.end())
				if (g.getClusterId() == it->second.getClusterId())
					neighbor.insert(it->second.getGridKey());
		}
	}
	return false;
}

bool Dstream::incrementalIsConnected(set<string>&neighbor) {
	//cout << "incremental is connected" << endl;
	if (neighbor.size() == 0)
		return false;
	set<string> container;
	bool flag = false;
	for (set<string>::iterator it = neighbor.begin(); it != neighbor.end(); ++it) {
		container.insert(*it);
		flag = false;
		for (set<string>::iterator cit = container.begin(); cit != container.end(); ++cit) {
			if (incrementalFindNeighbor(globalGridHash.find(*cit)->second, container, neighbor)) {
				flag = true;
				break;
			}
		}
		if (flag == false) {
			if (incrementalCreate(globalGridHash.find(*it)->second, globalGridHash.find(*it)->second.getClusterId())) {
				++clusterID;
				flag = true;
				break;
			}
			break;
		}
	}
	return flag;
}

void Dstream::addToCluster(hashMapIt it, Cluster & c) {
	int clusterId = c.getClusterID();
	int oldClusterID = it->second.getClusterId();
	string key = it->second.getGridKey();
	if (oldClusterID != UNCLASSIFIED) {
		cluster.at(oldClusterID).removeGridKey(key);
	}
	c.addGridKey(key);
	c.addGridCoordinate(it->second.getCoordinate());
	it->second.setClusterId(clusterId);
}

int Dstream::removeFromCluster(hashMapIt it) {
	int clusterID = it->second.getClusterId();

	if (clusterID != UNCLASSIFIED) {
		it->second.setClusterId(UNCLASSIFIED);
		cluster.at(clusterID).removeGridKey(it->second.getGridKey());
		cluster.at(clusterID).removeGridCoordinate(it->second.getCoordinate());
	}
	return clusterID;
}

void Dstream::clearCluster() {
	for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
		it->second.setClusterId(UNCLASSIFIED);
	}
	clusterID = 0;
}

void Dstream::epsilonRangeQuery(GlobalGrid & g, list<string> & seedList, int type) {
	double distance = 0;
	int pointnum = 0;
	for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
		if (it->second.getGridStatus() == SPARSE && type == EXPAND)
			continue;
		if (type == EXPAND && it->second.getClusterId() > UNCLASSIFIED)
			continue;
		if (type == FINDNEIGHBOR && pointnum >= minPts && it->second.getClusterId() > UNCLASSIFIED)
			continue;
		if (type == FINDDENSE && it->second.getGridStatus() != DENSE)
			continue;
		distance = g.distance(it->second);
		if (distance <= eps && distance > 0) {
			seedList.push_back(it->second.getGridKey());
			++pointnum;
		}
	}
}

bool Dstream::expandCluster(GlobalGrid & g, list<string>&seedList, int type) {
	//seedList.clear();
	if (seedList.size() == 0)
		epsilonRangeQuery(g, seedList, type);

	for (list<string>::iterator it = seedList.begin(); it != seedList.end(); ++it) {
		hasGlobalGrid(*it)->second.setClusterId(clusterID);
		hasGlobalGrid(*it)->second.setIsChanged(false);
	}

	if (seedList.size() == 0) {
		if ((int) g.getPointLabel().size() < minInnerPts) {
			g.setClusterId(NOISE);
			return false;
		}
		return true;
	}
	if (type == INCREMENTALEXPAND)
		type = INCREMENTALFINDNEIGHBOR;
	else if (type == EXPAND)
		type = FINDNEIGHBOR;
	for (list<string>::iterator it = seedList.begin(); it != seedList.end();) {
		if (hasGlobalGrid(*it)->second.getGridStatus() == DENSE) {
			list<string> listNeighbor;
			epsilonRangeQuery(hasGlobalGrid(*it)->second, listNeighbor, type);
			if ((int) listNeighbor.size() >= minPts) {
				//cout << "neigh size = " << listNeighbor.size() << endl;
				for (list<string>::iterator nit = listNeighbor.begin(); nit != listNeighbor.end(); ++nit) {
					if (hasGlobalGrid(*nit)->second.getClusterId() == UNCLASSIFIED || hasGlobalGrid(*nit)->second.getClusterId() == NOISE) {
						if (hasGlobalGrid(*nit)->second.getClusterId() == UNCLASSIFIED) {
							seedList.push_back((*nit));
						}
						hasGlobalGrid(*nit)->second.setClusterId(clusterID);
						hasGlobalGrid(*nit)->second.setIsChanged(false);
					} else if (type == INCREMENTALFINDNEIGHBOR && hasGlobalGrid(*nit)->second.getClusterId() != clusterID) {
						hasGlobalGrid(*nit)->second.setClusterId(clusterID);
						hasGlobalGrid(*nit)->second.setIsChanged(false);
						seedList.push_back((*nit));
					}
				}
			}
		}
		it = seedList.erase(it);
	}
	return true;
}

void Dstream::localEplisonRangeQuery(GlobalGrid & globalGrid, list<string>&globalKey, list<string>&seedlist, int type) {
	//double time = CommonUtil::getMicroSecond();
	hashMapIt hashIt;
	int pointnum = 0;
	double distance = 0;
	for (list<string>::iterator it = globalKey.begin(); it != globalKey.end(); ++it) {
		hashIt = globalGridHash.find(*it);
		if (hashIt != globalGridHash.end()) {
			if (hashIt->second.getGridStatus() == SPARSE)
				continue;
			if (type == FINDNEIGHBOR && hashIt->second.getClusterId() > UNCLASSIFIED && pointnum >= minPts)
				continue;
			if (globalGrid.getGridKey() == hashIt->second.getGridKey())
				continue;
			distance = globalGrid.distance(hashIt->second);
			if (distance <= eps /*&& distance > 0*/) {
				seedlist.push_back(*it);
				++pointnum;
			}
		}
	}
	//	time = CommonUtil::getMicroSecond() - time;
	//	if(time > 1000){
	//		cout<<"time is more than 1ms "<<time/1000<<endl;
	//	}
	//cout<<"time is "<<time / 1000<<endl;
}

bool Dstream::localExpandCluster(GlobalGrid & globalGrid, list<string>&seedList, LocalGrid & localGrid, int type, cudaGrid * cudaArray) {
	list<string> globalKey = localGrid.getGlobalKey();
	if (seedList.size() == 0) {
		localEplisonRangeQuery(globalGrid, globalKey, seedList, type);
	}
	hashMapIt hashIt;

	//	for (list<string>::iterator it = seedList.begin(); it != seedList.end();) {
	//		hashIt = hasGlobalGrid(*it);
	//		if (hashIt != globalGridHash.end()) {
	//			hashIt->second.setClusterId(clusterID);
	//			if (hashIt->second.getGridKey() == globalGrid.getGridKey()) {
	//				it = seedList.erase(it);
	//			} else {
	//				++it;
	//			}
	//		}
	//	}
	for (list<string>::iterator it = seedList.begin(); it != seedList.end(); ++it) {
		hashIt = hasGlobalGrid(*it);
		if (hashIt != globalGridHash.end()) {
			hashIt->second.setClusterId(clusterID);
			/*if(hashIt->second.getGridKey() == globalGrid.getGridKey()){
			 it = seedList.erase(it);
			 }else{
			 ++it;
			 }*/
		}
	}

	if (seedList.size() == 0) {
		if ((int) globalGrid.getPointLabel().size() < minInnerPts) {
			globalGrid.setClusterId(NOISE);
			return false;
		}
		return true;
	}
	if (type == INCREMENTALEXPAND)
		type = INCREMENTALFINDNEIGHBOR;
	else if (type == EXPAND)
		type = FINDNEIGHBOR;
	for (list<string>::iterator it = seedList.begin(); it != seedList.end();) {
		hashIt = hasGlobalGrid(*it);
		if (hashIt != globalGridHash.end()) {
			if (hashIt->second.getGridStatus() == DENSE) {
				list<string> neighbor;
				localEplisonRangeQuery(hashIt->second, globalKey, neighbor, type);
				if ((int) neighbor.size() >= minPts) {
					for (list<string>::iterator nit = neighbor.begin(); nit != neighbor.end(); ++nit) {
						if (hashIt->second.getClusterId() <= UNCLASSIFIED) {
							if (hashIt->second.getClusterId() == UNCLASSIFIED) {
								seedList.push_back(*nit);
							}
							hashIt->second.setClusterId(clusterID);
							hashIt->second.setIsChanged(false);
						} else if (type == INCREMENTALFINDNEIGHBOR) {
							if (hashIt->second.getClusterId() != clusterID) {
								hashIt->second.setClusterId(clusterID);
								hashIt->second.setIsChanged(false);
								seedList.push_back(*nit);
							}
						}
					}
				}
			}
		}
		it = seedList.erase(it);
	}
	return true;
}

void Dstream::localBuildCluster() {
	clearCluster();
	list<string> globalKey;
	hashMapIt globalIt;
	list<string> seedlist;
	//	cout << "local grid size = " << localGridHash.size() << endl;
	for (localHashMapIt localIt = localGridHash.begin(); localIt != localGridHash.end(); ++localIt) {//foreach local grid
		globalKey = localIt->second.getGlobalKey();//get global Grid set of the local grid
		for (list<string>::iterator git = globalKey.begin(); git != globalKey.end(); ++git) {//foreach glocal grid in the local grid
			globalIt = globalGridHash.find(*git);//get grid hash iterator
			if (globalIt->second.getGridStatus() == DENSE) {
				if (globalIt->second.getClusterId() == UNCLASSIFIED) {//expand filter:is dense and unclassified
					if (globalIt->second.getPointNum() >= minInnerPts) {//this condition is optional
						if (localExpandCluster(globalIt->second, seedlist, localIt->second)) {//expand in local grid
							++clusterID;
						}
					}
				}
			} else {
				//		cout << "is not dense" << endl;
			}
		}
	}

	borderCluster();
}

void Dstream::assigncudaGrid(list<string>&globalKey) {
	if (globalKey.size() > globalGridSize) {
		cout << "global size over flow" << endl;
		return;
	}
	memset(h_cudaArray, 0, sizeof(cudaGrid) * globalKey.size());
	int j = 0;
	hashMapIt globalIt;
	for (list<string>::iterator git = globalKey.begin(); git != globalKey.end(); ++git) {
		globalIt = globalGridHash.find(*git);
		if (globalIt != globalGridHash.end()) {
			assigncudaGrid(h_cudaArray + j, globalIt->second);
		}
		++j;
	}
	cudaMemoryCopy(h_cudaArray, d_cudaArray, sizeof(cudaGrid) * globalKey.size());
}

void Dstream::cudaBuildCluster() {
	clearCluster();
	list<string> globalKey;
	hashMapIt globalIt;
	list<string> seedlist;
	int j = 0;
	vector<double> cf;
	double time = CommonUtil::getMicroSecond();
	cudaConstruct();
	time = CommonUtil::getMicroSecond() - time;
	cout << "construct time in cuda build = " << time / 1000 << endl;
	for (localHashMapIt localIt = localGridHash.begin(); localIt != localGridHash.end(); ++localIt) {//foreach local grid
		globalKey = localIt->second.getGlobalKey();//get global Grid set of the local grid
		assigncudaGrid(globalKey);
		for (list<string>::iterator git = globalKey.begin(); git != globalKey.end(); ++git) {//foreach glocal grid in the local grid
			globalIt = globalGridHash.find(*git);//get grid hash iterator
			if (globalIt->second.getGridStatus() == DENSE) {
				if (globalIt->second.getClusterId() == UNCLASSIFIED) {//expand filter:is dense and unclassified
					if (globalIt->second.getPointNum() >= minInnerPts) {//this condition is optional
						//time = CommonUtil::getMicroSecond();
						if (cudaExpandCluster(globalIt->second, seedlist, localIt->second)) {//expand in local grid
							++clusterID;
						}
						//time = CommonUtil::getMicroSecond() - time;
						//cout<<"time is "<<time/1000<<endl;
					}
				}
			}
		}
	}
	//time = CommonUtil::getMicroSecond() - time;
	//cout << "first cuda = " << time / 1000 << endl;
	//time = CommonUtil::getMicroSecond();
	borderCluster();
	//time = CommonUtil::getMicroSecond() - time;
	//cout << "border cluster = " << time / 1000 << endl;
}

void Dstream::cudaEplisonRangeQuery(GlobalGrid & globalGrid, list<string>&seedlist, int size, int type) {
	memset(h_curGrid, 0, sizeof(cudaGrid));
	assigncudaGrid(h_curGrid, globalGrid);
	cudaMemoryCopy(h_curGrid, d_curGrid, sizeof(cudaGrid));
	seedlist.clear();
	//double time = CommonUtil::getMicroSecond();
	cudaDistance(h_cudaArray, d_cudaArray, h_curGrid, d_curGrid, size, d_result, h_result, type, h_pointCnt, d_pointCnt);
	//time = CommonUtil::getMicroSecond() - time;
	//	if(time > 1000){
	//		cout<<"time is more than 1ms"<<time/1000<<endl;
	//	}
	//cout<<"time is "<<time/1000<<endl;
	if (*h_pointCnt >= minPts) {

		for (int i = 0; i != size; ++i) {
			if (h_result[i] == 1) {
				seedlist.push_back(string(h_cudaArray[i].globalGridKey));
			}
		}
	}

}

void Dstream::assigncudaGrid(cudaGrid * grid, GlobalGrid & global) {
	vector<double> cf;
	sprintf(grid->globalGridKey, "%s", global.getGridKey().c_str());
	cf = global.getCF();
	grid->status = global.getGridStatus();
	grid->clusterID = global.getClusterId();
	for (int k = 0; k < DIMENSION; ++k) {
		grid->cf[k] = cf.at(k);
	}
}

bool Dstream::cudaExpandCluster(GlobalGrid & globalGrid, list<string>&seedList, LocalGrid & localGrid, int type) {
	list<string> globalKey = localGrid.getGlobalKey();
	if (seedList.size() == 0) {
		cudaEplisonRangeQuery(globalGrid, seedList, localGrid.getGlobalKey().size(), type);
	}

	hashMapIt hashIt;

	for (list<string>::iterator it = seedList.begin(); it != seedList.end();++it) {
		hashIt = hasGlobalGrid(*it);
		if (hashIt != globalGridHash.end()) {
			hashIt->second.setClusterId(clusterID);
		}
	}

	if (seedList.size() == 0) {
		if ((int) globalGrid.getPointLabel().size() < minInnerPts) {
			globalGrid.setClusterId(NOISE);
			return false;
		}
		return true;
	}

	//double temp = CommonUtil::getMicroSecond();
	assigncudaGrid(globalKey);
	//temp = CommonUtil::getMicroSecond() - temp;
	//cout<<"copy time ="<<temp/1000<<endl;
	if (type == INCREMENTALEXPAND)
		type = INCREMENTALFINDNEIGHBOR;
	else if (type == EXPAND)
		type = FINDNEIGHBOR;
	for (list<string>::iterator it = seedList.begin(); it != seedList.end();) {
		hashIt = hasGlobalGrid(*it);
		if (hashIt != globalGridHash.end()) {
			if (hashIt->second.getGridStatus() == DENSE) {
				list<string> neighbor;
				//localEplisonRangeQuery(hashIt->second, globalKey, neighbor, type);
				cudaEplisonRangeQuery(hashIt->second, neighbor, localGrid.getGlobalKey().size(), type);
				if ((int) neighbor.size() >= minPts) {
					for (list<string>::iterator nit = neighbor.begin(); nit != neighbor.end(); ++nit) {
						if (hashIt->second.getClusterId() <= UNCLASSIFIED) {
							if (hashIt->second.getClusterId() == UNCLASSIFIED) {
								seedList.push_back(*nit);
							}
							hashIt->second.setClusterId(clusterID);
							hashIt->second.setIsChanged(false);
						} else if (type == INCREMENTALFINDNEIGHBOR) {
							if (hashIt->second.getClusterId() != clusterID) {
								hashIt->second.setClusterId(clusterID);
								hashIt->second.setIsChanged(false);
								seedList.push_back(*nit);
							}
						}
					}
				}
			}
		}
		it = seedList.erase(it);
	}
	return true;
}

void Dstream::cudaGlobalIncrementalAdjust(double timeArrive) {
	updateDensity(timeArrive);
	list<string> globalKeyVector;
	hashMapIt it;
	list<string> seedlist;
	//cout << "before construct in cuda global" << endl;
	cudaConstruct();
	//cout << "after construct in cuda global" << endl;
	for (localHashMapIt lit = localGridHash.begin(); lit != localGridHash.end(); ++lit) {
		globalKeyVector = lit->second.getGlobalKey();
		//cout << "before assign in cuda global" << endl;
		assigncudaGrid(globalKeyVector);
		//cout << "after assign in cuda global" << endl;
		for (list<string>::iterator git = globalKeyVector.begin(); git != globalKeyVector.end(); ++git) {
			it = hasGlobalGrid(*git);
			if (it == globalGridHash.end())
				continue;
			if (it->second.isChanged() == true) {
				if (it->second.getGridStatus() == DENSE) {
					if (it->second.getClusterId() <= UNCLASSIFIED) {//noise or unclssified
						/*find neigh
						 * if(neigh is dense and it has cluster )join to neigh
						 * else if(neigh has not cluster and neigh size > minpts)then create
						 * else if(neigh has not cluster and neight size < minpts)then as noise;
						 * */
						//cout << "before dense cuda global incremental" << endl;
						cudaGlobalIncrementalRangeQuery(it->second, seedlist, globalKeyVector.size(), DENSE_WITH_NOCLASS);
						//cout << "after dense cuda global incremental" << endl;
						if (it->second.getClusterId() == UNCLASSIFIED) {
							if ((int) seedlist.size() < minPts) {
								it->second.setClusterId(NOISE);
							} else {
								//cout << "before cuda expand" << endl;
								if (cudaExpandCluster(it->second, seedlist, lit->second, INCREMENTALEXPAND))
									++clusterID;
								//cout << "after cuda expand" << endl;
							}
						}
					} else if (it->second.getClusterId() > UNCLASSIFIED) {
						/*find neigh
						 * if(neigh has cluster and is not the same as this)then join small one to bigger one
						 * else if(neigh has not cluster and has not dense grid but has transitional grid , add transitional grid to this cluster)
						 * */
						//cout << "before unclassified dense cuda global incremental" << endl;
						cudaGlobalIncrementalRangeQuery(it->second, seedlist, globalKeyVector.size(), DENSE_WITH_CLASS);
						//cout << "after unclassified dense cuda global incremental" << endl;
					}
				} else if (it->second.getGridStatus() == SPARSE && it->second.getClusterId() > UNCLASSIFIED) {
					//incrementalRangeQuery(it->second, seedlist,
					//	DELETE_SPARSE);
					it->second.setClusterId(UNCLASSIFIED);
					/*set<string>seedSet;
					 if(seedlist.size()!=0){
					 for(list<string>::iterator sit = seedlist.begin();sit!=seedlist.end();++sit){
					 seedSet.insert(*sit);
					 }
					 incrementalIsConnected(seedSet);
					 }*/
				} else if (it->second.getGridStatus() == TRANSITIONAL) {
					if (it->second.getClusterId() <= UNCLASSIFIED) {
						//cout << "before unclassified dense cuda global incremental" << endl;
						cudaGlobalIncrementalRangeQuery(it->second, seedlist, globalKeyVector.size(), TRANSITIONAL_WITH_NO_CLASS);
						//cout << "after unclassified dense cuda global incremental" << endl;
						if (it->second.getClusterId() == UNCLASSIFIED) {
							if ((int) seedlist.size() < minPts) {
								it->second.setClusterId(NOISE);
							}
						}
					} else if (it->second.getClusterId() > UNCLASSIFIED) {
						//cout << "before unclassified transitional cuda global incremental" << endl;
						cudaGlobalIncrementalRangeQuery(it->second, seedlist, globalKeyVector.size(), TRANSITIONAL_WITH_CLASS);
						//cout << "after unclassified transitional cuda global incremental" << endl;
					}
				}
			}
		}
	}

	borderCluster(true);
}

bool Dstream::cudaGlobalIncrementalExpand(GlobalGrid & g, int size) {
	list<string> seedlist;
	cudaEplisonRangeQuery(g, seedlist, size, INCREMENTALFINDNEIGHBOR);
	hashMapIt it;
	for (list<string>::iterator git = seedlist.begin(); git != seedlist.end(); ++git) {
		it = hasGlobalGrid(*git);
		if (it == globalGridHash.end())
			continue;
		if (it->second.getClusterId() == g.getClusterId())
			continue;
		seedlist.push_back(it->second.getGridKey());
		it->second.setClusterId(g.getClusterId());
		it->second.setIsChanged(false);
	}

	for (list<string>::iterator nit = seedlist.begin(); nit != seedlist.end(); ++nit) {
		it = globalGridHash.find(*nit);
		if (it != globalGridHash.end()) {
			if (it->second.getGridStatus() == DENSE) {
				//cudaGlobalIncrementalExpand(it->second, size);
			}
		}
	}
	return true;
}

bool Dstream::cudaGlobalIncrementalRangeQuery(GlobalGrid & g, list<string>&seedlist, int size, int type) {
	//cout << "before cuda eplison" << endl;
	cudaEplisonRangeQuery(g, seedlist, size, type);
	//cout << "after cuda eplison" << endl;
	bool flag = false;
	hashMapIt it;
	for (list<string>::iterator git = seedlist.begin(); git != seedlist.end(); ++git) {
		it = hasGlobalGrid(*git);
		if (it == globalGridHash.end())
			continue;

		switch (type) {
		case DENSE_WITH_NOCLASS:
			if (it->second.getClusterId() > UNCLASSIFIED && it->second.getGridStatus() == DENSE) {
				g.setClusterId(it->second.getClusterId());
				//cout << "before cuda global expand" << endl;
				cudaGlobalIncrementalExpand(g, size);
				//cout << "after cuda global expand" << endl;
				flag = true;
			}
			break;
		case DENSE_WITH_CLASS:
			if (it->second.getGridStatus() == DENSE && it->second.getClusterId() > UNCLASSIFIED && it->second.getClusterId() != g.getClusterId()) {
				//cout << "before cuda global merge" << endl;
				incrementalMergeCluster(g, it->second);
				//cout << "after cuda global merge" << endl;
				flag = true;
			} else if (it->second.getGridStatus() == TRANSITIONAL && it->second.getClusterId() == UNCLASSIFIED) {
				it->second.setClusterId(g.getClusterId());
				it->second.setIsChanged(false);
				flag = true;
			}
			break;
		case TRANSITIONAL_WITH_CLASS:
			if (it->second.getClusterId() > UNCLASSIFIED && it->second.getGridStatus() == DENSE && it->second.getClusterId() != g.getClusterId()) {
				g.setClusterId(it->second.getClusterId());
				flag = true;
			}
			break;
		case TRANSITIONAL_WITH_NO_CLASS:
			if (it->second.getClusterId() > UNCLASSIFIED && it->second.getGridStatus() == DENSE) {
				g.setClusterId(it->second.getClusterId());
				flag = true;
			}
			break;
		}
		if (flag == true)
			break;

	}
	return flag;
}

void Dstream::cudaConstruct() {
	if (globalGridHash.size() > globalGridSize) {
		if (h_cudaArray != 0) {
			memDealloc((void*) h_cudaArray);
			h_cudaArray = 0;
		}
		if (d_cudaArray != 0) {
			cudaMemFree((void*) d_cudaArray);
			d_cudaArray = 0;
		}
		if (d_result != 0) {
			cudaMemFree(d_result);
			d_result = 0;
		}
		if (h_result != 0) {
			memDealloc(h_result);
			h_result = 0;
		}
		memAlloc((void**) &h_cudaArray, sizeof(cudaGrid) * globalGridHash.size());
		memset(h_cudaArray, 0, sizeof(cudaGrid) * globalGridHash.size());
		memAlloc((void**) &h_result, sizeof(int) * globalGridHash.size());
		memset(h_result, 0, sizeof(int) * globalGridHash.size());

		cudaMemAlloc((void**) &d_cudaArray, sizeof(cudaGrid) * globalGridHash.size(), true);
		cudaMemAlloc((void**) &d_result, sizeof(int) * globalGridHash.size(), true);//it is necessary to set all memory of d_result as zero
		globalGridSize = globalGridHash.size();

	}
	if (h_pointCnt == 0) {
		memAlloc((void**) &h_pointCnt, sizeof(int));
		memset(h_pointCnt, 0, sizeof(int));
	}
	if (h_curGrid == 0) {
		memAlloc((void**) &h_curGrid, sizeof(cudaGrid));
		memset(h_curGrid, 0, sizeof(cudaGrid));
	}
	if (d_curGrid == 0) {
		cudaMemAlloc((void**) &d_curGrid, sizeof(cudaGrid));
	}
	if (d_pointCnt == 0) {
		cudaMemAlloc((void**) &d_pointCnt, sizeof(int));
	}
}
/*
 void Dstream::cudaLocalBuildCluster() {
 clearCluster();
 hashMapIt globalIt;
 list<string> globalKey;
 vector<double> cf;
 cudaGrid * cudaArray = 0;
 int * intermediate = 0;
 int * gridnum = 0;
 int i = 0, j = 0;
 int total = 0;
 int *stride1d =0;
 int * stride2d = 0;
 gridnum = cudaAlloc(sizeof(int) * localGridHash.size());
 memset(gridnum, 0, sizeof(int) * localGridHash.size());
 stride1d = cudaAlloc(sizeof(int) * localGridHash.size());
 memset(stride1d,0,sizeof(int)*localGridHash.size());
 stride2d = cudaAlloc(sizeof(int) * localGridHash.size());
 memset(stride2d,0,sizeof(int)*localGridHash.size());
 int max = 0;
 for (localHashMapIt localIt = localGridHash.begin(); localIt != localGridHash.end(); ++localIt) {//foreach local grid
 gridnum[i] = localIt->second.getGlobalKey().size();
 if(gridnum[i] > max){
 max = gridnum[i];
 }
 if(i>0){
 stride1d[i] += stride1d[i-1] + gridnum[i-1];//stride1d store the offset of each localgrid in the cudaArray allocated below
 stride2d[i] = total;//stride2d store the offset of each localgrid in the intermediate allocate below.
 }
 total += gridnum[i] * gridnum[i];
 ++i;
 }
 i = 0;


 cudaArray = (cudaGrid *)cudaAlloc(sizeof(cudaGrid)* globalGridHash.size());
 memset(cudaArray, 0, sizeof(cudaGrid) * globalGridHash.size());
 intermediate = cudaAlloc(sizeof(int) * total);
 memset(intermediate, 0, sizeof(int)* total);

 for (localHashMapIt localIt = localGridHash.begin(); localIt != localGridHash.end(); ++localIt) {//foreach local grid
 globalKey = localIt->second.getGlobalKey();//get global Grid set of the local grid
 j = 0;
 for (list<string>::iterator git = globalKey.begin(); git != globalKey.end(); ++git) {
 globalIt = globalGridHash.find(*git);
 if (globalIt != globalGridHash.end()) {
 sprintf(cudaArray[stride1d[i]  + j].globalGridKey, "%s", globalIt->second.getGridKey().c_str());
 cf = globalIt->second.getCF();
 for (int k = 0; k < DIMENSION; ++k) {
 cudaArray[stride1d[i] + j].cf[k] = cf.at(k);
 }
 }
 ++j;
 }
 ++i;
 }
 cout<<"before map"<<endl;
 //for(i = 0; i< max;++i){
 cudaMap(cudaArray, localGridHash.size(),gridnum,stride1d,stride2d, intermediate);
 //}
 cudaDealloc(intermediate);
 cudaDealloc((int*) cudaArray);
 cudaDealloc(gridnum);
 cudaDealloc(stride1d);
 cudaDealloc(stride2d);
 cudaArray = 0;
 intermediate = 0;
 cout << "end" << endl;
 //	borderCluster();
 }*/

void Dstream::searchInBorder(GlobalGrid & globalGrid, list<string>&keyList, list<string>&neighbor, int type) {
	hashMapIt globalIt;
	double distance = 0;
	bool flag = false;
	int pointnum = 0;
	for (list<string>::iterator it = keyList.begin(); it != keyList.end(); ++it) {
		globalIt = hasGlobalGrid(*it);
		if (globalIt == globalGridHash.end())
			continue;
		if (globalIt->second.getGridStatus() == SPARSE)//sparse grid is omited
			continue;
		if (hasGlobalGrid(*it)->second.getLocalKey() == globalGrid.getLocalKey())//if two globalGrid is in the same local grid then continue
			continue;
		switch (type) {
		case DENSE_WITH_NOCLASS:
			distance = globalGrid.distance(globalIt->second);
			if (distance <= eps && distance > 0) {//in the scope of globalit
				if (globalIt->second.getClusterId() > UNCLASSIFIED && globalIt->second.getGridStatus() != SPARSE) {//can be transitional
					globalGrid.setClusterId(globalIt->second.getClusterId());//add globalgrid to the cluster of globalit
					flag = true;
				} else if (globalIt->second.getGridStatus() != SPARSE) {
					neighbor.push_back(*it);//save neighbor for create cluster later
				}
			}
			break;
		case DENSE_WITH_CLASS:
			distance = globalGrid.distance(globalIt->second);
			if (distance <= eps && distance > 0) {
				if (globalIt->second.getClusterId() > UNCLASSIFIED && globalIt->second.getClusterId() != globalGrid.getClusterId()
						&& globalIt->second.getGridStatus() == DENSE) {
					GlobalMerge(globalGrid, globalIt->second);//two cluster merge
					flag = true;
				} else if (globalIt->second.getClusterId() <= UNCLASSIFIED && globalIt->second.getGridStatus() != SPARSE) {
					//	cout << "dense set id" << endl;
					globalIt->second.setClusterId(globalGrid.getClusterId());//neighbor is not classified and is not sparse,then add neighbor to the cluster of globalgrid
					globalIt->second.setIsChanged(false);
				}
			}
			break;
		case TRANSITIONAL_WITH_CLASS:
			//for now i use this kind of method , but i am a little confused about it,maybe i can change it
			if (globalIt->second.getClusterId() > UNCLASSIFIED && globalIt->second.getGridStatus() == DENSE && globalIt->second.getClusterId()
					!= globalGrid.getClusterId()) {
				distance = globalGrid.distance(globalIt->second);
				if (distance <= eps && distance > 0) {
					globalGrid.setClusterId(globalIt->second.getClusterId());
					flag = true;
				}
			}
			break;
		case TRANSITIONAL_WITH_NO_CLASS:
			if (globalIt->second.getClusterId() > UNCLASSIFIED && globalIt->second.getGridStatus() == DENSE) {
				distance = globalGrid.distance(globalIt->second);
				if (distance <= eps && distance > 0) {
					globalGrid.setClusterId(globalIt->second.getClusterId());
					flag = true;
				}
			} else {
				distance = globalGrid.distance(globalIt->second);
				if (distance <= eps && distance > 0) {
					neighbor.push_back(*it);
				}
			}
			break;
		case EXPAND:
			distance = globalGrid.distance(globalIt->second);//expand then save all the grid within the distance
			if (distance <= eps && distance > 0) {
				neighbor.push_back(*it);
			}
			break;
		case FINDNEIGHBOR:
			if (globalIt->second.getClusterId() > UNCLASSIFIED && pointnum >= minPts)//if it is classified then omit it
				break;
			else {
				distance = globalGrid.distance(globalIt->second);//not classified then add it
				if (distance > 0 && distance <= eps) {
					neighbor.push_back(*it);
					++pointnum;
				}
			}
			break;
		}
		if (flag == true)
			break;
	}
}

bool Dstream::borderExpand(GlobalGrid & globalGrid, list<string>&seedlist, list<string>&keyList) {
	if (seedlist.size() == 0)
		searchInBorder(globalGrid, keyList, seedlist, FINDNEIGHBOR);

	if ((int) seedlist.size() < minPts) {
		if ((int) globalGrid.getPointLabel().size() < minInnerPts) {
			globalGrid.setClusterId(NOISE);
			return false;
		}
		return true;
	}

	hashMapIt globalIt;
	for (list<string>::iterator it = seedlist.begin(); it != seedlist.end(); ++it) {
		globalIt = hasGlobalGrid(*it);
		globalIt->second.setClusterId(clusterID);
	}

	for (list<string>::iterator it = seedlist.begin(); it != seedlist.end();) {
		globalIt = hasGlobalGrid(*it);
		if (globalIt->second.getGridStatus() == DENSE) {
			list<string> neighbor;
			searchInBorder(globalIt->second, keyList, neighbor, FINDNEIGHBOR);
			if ((int) seedlist.size() >= minPts) {
				for (list<string>::iterator nit = neighbor.begin(); nit != neighbor.end(); ++nit) {
					globalIt = hasGlobalGrid(*nit);
					if (globalIt->second.getClusterId() <= UNCLASSIFIED) {
						if (globalIt->second.getClusterId() == UNCLASSIFIED) {
							seedlist.push_back(*nit);
						}
						globalIt->second.setClusterId(clusterID);
					}
				}
			}
		}
		it = seedlist.erase(it);
	}
	return true;
}

void Dstream::borderCluster(bool isUpdating) {
	list<string> globalGrid;
	hashMapIt globalIt;
	list<string> seedlist;
	//cout << "bordersize" << borderGrid.size() << endl;
	for (borderHashIt borderIt = borderGrid.begin(); borderIt != borderGrid.end(); ++borderIt) {//for each border grid
		globalGrid = borderIt->second;//get global grid key of the border grid
		if (globalGrid.size() == 0)
			continue;
		for (list<string>::iterator git = globalGrid.begin(); git != globalGrid.end(); ++git) {//for each global grid in the border grid
			globalIt = hasGlobalGrid(*git);
			if (globalIt == globalGridHash.end())
				continue;
			if (isUpdating == true)//if is updating is true then we just care about those grids which is changed
				if (globalIt->second.isChanged() == false)
					continue;
			if (globalIt->second.getGridStatus() == DENSE && globalIt->second.getClusterId() == UNCLASSIFIED) {//dense and no class
				//		cout << "dense unclassifed" << endl;
				searchInBorder(globalIt->second, globalGrid, seedlist, DENSE_WITH_NOCLASS);//two kind of sitiuation.
				if (globalIt->second.getClusterId() == UNCLASSIFIED) {
					if ((int) seedlist.size() >= minPts) {
						//		cout << "border expand" << endl;
						if (borderExpand(globalIt->second, seedlist, globalGrid))//create a new
							++clusterID;
					} else {
						globalIt->second.setClusterId(NOISE);//is a noise
					}
				}//add to other's cluster
			} else if (globalIt->second.getGridStatus() == DENSE && globalIt->second.getClusterId() > UNCLASSIFIED) {
				//	cout << "dense classified" << endl;//is dense and classified
				searchInBorder(globalIt->second, globalGrid, seedlist, DENSE_WITH_CLASS);//absorb neighbor or merge with other cluster
			} else if (globalIt->second.getGridStatus() == TRANSITIONAL && globalIt->second.getClusterId() > UNCLASSIFIED) {
				//	cout << "transitional  classified" << endl;
				searchInBorder(globalIt->second, globalGrid, seedlist, TRANSITIONAL_WITH_CLASS);//find dense neighbor and join it
			} else if (globalIt->second.getGridStatus() == TRANSITIONAL && globalIt->second.getClusterId() <= UNCLASSIFIED) {
				//	cout << "transitional un classified" << endl;
				searchInBorder(globalIt->second, globalGrid, seedlist, TRANSITIONAL_WITH_NO_CLASS);// find dense neighbor and join it
				if (globalIt->second.getClusterId() == UNCLASSIFIED && (int) seedlist.size() < minPts)
					globalIt->second.setClusterId(NOISE);
			}
		}
	}
}

void Dstream::borderCluster(LocalGrid & localGrid, int dimension, set<pair<string, string> >&localGridPair, hashSet & globalGridKeySet) {
	string localKey = localGrid.getKey();
	string localNeighborKey = localFindNeighbor(localGrid, dimension);
	if (localKey == "")
		return;
	if (localGridPair.find(make_pair(localKey, localNeighborKey)) != localGridPair.end() || localGridPair .find(make_pair(localNeighborKey, localKey))
			!= localGridPair.end())
		return;
	list<string> globalGridKey = localGrid.getGlobalKey();
	list<string> neighborGlobalGridKey;
	localHashMapIt localGridIt = localGridHash.find(localKey);
	borderHashMapIt borderIt;
	borderHashMapIt neighborBorderIt;
	if (localGridIt != localGridHash.end()) {
		neighborGlobalGridKey = localGridIt->second.getGlobalKey();
		for (list<string>::iterator it = globalGridKey.begin(); it != globalGridKey.end(); ++it) {
			if (globalGridKeySet.find(*it) != globalGridKeySet.end())
				continue;
			for (list<string>::iterator nit = neighborGlobalGridKey.begin(); nit != neighborGlobalGridKey.end(); ++nit) {
				if (globalGridKeySet.find(*it) != globalGridKeySet.end())
					continue;
				borderIt = borderGridHash.find(*it);
				neighborBorderIt = borderGridHash.find(*nit);
				if (borderIt != borderGridHash.end() && borderIt == neighborBorderIt) {
					GlobalGrid & g1 = globalGridHash.find(*it)->second;
					GlobalGrid & g2 = globalGridHash.find(*nit)->second;
					if (g1.getGridStatus() != DENSE && g2.getGridStatus() != DENSE)
						continue;
					if (g1.getClusterId() <= UNCLASSIFIED && g2.getClusterId() <= UNCLASSIFIED)
						continue;
					double distance = g1.distance(g2);
					if (distance <= eps && distance > 0) {
						if (g1.getGridStatus() == DENSE) {
							if (g1.getClusterId() > UNCLASSIFIED) {
								if (g2.getClusterId() <= UNCLASSIFIED) {
									g2.setClusterId(g1.getClusterId());
								} else {
									GlobalMerge(g1, g2);//g1 is dense and is classified then merge g2 to g1
								}
							} else if (g1.getClusterId() <= UNCLASSIFIED) {//g1 is not dense
								if (g2.getGridStatus() == DENSE) {
									if (g2.getClusterId() > UNCLASSIFIED) {
										//if g2 is dense and classified
										g1.setClusterId(g2.getClusterId());
									}
								}
							}
						} else if (g2.getGridStatus() == DENSE) {
							if (g2.getClusterId() > UNCLASSIFIED) {
								if (g1.getClusterId() <= UNCLASSIFIED) {
									g1.setClusterId(g2.getClusterId());
								} else {
									GlobalMerge(g2, g1);
								}
							}
						}
					}
				}
			}
		}
	}
}

void Dstream::globalCluster(LocalGrid & localGrid, int dimension, set<pair<string, string> >& localGridPair) {
	set<int> cidset;
	hashSet hs;
	hashMapIt curGlobalIt;
	hashMapIt globalIt;
	localHashMapIt localGridIt;
	hashSet localneighborKey;
	string localNeighbor;
	string localKey = localGrid.getKey();
	hs = localGrid.getDenseBorderGrid(dimension);
	if (hs.size() > 0) {
		localNeighbor = localFindNeighbor(localGrid, dimension);
		localGridIt = localGridHash.find(localNeighbor);
		if (localGridIt == localGridHash.end())
			return;
		localGridPair.find(make_pair(localKey, localNeighbor));
		if (localGridPair.find(make_pair(localKey, localNeighbor)) != localGridPair.end() || localGridPair.find(make_pair(localNeighbor, localKey))
				!= localGridPair.end())
			return;
		localGridPair.insert(make_pair(localKey, localNeighbor));
		localneighborKey = localGridIt->second.getDenseBorderGrid(dimension);
		if (localneighborKey.size() == 0)
			return;
		for (hashSetIt hsit = hs.begin(); hsit != hs.end(); ++hsit) {
			curGlobalIt = globalGridHash.find(*hsit);
			if (curGlobalIt->second.getClusterId() > UNCLASSIFIED && cidset.find(curGlobalIt->second.getClusterId()) != cidset.end())
				continue;
			for (hashSetIt nit = localneighborKey.begin(); nit != localneighborKey.end(); ++nit) {
				globalIt = globalGridHash.find(*nit);
				if (globalIt != globalGridHash.end()) {
					if (globalIt->second.getGridStatus() == DENSE && curGlobalIt->second.distance(globalIt->second) <= eps) {
						ConnectedInfo ci;
						ci.dimension = dimension + 1;
						ci.key = *nit;
						curGlobalIt->second.setCi(ci);
						ci.key = *hsit;
						globalIt->second.setCi(ci);

						if (globalIt->second.getClusterId() > UNCLASSIFIED && globalIt->second.getClusterId()
								!= curGlobalIt->second.getClusterId()) {
							GlobalMerge(curGlobalIt->second, globalIt->second);
							cidset.insert(curGlobalIt->second.getClusterId());
						}
						break;
					}
				}
			}
		}
	}
}

string Dstream::localFindNeighbor(LocalGrid & localGrid, int dimension) {
	string key;
	bool flag = true;
	vector<int> coordinate = localGrid.getCoordinate();
	if (dimension > 0) {
		if (coordinate.at(dimension - 1) != localWarp - 1) {
			++coordinate[dimension - 1];
		} else {
			flag = false;
		}
	} else if (dimension < 0) {
		if (coordinate.at(dimension * (-1) - 1) != 0) {
			--coordinate[dimension * (-1) - 1];
		} else {
			flag = false;
		}
	}
	if (flag == true) {
		key = CommonUtil::getString(coordinate);
		return key;
	}

	return string("");
}

void Dstream::GlobalMerge(GlobalGrid & g1, GlobalGrid & g2) {
	//cout << "global merge" << endl;
	string localGridKey = g2.getLocalKey();
	int newClusterID = g1.getClusterId();
	int oldClusterID = g2.getClusterId();
	g2.setClusterId(newClusterID);
	list<string> globalKey = localGridHash.find(localGridKey)->second.getGlobalKey();
	hashMapIt hashIt;
	for (list<string>::iterator it = globalKey.begin(); it != globalKey.end(); ++it) {
		hashIt = globalGridHash.find(*it);
		if (hashIt == globalGridHash.end())
			continue;
		if (hashIt->second.getClusterId() == oldClusterID) {
			hashIt->second.setClusterId(newClusterID);
			hashIt->second.setIsChanged(false);
		}
	}
}

void Dstream::buildCluster() {
	clearCluster();
	list<string> seedList;
	hashMapIt it = globalGridHash.begin();
	while (it != globalGridHash.end()) {
		if (it->second.getGridStatus() == DENSE && it->second.getClusterId() == UNCLASSIFIED /*&& it->second.getPointNum() >= minInnerPts*/) {
			if (expandCluster(it->second, seedList)) {
				++clusterID;
			}
		}
		++it;
	}
}
int Dstream::createCluster(hashMapIt it) {
	int clusterID = cluster.size();
	cluster.push_back(Cluster(clusterID));
	addToCluster(it, cluster.back());
	return clusterID;
}

void Dstream::output() {
	cout << "gridnum = " << gridnum << endl;
	cout << "grid size = " << globalGridHash.size() << endl;
	cout << "hit num = " << hitnum << endl;
	cout << "cluster id = " << clusterID << endl;
	int cnt = 0;
	ofstream og("./file/griddata");
	vector<string> label;
	int gridcot = 0;
	int multicnt = 0;
	vector<double> cf;
	for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
		vector<double> cf = it->second.getCF();
		hash_map<string, int, str_hash> hash;
		og << "gridcot  =" << gridcot << endl;
		label = it->second.getPointLabel();
		og << label.at(label.size() / 2) << endl;
		cf = it->second.getCF();
		for (vector<double>::iterator cit = cf.begin(); cit != cf.end(); ++cit) {
			og << *cit << " , ";
		}
		og << endl;
		for (vector<string>::iterator lit = label.begin(); lit != label.end(); ++lit) {
			if (hash.find(*lit) == hash.end()) {
				hash[*lit] = 1;
			} else {
				hash[*lit]++;
			}
			cnt++;
			//	og << *lit << endl;
		}
		if (hash.size() > 1) {
			multicnt++;
			cout << "gridcot" << gridcot << endl;
			for (hash_map<string, int, str_hash>::iterator hit = hash.begin(); hit != hash.end(); ++hit) {
				cout << "key = " << hit->first << ",value = " << hit->second << endl;
			}
		}
		++gridcot;
	}

	ofstream outcl("./file/clsdata");
	int inclunums = 0;
	for (int i = 0; i < clusterID; ++i) {
		outcl << "clusterID = " << i << endl;
		for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
			if (it->second.getClusterId() == i) {
				label = it->second.getPointLabel();
				for (vector<string>::iterator lit = label.begin(); lit != label.end(); ++lit) {
					inclunums++;
					outcl << (*lit) << endl;
				}
			}
		}
	}

	int globalnum = 0;
	for (localHashMapIt it = localGridHash.begin(); it != localGridHash.end(); ++it) {
		globalnum += it->second.getGlobalKey().size();
	}
	cout << "local grid size" << localGridHash.size() << endl;
	cout << "globalnum = " << globalnum << endl;
	cout << "multi cnt = " << multicnt << endl;
	cout << "cnt = " << cnt << endl;
	cout << "inclunums = " << inclunums << endl;

}

void Dstream::experimentResult(clock_t time) {
	ofstream out("./file/experimentdata");
	vector<vector<string> > index;
	for (int i = 0; i < clusterID; ++i) {
		index.push_back(vector<string> ());
	}

	//cout << "clusterid" << clusterID << endl;
	int noise = 0;
	int innoisepoint = 0;
	int inunclassified = 0;
	int unclassified = 0;
	for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
		if (it->second.getClusterId() == NOISE) {
			++noise;
			innoisepoint += it->second.getPointLabel().size();
		} else if (it->second.getClusterId() == UNCLASSIFIED) {
			++unclassified;
			inunclassified += it->second.getPointLabel().size();
		} else {
			index.at(it->second.getClusterId()).push_back(it->second.getGridKey());
		}
	}

	double ssq = 0;
	double weightedSSQ = 0;
	double purity = 0;
	double weightedPurity = 0;
	string type;
	double totalSSQ = 0;
	double totalPurity = 0;
	double totalWeightedPurity = 0;
	double totalPointNum = 0;
	double totalCorrecteRate = 0;
	double totalWeightedSSQ = 0;
	double emptyClusterNum = 0;
	double correctedRate = 0;
	double noisePoint = 0;
	out << "time = " << time << endl;
	out << "readline = " << readLine << " ,eps = " << eps << " ,minpts = " << minPts << " , partition = " << globalWarp << endl;
	out << "localWarp = " << localWarp << " , borderwarp = " << borderWarp << endl;
	out << "factor of src bytes = " << src_bytes_factor << endl;
	out << "factor of dst bytes = " << dst_bytes_factor << endl;
	out << "factor of count = " << count_factor << endl;
	out << "factor of srv count = " << srv_count_factor << endl;
	out << "factor of protocol = " << protocol_factor << endl;
	out << "factor of service = " << service_factor << endl;
	out << "factor of flag = " << flag_factor << endl;
	out << "factor of error = " << serror_factor << endl;
	out << "factor of dst = " << dst_factor << endl;
	out << "factor of dst_srv = " << dst_srv_factor << endl;
	out << "factor of hot = " << hot_factor << endl;
	out << "factor of fagment = " << fagment_factor << endl;
	out << "beta = " << beta << endl;
	out << "gap = " << gap << endl;
	out << "grid num =" << gridnum << endl;
	out << "cluster num = " << clusterID << endl;
	out << "noise num = " << noise << endl;
	out << "in noise point num =" << innoisepoint << endl;
	out << "unclassified num = " << unclassified << endl;
	out << "in unclassified num = " << inunclassified << endl;
	for (int i = 0; i != clusterID; ++i) {
		if (index.at(i).size() == 0) {
			++emptyClusterNum;
			continue;
		}
		totalPointNum += resultdata(index.at(i), ssq, weightedSSQ, purity, correctedRate, weightedPurity, type);
		if (correctedRate < 0.02) {
			++noisePoint;
		} else {
			totalCorrecteRate += correctedRate;
		}
		out << "clusterid = " << i << ",  type =" << type << endl;
		out << "ssq = " << ssq << endl;
		out << "purity" << purity << endl;
		out << "correctRate = " << correctedRate << endl;
		totalSSQ += ssq;
		totalPurity += purity;
		totalWeightedPurity += weightedPurity;
		totalWeightedSSQ += weightedSSQ;
	}
	out << "empty cluster num = " << emptyClusterNum << endl;
	out << "total CorrectedRate without noise= " << (totalCorrecteRate / (clusterID - emptyClusterNum - noisePoint)) << endl;
	out << "total CorrectedRate with noise = " << (totalCorrecteRate / (clusterID - emptyClusterNum)) << endl;
	out << "total SSQ = " << (totalSSQ / (clusterID - emptyClusterNum)) << endl;
	out << "total weighted SSQ = " << (totalWeightedSSQ / totalPointNum) << endl;
	out << "total purity=" << (totalPurity / (clusterID - emptyClusterNum)) << endl;
	out << "total weighed purity = " << (totalWeightedPurity / totalPointNum) << endl;
	out << "read Point Num = " << readPointNum << endl;
	out << "total Point Num = " << totalPointNum << endl;
	out.flush();
	out.close();
	out.clear();
	//output();
}

int Dstream::resultdata(vector<string> &index, double & ssq, double & weightedSSQ, double & purity, double & correctedRate, double & weightedPurity,
		string & key) {
	typedef hash_map<string, int, str_hash> pureHash;
	typedef hash_map<string, int, str_hash>::iterator pureHashIt;
	int dos = 0;
	int r2l = 0;
	int u2l = 0;
	int probing = 0;
	int normal = 0;
	vector<double> sum;
	vector<double> data;
	vector<double> data2x;
	vector<double> sum2x;
	vector<string> pointLabel;
	string label("");
	int pointnum = 0;
	int totalPointNum = 0;
	for (std::size_t i = 0; i < index.size(); ++i) {
		data = globalGridHash.find(index.at(i))->second.getCF();
		data2x = globalGridHash.find(index.at(i))->second.getCF2();
		pointnum = globalGridHash.find(index.at(i))->second.getPointNum();
		totalPointNum += pointnum;
		if (sum.size() == 0) {
			for (vector<double>::iterator it = data.begin(); it != data.end(); ++it) {
				sum.push_back(*it * pointnum);
			}
		} else {
			for (std::size_t it = 0; it != data.size(); ++it) {
				sum[it] += (data.at(it) * pointnum);
			}
		}

		if (sum2x.size() == 0) {
			for (vector<double>::iterator it = data2x.begin(); it != data2x.end(); ++it) {
				sum2x.push_back(*it * pointnum);
			}
		} else {
			for (std::size_t it = 0; it != data2x.size(); ++it) {
				sum2x[it] += (data2x.at(it) * pointnum);
			}
		}

		pointLabel = globalGridHash.find(index.at(i))->second.getPointLabel();
		for (vector<string>::iterator it = pointLabel.begin(); it != pointLabel.end(); ++it) {
			CommonUtil::findType(*it, dos, normal, r2l, u2l, probing);
		}
	}

	for (std::size_t i = 0; i != sum.size(); ++i) {
		sum[i] /= totalPointNum;//means of vector X
	}

	ssq = 0;
	for (std::size_t i = 0; i != sum2x.size(); ++i) {
		ssq += sum2x.at(i) - pow(sum.at(i), 2) * totalPointNum;
	}

	weightedSSQ = ssq * totalPointNum;

	int max = 0;
	int max1 = 0;
	int max2 = 0;
	string key1;
	string key2;
	if (normal > dos) {
		max1 = normal;
		key1 = NORMAL_TYPE;
	} else {
		max1 = dos;
		key1 = DOS_TYPE;
	}

	if (probing > r2l) {
		max2 = probing;
		key2 = PROBING_TYPE;
	} else {
		max2 = r2l;
		key2 = R2L_TYPE;
	}

	if (max1 > max2) {
		max = max1;
		key = key1;
	} else {
		max = max2;
		key = key2;
	}

	if (max < u2l) {
		max = u2l;
		key = U2L_TYPE;
	}

	purity = double(max) / totalPointNum * 100;
	weightedPurity = purity * max;
	if (key == NORMAL_TYPE) {
		correctedRate = double(max) / totalNormal;
	} else if (key == DOS_TYPE) {
		correctedRate = double(max) / totalDos;
	} else if (key == R2L_TYPE) {
		correctedRate = double(max) / totalR2l;
	} else if (key == U2L_TYPE) {
		correctedRate = double(max) / totalU2l;
	} else if (key == PROBING_TYPE) {
		correctedRate = double(max) / totalProbing;
	}

	return totalPointNum;
}

void Dstream::initCluster(double timeArrive) {
	updateDensity(timeArrive);
	//clock_t tt = clock();
	//localBuildCluster();
	//buildCluster();
	cudaBuildCluster();
	//tt = (clock() - tt) / CLOCKS_PER_SEC;
	//cout << "time  is " << tt << endl;
	/*
	 bool flag = false;
	 hashMapIt it = globalGridHash.begin();
	 while (it != globalGridHash.end()) {
	 if (it->second.getGridStatus() == DENSE && it->second.getClusterId()==UNCLASSIFIED) {
	 it->second.setClusterId(clusterID++);
	 flag = changeClusterLabel(it->second);
	 ++it;
	 }
	 }*/
}

bool Dstream::changeClusterLabel(GlobalGrid & g) {
	bool result = false;
	vector<vector<int> > neighbor;
	vector<int> coordinate = g.getCoordinate();
	CommonUtil::getNeighbour(coordinate, neighbor);
	for (vector<vector<int> >::iterator it = neighbor.begin(); it != neighbor.end(); ++it) {
		hashMapIt hashIt = hasGlobalGrid(CommonUtil::getString(*it));
		if (hashIt != globalGridHash.end()) {
			if (hashIt->second.getGridStatus() == SPARSE)
				continue;
			hashIt->second.setClusterId(g.getClusterId());
		}
	}

	for (vector<vector<int> >::iterator it = neighbor.begin(); it != neighbor.end(); ++it) {
		hashMapIt hashIt = hasGlobalGrid(CommonUtil::getString(*it));
		if (hashIt != globalGridHash.end()) {
			if (hashIt->second.getGridStatus() == SPARSE)
				continue;
			vector<vector<int> > nn;
			CommonUtil::getNeighbour(*it, nn);
			for (vector<vector<int> >::iterator nit = nn.begin(); it != nn.end(); ++nit) {
				hashMapIt nhashIt = hasGlobalGrid(CommonUtil::getString(*it));
				if (nhashIt != globalGridHash.end()) {
					if (nhashIt->second.getGridStatus() == SPARSE)
						continue;
					if (nhashIt->second.getClusterId() > UNCLASSIFIED && nhashIt->second.getClusterId() != g.getClusterId()) {
						mergeCluster(g.getClusterId(), nhashIt->second.getClusterId());
						result = true;
					} else {
						if (nhashIt->second.getGridStatus() == TRANSITIONAL) {
							nhashIt->second.setClusterId(g.getClusterId());
							result = true;
						}
					}
				}
			}
		}
	}
	return result;
}

void Dstream::mergeCluster(int clusterid1, int clusterid2) {
	vector<string> cluster1;
	vector<string> cluster2;
	for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
		if (it->second.getClusterId() == clusterid1) {
			cluster1.push_back(it->second.getGridKey());
		} else if (it->second.getClusterId() == clusterid2) {
			cluster2.push_back(it->second.getGridKey());
		}
	}
	if (cluster1.size() > cluster2.size()) {
		for (vector<string>::iterator it = cluster2.begin(); it != cluster2.end(); ++it) {
			globalGridHash.find(*it)->second.setClusterId(clusterid1);
		}
	} else {
		for (vector<string>::iterator it = cluster1.begin(); it != cluster1.end(); ++it) {
			globalGridHash.find(*it)->second.setClusterId(clusterid2);
		}
	}
}

//join c2 to c1
void Dstream::joinToCluster(Cluster & c1, Cluster &c2) {
	vector<string> gridKey = c2.getGridKey();
	vector<vector<int> > gridCoordinate = c2.getGridCoordinate();
	int clusterID = c2.getClusterID();
	hashMapIt it;
	for (std::size_t index = 0; index != gridKey.size(); ++index) {
		it = hasGlobalGrid(gridKey.at(index));
		if (it != globalGridHash.end()) {
			it->second.setClusterId(clusterID);
		}
		c1.addGridKey(gridKey.at(index));
		c1.addGridCoordinate(gridCoordinate.at(index));
	}
	c2.remove();
}

void Dstream::adjustCluster(double timeArrive) {
	updateDensity(timeArrive);
	hashMapIt it = globalGridHash.begin();
	vector<int> largestNeighbor;
	hashMapIt neighborIt;
	vector<int> coordinate;
	int neigh = 0;
	while (it != globalGridHash.end()) {//never plus it when i will use it later
		coordinate = it->second.getCoordinate();
		if (it->second.isChanged() == true) {
			largestNeighbor = findLargestNeighbor(coordinate);
			if (largestNeighbor == coordinate) {
				it++;//cout<<"continue"<<endl;
				continue;
			}
			neigh++;
			if (it->second.getGridStatus() == DENSE) {
				//cout << "after continue" << endl;//may change later
				neighborIt = hasGlobalGrid(CommonUtil::getString(largestNeighbor));
				if (neighborIt->second.getGridStatus() == DENSE) {
					if (it->second.getClusterId() == UNCLASSIFIED) {
						if (neighborIt->second.getClusterId() > UNCLASSIFIED) {
							it->second.setClusterId(neighborIt->second.getClusterId());
						}
					} else {
						mergeCluster(it->second.getClusterId(), neighborIt->second.getClusterId());
					}
				} else if (neighborIt->second.getGridStatus() == TRANSITIONAL) {
					if (it->second.getClusterId() == UNCLASSIFIED) {
						it->second.setClusterId(neighborIt->second.getClusterId());
					} else if (it->second.getClusterId() != UNCLASSIFIED) {
						int num1 = getGridNum(it->second.getClusterId());
						int num2 = getGridNum(neighborIt->second.getClusterId());
						if (num1 > num2) {
							neighborIt->second.setClusterId(it->second.getClusterId());
						}
					}
				}
			}
			if (it->second.getGridStatus() == SPARSE) {
				it->second.setClusterId(UNCLASSIFIED);
			}
			if (it->second.getGridStatus() == TRANSITIONAL) {
				it->second.setClusterId(neighborIt->second.getClusterId());
			}
		}
		it++;
	}
}

void Dstream::reClustering(vector<int>&key) {
	hashMapIt hashIt = globalGridHash.find(CommonUtil::getString(key));
	int clusterID = createCluster(hashIt);
	vector<vector<int> > neighbor;
	Cluster & c = cluster.at(clusterID);
	CommonUtil::getNeighbour(key, neighbor);
	addToCluster(neighbor, c);
}

void Dstream::addToCluster(vector<vector<int> >& neighbor, Cluster & c) {
	hashMapIt hashIt;
	int oldClusterId = 0;
	int clusterID = c.getClusterID();
	vector<vector<int> > nOfn;
	for (vector<vector<int> >::iterator it = neighbor.begin(); it != neighbor.end(); ++it) {
		hashIt = hasGlobalGrid(CommonUtil::getString(*it));
		if (hashIt == globalGridHash.end())
			continue;
		else {
			oldClusterId = hashIt->second.getClusterId();
			if (oldClusterId == UNCLASSIFIED || oldClusterId == clusterID)
				continue;
			else {
				addToCluster(hashIt, c);
				nOfn.clear();
				CommonUtil::getNeighbour(*it, nOfn);
				addToCluster(nOfn, c);
			}
		}
	}
}

vector<int> Dstream::findLargestNeighbor(vector<int>&coordinate) {
	vector<vector<int> > neighbor;
	CommonUtil::getNeighbour(coordinate, neighbor);
	int max = 0;
	int maxindex = 0;
	for (std::size_t index = 0; index != neighbor.size(); ++index) {
		string key = CommonUtil::getString(neighbor.at(index));
		hashMapIt git = hasGlobalGrid(key);
		if (git != globalGridHash.end()) {
			if (git->second.getClusterId() > UNCLASSIFIED) {
				int temp = getGridNum(git->second.getClusterId());
				if (temp > max) {
					max = temp;
					maxindex = (int) index;
				}
			}
		}
	}
	if (maxindex == 0)
		return coordinate;
	else
		return neighbor.at(maxindex);
}

int Dstream::getGridNum(const string key) {
	hashMapIt hashIt = globalGridHash.find(key);
	int clusterID = 0;
	if (hashIt != globalGridHash.end()) {
		clusterID = hashIt->second.getClusterId();
		if (clusterID == UNCLASSIFIED)
			return -1;
		else
			return cluster.at(hashIt->second.getClusterId()).getGridNum();
	} else
		return -1;
}

int Dstream::getGridNum(int clusterid) {
	int cnt = 0;
	for (hashMapIt it = globalGridHash.begin(); it != globalGridHash.end(); ++it) {
		if (it->second.getClusterId() == clusterid) {
			++cnt;
		}
	}
	return cnt;
}

