package example.aggregation;

import peersim.core.Protocol;

public class ScaleFreeProtocol implements Protocol
{
	public boolean hasVisited;
	public boolean hasImmunized;
	public boolean isOnline;
	public String name;
	public Object clone()
	{
		ScaleFreeProtocol sfp=null;
		try {
			sfp= (ScaleFreeProtocol)super.clone();
		} catch (CloneNotSupportedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return sfp;
	}
	
	public ScaleFreeProtocol(String name){
		this.name = name;
		this.hasImmunized = false;
		this.hasVisited = false;
		this.isOnline = true;
	}
}
