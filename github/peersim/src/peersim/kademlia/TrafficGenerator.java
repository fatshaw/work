package peersim.kademlia;

import java.math.BigInteger;

import peersim.config.Configuration;
import peersim.core.CommonState;
import peersim.core.Control;
import peersim.core.Network;
import peersim.core.Node;
import peersim.edsim.EDSimulator;

/**
 * This control generates random search traffic from nodes to random destination node.
 * 
 * @author Daniele Furlan, Maurizio Bonani
 * @version 1.0
 */

// ______________________________________________________________________________________________
public class TrafficGenerator implements Control {

	// ______________________________________________________________________________________________
	/**
	 * MSPastry Protocol to act
	 */
	private final static String PAR_PROT = "protocol";

	/**
	 * MSPastry Protocol ID to act
	 */
	private final int pid;
	
	// ______________________________________________________________________________________________
	public TrafficGenerator(String prefix) {
		pid = Configuration.getPid(prefix + "." + PAR_PROT);

	}

	// ______________________________________________________________________________________________
	/**
	 * generates a random find node message, by selecting randomly the destination.
	 * 
	 * @return Message
	 */
	public static Message generateFindNodeMessage(int pid) {
		return generateFindNodeMessage(pid,null);
	}
	
	public static Message generateFindNodeMessage(int pid, BigInteger targetID)
	{
		Message m = Message.makeFindNode("Automatically Generated Traffic");
		m.timestamp = CommonState.getTime();
		Node targetNode = null;
		
		// existing active destination node
		if(targetID == null){
			targetNode = Network.get(CommonState.r.nextInt(Network.size()));
			while (!targetNode.isUp()) {
				targetNode = Network.get(CommonState.r.nextInt(Network.size()));
			}
			m.dest = ((KademliaProtocol) (targetNode.getProtocol(pid))).nodeId;
		}
		else
		{
			m.dest = targetID;
		}
		
		return m;
	}
	
	public static Message generatorMessage(int messageType)
	{
		Message m = new Message(messageType);
		m.timestamp = CommonState.getTime();
		return m;
	}
	
	// ______________________________________________________________________________________________
	/**
	 * every call of this control generates and send a random find node message
	 * 
	 * @return boolean
	 */
	public boolean execute() {
		Node start;
		do {
			start = Network.get(CommonState.r.nextInt(Network.size()));
		} while ((start == null) || (!start.isUp()));

		// send message
		EDSimulator.add(0, generatorMessage(Message.MSG_SEARCH),start, pid);

		return false;
	}

	
	// ______________________________________________________________________________________________

} // End of class
// ______________________________________________________________________________________________
