package example.aggregation;

import peersim.config.Configuration;
import peersim.core.CommonState;
import peersim.core.Control;
import peersim.core.Network;
import peersim.edsim.EDSimulator;

public class ScaleFreeTrafficGenerator implements Control {

	private int pid;
	
	public ScaleFreeTrafficGenerator(String prefix){
		this.pid = Configuration.getPid(prefix + "." + "protocol");
	}
	
	@Override
	public boolean execute() 
	{
		//EDSimulator.add(1, null,Network.get(CommonState.r.nextInt(Network.size())),this.pid);
		EDSimulator.add(1, new ScaleFreeStartMsg(),Network.get(0),this.pid);
		return false;
	}
}
