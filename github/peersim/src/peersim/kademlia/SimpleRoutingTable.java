package peersim.kademlia;

import java.math.BigInteger;

import peersim.core.CommonState;

public class SimpleRoutingTable {
	private int []kBucketNum = new int[KademliaCommonConfig.BITS];
	private int [] allNodesBefore = new int[KademliaCommonConfig.BITS];
	private BigInteger[]allNodes = new BigInteger[KademliaCommonConfig.K * KademliaCommonConfig.BITS];
	private int allNodesCnt;
	public BigInteger nodeId;
	private BigInteger [] nodes;
	
	public void addNeighbour(BigInteger node)
	{
		if(node == nodeId)return;
		int prefix_len= 0;
		try
		{
			prefix_len= Util.prefixLen(nodeId, node);
		if(kBucketNum[prefix_len]<KademliaCommonConfig.K)
		{
			allNodes[prefix_len*KademliaCommonConfig.K + kBucketNum[prefix_len]]=node;
			kBucketNum[prefix_len]++;
			allNodesCnt++;
		}
		}
		catch(Exception ex)
		{
			System.err.println("prefix = "+prefix_len);
			System.err.println("nodeid  = "+nodeId.toString()+"node = "+node.toString());
			System.err.println("ex = "+ex.getMessage());
		}
	}
	
	public void compact()
	{
		nodes = new BigInteger[allNodesCnt];
		int j = 0;
		for(int i = 0;i<allNodes.length;++i)
		{
			if(allNodes[i] == null)continue;
			nodes[j++]  = allNodes[i];
		}		
		allNodes = null;
		allNodesBefore[0]=0;
		for(int i = 1;i<kBucketNum.length;++i)
		{
			allNodesBefore[i]+=allNodesBefore[i-1]+kBucketNum[i-1];
		}
	}
	
	public BigInteger getNeighbor(int kbucketNo)
	{
		int start = allNodesBefore[kbucketNo];
		int end = start+kBucketNum[kbucketNo];
		if(end - start <=0)return null;
		int rand = CommonState.r.nextInt(end-start)+start;
		if(rand < nodes.length)return nodes[rand];
		return null;
	}
}
