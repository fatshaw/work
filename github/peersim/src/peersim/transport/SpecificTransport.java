package peersim.transport;
import peersim.edsim.*;
import peersim.core.Node;
import peersim.chord.LookUpMessage;
public class SpecificTransport implements Transport{

	private long delay;
	
	public SpecificTransport(String prefix)
	{
		
	}
	
	@Override
	public long getLatency(Node src, Node dest) {
		// TODO Auto-generated method stub
		return delay;
	}

	@Override
	public void send(Node src, Node dest, Object msg, int pid) {
		// TODO Auto-generated method stub
		LookUpMessage message= (LookUpMessage)msg;
		this.delay =  message.delay;
		EDSimulator.add(delay, msg, dest, pid);
	}
	
	public Object clone()
	{
		return this;
	}

}
