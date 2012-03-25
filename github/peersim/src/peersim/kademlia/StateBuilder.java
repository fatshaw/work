package peersim.kademlia;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import peersim.config.Configuration;
import peersim.core.CommonState;
import peersim.core.Network;
import peersim.core.Node;
import peersim.transport.Transport;

/**
 * Initialization class that performs the bootsrap filling the k-buckets of all initial nodes.<br>
 * In particular every node is added to the routing table of every other node in the network. In the end however the various nodes
 * doesn't have the same k-buckets because when a k-bucket is full a random node in it is deleted.
 * 
 * @author Daniele Furlan, Maurizio Bonani
 * @version 1.0
 */
public class StateBuilder implements peersim.core.Control {

	private static final String PAR_PROT = "protocol";
	private static final String PAR_TRANSPORT = "transport";
	public static Map<Integer,ArrayList<String>> map = new HashMap<Integer,ArrayList<String>>(KademliaCommonConfig.BITS);
	private String prefix;
	private int kademliaid;
	private int transportid;

	public StateBuilder(String prefix) {
		this.prefix = prefix;
		kademliaid = Configuration.getPid(this.prefix + "." + PAR_PROT);
		transportid = Configuration.getPid(this.prefix + "." + PAR_TRANSPORT);
		for(int i = 0; i< KademliaCommonConfig.BITS;i++){
			ArrayList<String>s = new ArrayList<String>();
			strPerm(i,s);
			map.put(i, s);
		}
	}

	// ______________________________________________________________________________________________
	public final KademliaProtocol get(int i) {
		return ((KademliaProtocol) (Network.get(i)).getProtocol(kademliaid));
	}

	// ______________________________________________________________________________________________
	public final Transport getTr(int i) {
		return ((Transport) (Network.get(i)).getProtocol(transportid));
	}

	// ______________________________________________________________________________________________
	public static void o(Object o) {
		System.out.println(o);
	}

	// ______________________________________________________________________________________________
	public boolean execute() {

		// Sort the network by nodeId (Ascending)
		Network.sort(new Comparator<Node>() {

			public int compare(Node o1, Node o2) {
				Node n1 = (Node) o1;
				Node n2 = (Node) o2;
				KademliaProtocol p1 = (KademliaProtocol) (n1.getProtocol(kademliaid));
				KademliaProtocol p2 = (KademliaProtocol) (n2.getProtocol(kademliaid));
				//return Util.put0(p1.nodeId).compareTo(Util.put0(p2.nodeId));
				return p1.nodeId.compareTo(p2.nodeId);
			}

		});

		int sz = Network.size();
		int bit = KademliaCommonConfig.BITS;
		// for every node take 50 random node and add to k-bucket of it
		for (int i = 0; i < sz; i++) {
			Node iNode = Network.get(i);
			KademliaProtocol iKad = (KademliaProtocol) (iNode.getProtocol(kademliaid));
			//BigInteger nodeid  = iKad.nodeId;
			/*for (int k = 0; k < 100; k++) {
				KademliaProtocol jKad = (KademliaProtocol) (Network.get(CommonState.r.nextInt(sz)).getProtocol(kademliaid));
				iKad.routingTable.addNeighbour(jKad.nodeId);
			}*/
			/*BigInteger nodeid  = iKad.nodeId;
			String idStr = nodeid.toString(2);
			int bit = KademliaCommonConfig.BITS;
			int l = idStr.length();
			if(l< bit){
				for(int j = 0;j<bit - l;j++)
				{
					idStr = '0'+ idStr;
				}
			}
			try{
				for(int j = 0; j< bit;j++)
				{
					int num = bit - j -1;
					ArrayList<String>s = map.get(num);
					for(int k = 0;k<s.size();k++)
					{
						String otherNodeStr = idStr.substring(0, j);
						if(idStr.charAt(j)== '0'){
							otherNodeStr += '1' + s.get(k);
						}
						else{
							otherNodeStr += '0' + s.get(k);
						}
						BigInteger b = new BigInteger(otherNodeStr,2);
						iKad.routingTable.addNeighbour(j,b);
					}
				}
			}
			catch(Exception e){
				
			}*/

			/*for(int k = 0; k < sz; ++k)
			{
				if(k == i)continue;
				KademliaProtocol kKad = (KademliaProtocol) (Network.get(k).getProtocol(kademliaid));
				int prefix_len= bit - 1 -(int)(java.lang.Math.log10(nodeid.xor(kKad.nodeId).doubleValue()) / java.lang.Math.log10(2.0));
				iKad.routingTable.addNeighbour(prefix_len,kKad.nodeId);
			}*/
			
			
			// for every node take 50 random node and add to k-bucket of it
			for (int k = 0; k < bit*KademliaCommonConfig.K; k++) {
				KademliaProtocol jKad = (KademliaProtocol) (Network.get(CommonState.r.nextInt(sz)).getProtocol(kademliaid));
				//iKad.routingTable.addNeighbour(jKad.nodeId);
				//iKad.simpleRoutingTable.addNeighbour(jKad.nodeId);
				iKad.insertNeighbour(jKad.nodeId);
			}
			
			int start = i;
			if (i > sz - 50) {
				start = sz - 25;
			}
			for (int k = 0; k < 50; k++) {
				start = start++;
				if (start > 0 && start < sz) {
					KademliaProtocol jKad = (KademliaProtocol) (Network.get(start++).getProtocol(kademliaid));
					//iKad.routingTable.addNeighbour(jKad.nodeId);
					//iKad.simpleRoutingTable.addNeighbour(jKad.nodeId);
					iKad.insertNeighbour(jKad.nodeId);
				}
			}
			
			iKad.simpleRoutingTable.compact();
		}
		
		// add other 50 near nodes
		/*for (int i = 0; i < sz; i++) {
			Node iNode = Network.get(i);
			KademliaProtocol iKad = (KademliaProtocol) (iNode.getProtocol(kademliaid));

			int start = i;
			if (i > sz - 50) {
				start = sz - 25;
			}
			for (int k = 0; k < 50; k++) {
				start = start++;
				if (start > 0 && start < sz) {
					KademliaProtocol jKad = (KademliaProtocol) (Network.get(start++).getProtocol(kademliaid));
					//iKad.routingTable.addNeighbour(jKad.nodeId);
					//iKad.simpleRoutingTable.addNeighbour(jKad.nodeId);
					iKad.insertNeighbour(jKad.nodeId);
				}
			}
			iKad.compactRoutingTable();
		}*/
		return false;

	} // end execute()
	
