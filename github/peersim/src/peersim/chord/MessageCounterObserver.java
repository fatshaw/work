/**
 * 
 */
package peersim.chord;

import java.util.ArrayList;

import peersim.core.CommonState;
import peersim.core.Control;
import peersim.core.Network;
import peersim.config.Configuration;

/**
 * @author Andrea
 * 
 */
public class MessageCounterObserver implements Control {

	private static final String PAR_PROT = "protocol";

	private final String prefix;

	private final int pid;
	
	
	public static double incrementalInfected = 0;
	
	public static double susceptible = Network.size();
	
	public static double infected = 0; 
	
	public static double immunizedFromSusceptible = 0;
	
	public static double immunizedFromInfected = 0;
	
	public static double removed = 0;
	
	public static double u = 0.06;//average rate of removal of susceptible hosts
	
	public static double r = 0.05;//average rate of removal of infectious hosts

	public static double onlinerate[] ={0.40,0.58,0.77,0.91,0.99,0.995,0.985,0.974,0.957,0.965,0.97,0.978,0.985,0.91,0.8,0.71,0.62,0.51,0.38,0.28,0.21,0.20,0.22,0.3};
	//public static double onlinerate[] ={0.39,0.74,0.9925,0.98,0.961,0.974,0.9475,0.755,0.565,0.43,0.35,0.26};
	//public static double onlinerate[] ={0.62,0.51,0.38,0.28,0.21,0.20,0.22,0.3,0.40,0.58,0.77,0.91,0.99,0.995,0.985,0.974,0.957,0.965,0.97,0.978,0.985,0.91,0.8,0.71};
	public static int current_online = 0;
	

	/**
	 * 
	 */
	public MessageCounterObserver(String prefix) {
		this.prefix = prefix;
		this.pid = Configuration.getPid(prefix + "." + PAR_PROT);
	}

	
	/**
	 *  my version
	 *  
	 *  it is a simple procedure
	 *  
	 *  report the incremental infected nodes and total infected nodes in the network at the current time.
	 * 
	 * 	@return false always
	 */
	public boolean execute()
	{
		long currenttime = CommonState.getTime();
		int sz = Network.size();
		String s = String.format("[time=%d]:[N=%d current nodes UP],[Susceptible= %d],[remove = %d], [ incrementalVisited = %d] ,[totalVisited = %d] ", currenttime, sz,(int)susceptible,(int)removed,(int)incrementalInfected,(int)infected);
		incrementalInfected = 0;
		System.err.println(s);
		int index = (int)currenttime - 3;
		index = index > 0?index:0;
		index = index % onlinerate.length;
		onlineRate(sz,currenttime);
		twoFactor(sz,currenttime,onlinerate[index]);
		//twoFactor(sz,currenttime,1);
		return false;
	}
	
	public void twoFactor(int sz,long currenttime,double onRate)
	{
		if(currenttime >2){
			double rt = 0;
			double qt = 0;
			rt = r * infected*onRate;
			qt = u * susceptible*onRate;
			rt = (int)rt;
			qt = (int)qt;
			double temprt = 0, tempqt = 0;
			for(int i = 0; i< sz;i++)
			{
				ChordProtocol cp = (ChordProtocol)Network.get(CommonState.r.nextInt(sz)).getProtocol(pid);
				if(cp.isOnline == false)continue;
				if(cp.hasImmunized == true)continue;
				if(cp.hasVisited == true)
				{
					if(temprt < rt){
						cp.hasImmunized = true;
						immunizedFromInfected+=1;
						temprt++;
					}
				}
				else{
					if(tempqt < qt){
						cp.hasImmunized = true;
						immunizedFromSusceptible+=1;
						tempqt++;
					}
				}
				
				if(temprt >= rt && tempqt >= qt)break;
			}
			
			
			susceptible -= tempqt;
			infected  -= temprt;
			removed += tempqt;
			removed += temprt;
		}
	}
	
	public void onlineRate(int sz,long currenttime)
	{
		//currenttime >2,it means that when the first node send requests for propagation all the nodes are online
		//the first node send requests at the time 1 and will arrive the propagated nodes at the time 2
		//so we should think about online rate from the time 3
		if(currenttime >2){ 
			int index = (int)currenttime - 3;
			index = index % onlinerate.length;
			int onlineNodeNum = (int)( onlinerate[index]*Network.size());
			for(int i = 0;i<sz;i++)
			{
				ChordProtocol cp = (ChordProtocol)Network.get(i).getProtocol(pid);
				cp.isOnline = false;
			}
			
			int tempspan = 0;
			for(int i = 0; i< onlineNodeNum;i++)
			{
				ChordProtocol cp;
				while(true){
					cp = (ChordProtocol)Network.get(CommonState.r.nextInt(sz)).getProtocol(pid);
					if(cp.isOnline == false)break;
				}
				cp.isOnline = true;
			}
		}
		/*
		if(currenttime > 2){
			int index = (int)currenttime -3;
			index = index % onlinerate.length;
			int onlineNodeNum = (int)( onlinerate[index]*Network.size());
			int span = onlineNodeNum - current_online;
			boolean setOnline = false;
			if(span < 0){
				setOnline = false;
			}
			else if (span > 0){
				setOnline = true;
			}
			else {
				//span == 0
			}
			span = java.lang.Math.abs(span);
			int tempspan = 0;
			for(int i = 0;i < sz;i++){
				ChordProtocol cp = (ChordProtocol)Network.get(CommonState.r.nextInt(sz)).getProtocol(pid);
				if(cp.isOnline == setOnline)continue;
				if(tempspan < span){
					cp.isOnline = setOnline;
					tempspan++;
				}
				else break;
			}
			current_online = onlineNodeNum;
		}*/
	}
}
