import weka.core.Instance;

/**
 * Flow Instance extends from Weka Instance for P2P Flow Detection.<br/>
 * Add flow addr(IP:PORT) to instance
 * 
 * @author Administrator
 * @edit:обнГ04:12:35
 */
public class FlowInstance extends Instance {
	/**
	 * serialVersionUID is useless in this application
	 */
	private static final long serialVersionUID = 14823453651289232L;

	public String flow_addr;

	public FlowInstance(String flow_addr, Instance instance) {
		super(instance);
		this.setDataset(instance.dataset());
		this.flow_addr = flow_addr;
	}
}
