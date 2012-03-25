package example.aggregation;

import java.util.HashSet;
import java.util.Set;

import peersim.chord.ChordProtocol;
import peersim.core.CommonState;
import peersim.core.Network;

public class ScaleFreeStatistic {
	public static double incrementalInfected = 0;
	
	public static double susceptible = Network.size();
	
	public static double infected = 0; 
	
	public static double immunizedFromSusceptible = 0;
	
	public static double immunizedFromInfected = 0;
	
	public static double removed = 0;
	
	public static double u = 0.06;//average rate of removal of susceptible hosts
	
	public static double r = 0.05;//average rate of removal of infectious hosts

	public static double onlinerate[] ={0.40,0.58,0.77,0.91,0.99,0.995,0.985,0.974,0.957,0.965,0.97,0.978,0.985,0.91,0.8,0.71,0.62,0.51,0.38,0.28,0.21,0.20,0.22,0.3};

	public static int current_online = 0;
	
	public static Set<Long> visitorSet = new HashSet<Long>(Network.size());
	
	public static void add()
	{
		incrementalInfected++;
		infected++;
		susceptible--;
	}
	
	public static void twoFactor(int sz,long currenttime,int pid,boolean isOnline)
	{
		double onRate = 1;
		if(currenttime >2){
			if(isOnline)
			{
				onRate = onlinerate[(int) ((currenttime - 3)%onlinerate.length)];
			}
			else
			{
				onRate = 1;	
			}
			double rt = 0;
			double qt = 0;
			rt = r * infected*onRate;
			qt = u * susceptible*onRate;
			rt = (int)rt;
			qt = (int)qt;
			double temprt = 0, tempqt = 0;
			for(int i = 0; i< sz;i++)
			{
				ScaleFreeProtocol sfp = (ScaleFreeProtocol)Network.get(CommonState.r.nextInt(sz)).getProtocol(pid);
				if(sfp.isOnline == false)continue;
				if(sfp.hasImmunized == true)continue;
				if(sfp.hasVisited == true)
				{
					if(temprt < rt){
						sfp.hasImmunized = true;
						immunizedFromInfected+=1;
						temprt++;
					}
				}
				else{
					if(tempqt < qt){
						sfp.hasImmunized = true;
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
	
	public static void onlineRate(int sz,long currenttime, int pid,boolean isOnline)
	{
		if(isOnline == false)return;
		//currenttime >2,it means that when the first node send requests for propagation all the nodes are online
		//the first node send requests at the time 1 and will arrive the propagated nodes at the time 2
		//so we should think about online rate from the time 3
		if(currenttime >2){ 
			int index = (int)currenttime - 3;
			index = index % onlinerate.length;
			int onlineNodeNum = (int)( onlinerate[index]*Network.size());
			for(int i = 0;i<sz;i++)
			{
				ScaleFreeProtocol sfp = (ScaleFreeProtocol)Network.get(i).getProtocol(pid);
				sfp.isOnline = false;
			}
			
			int tempspan = 0;
			for(int i = 0; i< onlineNodeNum;i++)
			{
				ScaleFreeProtocol sfp;
				while(true){
					sfp = (ScaleFreeProtocol)Network.get(CommonState.r.nextInt(sz)).getProtocol(pid);
					if(sfp.isOnline == false)break;
				}
				sfp.isOnline = true;
			}
		}
	}

	public static void output(int currenttime,int sz) {
		// TODO Auto-generated method stub
		String s = String.format("[time=%d]:[N=%d current nodes UP],[Susceptible= %d],[remove = %d], [ incrementalVisited = %d] ,[totalVisited = %d] ", currenttime, sz,(int)susceptible,(int)removed,(int)incrementalInfected,(int)infected);
		System.out.println(s);
		System.out.flush();
		incrementalInfected = 0;
		visitorSet.clear();
	}
}
