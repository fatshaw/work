package peersim.kademlia;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import peersim.config.Configuration;
import peersim.core.CommonState;
import peersim.core.Control;
import peersim.core.Network;
import peersim.util.IncrementalStats;

/**
 * This class implements a simple observer of search time and hop average in finding a node in the network
 * 
 * @author Daniele Furlan, Maurizio Bonani
 * @version 1.0
 */
public class KademliaObserver implements Control {

	/**
	 * keep statistics of the number of hops of every message delivered.
	 */
	public static IncrementalStats hopStore = new IncrementalStats();

	/**
	 * keep statistics of the time every message delivered.
	 */
	public static IncrementalStats timeStore = new IncrementalStats();

	/**
	 * keep statistic of number of message delivered
	 */
	public static IncrementalStats msg_deliv = new IncrementalStats();

	/**
	 * keep statistic of number of find operation
	 */
	public static IncrementalStats find_op = new IncrementalStats();
	
	//public static IncrementalStats totalVisited = new IncrementalStats();
	
	//public static IncrementalStats incrementalVisited = new IncrementalStats();
	
		
	public static double incrementalInfected = 0;
	
	public static double susceptible = Network.size();
	
	public static double infected = 0; 
	
	public static double immunizedFromSusceptible = 0;
	
	public static double immunizedFromInfected = 0;
	
	public static double removed = 0;
	
	public static double u = 0.06;
	
	public static double r = 0.05;

	public static double onlinerate[] ={0.40,0.58,0.77,0.91,0.99,0.995,0.985,0.974,0.957,0.965,0.97,0.978,0.985,0.91,0.8,0.71,0.62,0.51,0.38,0.28,0.21,0.20,0.22,0.3};
	//public static double onlinerate[] ={0.62,0.51,0.38,0.28,0.21,0.20,0.22,0.3,0.40,0.58,0.77,0.91,0.99,0.995,0.985,0.974,0.957,0.965,0.97,0.978,0.985,0.91,0.8,0.71};
	public static int current_online = 0;
	
	/** Parameter of the protocol we want to observe */
	private static final String PAR_PROT = "protocol";

	/** Protocol id */
	private int pid;

	/** Prefix to be printed in output */
	private String prefix;
	
	
	public static boolean useTwoFactor = true;
	public static boolean useOnlineRate = true;

	public KademliaObserver(String prefix) {
		this.prefix = prefix;
		pid = Configuration.getPid(prefix + "." + PAR_PROT);
	}

	/**
	 * print the statistical snapshot of the current situation
	 * 
	 * @return boolean always false
	 */
	public boolean execute() {
		// get the real network size
		
		int sz = Network.size();
		for (int i = 0; i < Network.size(); i++)
			if (!Network.get(i).isUp())
				sz--;

		//String s = String.format("[time=%d]:[N=%d current nodes UP] [D=%f msg deliv] [%f min hop] [%f average hop] [%f max hop] [%d min timeduration] [%d msec average timeduration] [%d max timeduration], [find operation num = %d sum], [ incrementalVisited = %d] ,[totalVisited = %d] ", CommonState.getTime(), sz, msg_deliv.getSum(), hopStore.getMin(), hopStore.getAverage(), hopStore.getMax(), (int) timeStore.getMin(), (int) timeStore.getAverage(), (int) timeStore.getMax(), (int)find_op.getSum(),(int)incrementalVisited.getSum(),(int)totalVisited.getSum());
		String s = String.format("[time=%d]:[N=%d current nodes UP],[Susceptible= %d],[remove = %d], [ incrementalVisited = %d] ,[totalVisited = %d] ", CommonState.getTime(), sz,(int)susceptible,(int)removed,(int)incrementalInfected,(int)infected);
		incrementalInfected = 0;
		if (CommonState.getTime() == 3600000) {
			// create hop file
			try {
				File f = new File("D:/simulazioni/hopcountNEW.dat"); // " + sz + "
				f.createNewFile();
				BufferedWriter out = new BufferedWriter(new FileWriter(f, true));
				out.write(String.valueOf(hopStore.getAverage()).replace(".", ",") + ";\n");
				out.close();
			} catch (IOException e) {
			}
			// create latency file
			try {
				File f = new File("D:/simulazioni/latencyNEW.dat");
				f.createNewFile();
				BufferedWriter out = new BufferedWriter(new FileWriter(f, true));
				out.write(String.valueOf(timeStore.getAverage()).replace(".", ",") + ";\n");
				out.close();
			} catch (IOException e) {
			}

		}
		long currenttime  = CommonState.getTime();
		
		/*
		if(useOnlineRate == true){
			if(currenttime > 0){
				int index = (int)currenttime -1;
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
					KademliaProtocol kad = (KademliaProtocol)Network.get(CommonState.r.nextInt(sz)).getProtocol(pid);
					if(kad.isOnline == setOnline)continue;
					if(tempspan < span){
						kad.isOnline = setOnline;
						if(kad.isOnline==true){
							if(kad.canSpread > 0){
								System.err.println("can spread");
								kad.reSend();
							}
						}
						tempspan++;
					}
					else break;
				}
				current_online = onlineNodeNum;
			}
		}
		*/
		int index = (int)currenttime-3;
		index = index > 0 ? index : 0;
		double onrate = 1;
		if(useOnlineRate == true){
			if(currenttime > 2){//the same with chord,start from time 3
				index = index % onlinerate.length;
				onrate = onlinerate[index];
				int onlineNodeNum = (int)( onrate*Network.size());
				for(int i = 0;i<sz;i++)
				{
					KademliaProtocol kad = (KademliaProtocol)Network.get(i).getProtocol(pid);
					kad.isOnline = false;
				}
				
				
				int tempspan = 0;
				for(int i = 0; i< onlineNodeNum;i++)
				{
					KademliaProtocol kad;
					while(true){
						kad = (KademliaProtocol)Network.get(CommonState.r.nextInt(sz)).getProtocol(pid);
						if(kad.isOnline == false)break;
					}
					kad.isOnline = true;
				}
			}
		}
		
		if(KademliaObserver.useTwoFactor == true){
			if(currenttime > 2){
				double rt = 0;
				double qt = 0;
				rt = r * infected * onrate;
				qt = u * susceptible* onrate;
				rt = (int)rt;
				qt = (int)qt;
				double temprt = 0, tempqt = 0;
				for(int i = 0; i< sz;i++)
				{
					//KademliaProtocol kad = (KademliaProtocol)Network.get(i).getProtocol(pid);
					KademliaProtocol kad = (KademliaProtocol)Network.get(CommonState.r.nextInt(sz)).getProtocol(pid);
					if(useOnlineRate == true && kad.isOnline == false)continue;
					if(kad.hasImmunized == true)continue;
					if(kad.hasVisited == true)
					{
						if(temprt < rt){
							kad.hasImmunized = true;
							immunizedFromInfected+=1;
							temprt++;
						}
					}
					else{
						if(tempqt < qt){
							kad.hasImmunized = true;
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
		
		System.err.println(s);

		return false;
	}
}