	public static void strPerm(int num,ArrayList<String>a)
	{
		char [] s = new char[num];
		for(int i = 0; i< num;++i){
			s[i] = '0';
		}
		strPerm(num,a,s,0);
	}
	
	public static void strPerm(int strLen,ArrayList<String>a,char[]s,int l){
		if(l == strLen){
			a.add(new String(s));
			return;
		}
		for(int i = 0; i< 2;i++)
		{
			s[l] = (char)(i+'0');
			strPerm(strLen,a,s,l+1);
		}
	}
	
	public static void main(String[]args){
		BigInteger nodeid  = new BigInteger("11100000001111111100",2);
		String idStr = nodeid.toString(2);
		int bit =20;
		if(idStr.length()< bit){
			for(int j = 0;j<bit -  idStr.length();j++)
			{
				idStr = '0'+ idStr;
			}
		}
		for(int i = 0; i< bit;i++){
			ArrayList<String>s = new ArrayList<String>();
			strPerm(i,s);
			map.put(i, s);
		}
		Map<Integer,ArrayList<String>>m = new HashMap<Integer,ArrayList<String>>();
		for(int j = 0; j< bit;j++)
		{
			int num = bit - j -1;
			ArrayList<String>s = map.get(num);
			ArrayList<String>r = new ArrayList<String>();
			for(int k = 0;k<s.size();k++)
			{
				String otherNodeStr = idStr.substring(0, j);
				if(idStr.charAt(j)== '0'){
					otherNodeStr += '1' + s.get(k);
				}
				else{
					otherNodeStr += '0' + s.get(k);
				}
				r.add(otherNodeStr);
			}
			m.put(j, r);
		}
		
		
		Iterator<Integer>it  = m.keySet().iterator();
		while(it.hasNext())
		{
			int key = it.next();
			ArrayList<String>a = m.get(key);
			System.out.println("key = "+key);
			for(String s: a){
				System.out.println("s = "+s);
			}
		}
	}

}
