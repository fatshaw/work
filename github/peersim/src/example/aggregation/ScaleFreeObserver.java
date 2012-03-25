package example.aggregation;

import peersim.config.Configuration;
import peersim.core.CommonState;
import peersim.core.Control;
import peersim.core.Network;
import peersim.core.Node;
import peersim.core.Linkable;
public class ScaleFreeObserver implements Control{
	public static int pid;
	public static int linkid;
	public static int isinit  =0;
	public ScaleFreeObserver(String prefix)
	{
		this.pid = Configuration.getPid(prefix + "." + "protocol");
		this.linkid = Configuration.getPid(prefix + "." + "link");
	}
	
	public void outputDegree()
	{
		int time = CommonState.getIntTime();
		if(time <1)return;
		if(isinit ==1)return;
		isinit  = 1;
		int sz = Network.size();
		for(int i = 0 ; i < sz;++i){
			Node n = Network.get(i);
			Linkable link = (Linkable)n.getProtocol(this.linkid);
			for(int j = 0 ; j < link.degree();++j){
				System.out.print(link.getNeighbor(j).getID()+",");
			}
			System.out.println();
		}
		System.out.println();
		System.out.flush();
	}
	
	@Override
	public boolean execute() {
		//TODO
		//output statistic of ScaleFreePropagate.
		//deal with factors including two-factor and onlinerate.
		int time = CommonState.getIntTime();
		ScaleFreeStatistic.output(time,Network.size());
		outputDegree();
		boolean isOnline = true;
		if(time>2)
		{
			ScaleFreeStatistic.twoFactor(Network.size(), time,pid,isOnline);
			ScaleFreeStatistic.onlineRate(Network.size(),time, pid,isOnline);
		}
		
		return false;
	}

}
