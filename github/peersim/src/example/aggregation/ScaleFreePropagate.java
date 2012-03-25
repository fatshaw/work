package example.aggregation;


import peersim.config.Configuration;
import peersim.config.FastConfig;
import peersim.core.CommonState;
import peersim.core.Linkable;
import peersim.core.Node;
import peersim.edsim.EDProtocol;
import peersim.edsim.EDSimulator;

public class ScaleFreePropagate implements EDProtocol{

	private int pid;
    private static final double infectedRatio = 0.4; 
	public ScaleFreePropagate(String prefix)
	{
		this.pid = Configuration.getPid(prefix + "." + "nodeproid");
	}
	
	public void setVisited(Node node,boolean needCheck)
	{		
		ScaleFreeProtocol  sfp = (ScaleFreeProtocol)node.getProtocol(pid);
		if(sfp.isOnline && sfp.hasImmunized == false && sfp.hasVisited == false){
			if(needCheck && CommonState.r.nextDouble()>=infectedRatio)return;
			sfp.hasVisited = true;
			ScaleFreeStatistic.add();
		}
	}
	
	public boolean canVisit(Node node,boolean can){
		ScaleFreeProtocol  sfp = (ScaleFreeProtocol)node.getProtocol(pid);
		if(sfp.hasImmunized == true || sfp.hasVisited == true || (can &&sfp.isOnline == false)){
				return false;
		}
		return true;
	}
	
	public void processEvent( Node node, int protocolId, Object event ){
		if(canVisit(node,true) == false)return;
		
		if(event == null || event.getClass() != ScaleFreeStartMsg.class)
		{
			setVisited(node,true);
		}
		else
		{
			setVisited(node,false);
		}
		
        int linkableID = FastConfig.getLinkable(protocolId);
        Linkable linkable = (Linkable) node.getProtocol(linkableID);
        int degree =linkable.degree() ; 
        if(degree > 0)
        {
        	for(int i = 0 ; i <linkable.degree();++i)
        	{
        		//Node j = linkable.getNeighbor(CommonState.r.nextInt(degree));
        		Node j = linkable.getNeighbor(i);
        		if(canVisit(j,false)){
        			int r = CommonState.r.nextInt(1);
        			EDSimulator.add(1+r,null,j,protocolId);
        		}
        	}
        }
	}
	
	
	public Object clone()
	{
		ScaleFreePropagate sfp = null;
		try {
			sfp = (ScaleFreePropagate)super.clone();
		} catch (CloneNotSupportedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return sfp;
	}

}
