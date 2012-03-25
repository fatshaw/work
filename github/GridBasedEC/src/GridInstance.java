import java.util.ArrayList;
import java.util.List;

import weka.core.Instance;

/**
 * Grid Instance,all the instances are represented by one instance use center of
 * grid to represent all the instances of the grid record all the instances in
 * the grid so that we can label them
 * 
 * @author Administrator
 * @edit:ÏÂÎç04:14:07
 */
public class GridInstance {
	List<Instance> instanceInGrid;
	Instance gridInstance;

	public GridInstance(Instance instance) {
		this.gridInstance = new Instance(instance);
		this.gridInstance.setDataset(instance.dataset());
		instanceInGrid = new ArrayList<Instance>();
		instanceInGrid.add(instance);
	}

	/**
	 * Add an instance into grid
	 * 
	 * @param instance
	 *            add instance into grid
	 */
	public void addInstance(Instance instance) {
		this.instanceInGrid.add(instance);
		for (int j = 0; j < instance.numAttributes() - 1; j++) {
			this.gridInstance.setValue(j, this.gridInstance.value(j) + instance.value(j));
		}
	}

	/**
	 * calculate grid center
	 */
	public void average() {
		if (this.instanceInGrid.size() <= 0) {
			return;
		}
		for (int j = 0; j < this.gridInstance.numAttributes() - 1; j++) {
			gridInstance.setValue(j, gridInstance.value(j) / this.instanceInGrid.size());
		}

	}

	public Instance getInstance() {
		return gridInstance;
	}

	/**
	 * label all the instances in the grid with label of grid
	 */
	public void setGridClassValue() {
		for (int i = 0; i < instanceInGrid.size(); ++i) {
			instanceInGrid.get(i).setClassValue(gridInstance.classValue());
			String info;
			if (instanceInGrid.get(i) instanceof FlowInstance) {
				FlowInstance flowInstance = (FlowInstance) instanceInGrid.get(i);
				info = flowInstance.flow_addr + "," + instanceInGrid.get(i).classAttribute().value((int) gridInstance.classValue());
			} else {
				info = instanceInGrid.get(i).classAttribute().value((int) gridInstance.classValue());
			}
			Util.debug(info);
			Util.fifoWrite(info);
		}
	}
}