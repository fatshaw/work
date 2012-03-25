import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Reader;
import java.util.LinkedList;
import java.util.Random;

import weka.core.Instance;
import weka.core.Instances;

/**
 * Manage Data,including training data,test data,output data Split Data into
 * blocks with same instances to simulate data stream environment. Calculate
 * correct rate of each class
 * 
 * @author Administrator
 * @edit:ÏÂÎç04:17:29
 */
public class DataManager {
	protected Instances allData;
	protected Instances nextData;
	protected int offset;
	protected LinkedList<Instances> InstancesCache;
	protected boolean neededOutput;
	protected Instances outputData;
	protected LinkedList<Instance> flowInstances;
	private static String BOTNET = "peacomm";
	private static String NORMAL = "normal.";

	public DataManager(Instances allData) throws IOException {
		setAllData(allData);
	}

	public DataManager(Reader reader) throws IOException {
		setAllData(new Instances(reader));
	}

	public DataManager() {
	}

	public void setAllData(Instances allData) throws IOException {
		this.neededOutput = false;
		this.allData = allData;
		this.allData.setClassIndex(this.allData.numAttributes() - 1);
		this.nextData = new Instances(PublicVariable.header);
		this.InstancesCache = new LinkedList<Instances>();
		this.flowInstances = new LinkedList<Instance>();
	}

	/**
	 * Get r consecutive data block
	 * 
	 * @param r
	 *            number of consecutive data block
	 * @param chunksize
	 *            size of each data block
	 * @return instance containing r consecutive data block
	 */
	public Instances getChunkData(int r, int chunksize) {
		if (offset >= allData.numInstances()) {
			return null;
		}
		Instances instances = new Instances(PublicVariable.header);
		boolean skip = false;
		for (int i = 0; i < r; ++i) {
			for (int j = 0; j < chunksize; ++j) {
				if (j + offset >= allData.numInstances()) {
					skip = true;
					break;
				}
				instances.add(allData.instance(j + offset));
			}
			offset += chunksize;
			if (skip == true) {
				break;
			}
		}
		return instances;
	}

	/**
	 * Get next instance from data stream
	 * 
	 * @return next instance
	 */
	synchronized public Instance getNextInstance() {
		if (offset >= allData.numInstances()) {
			if (this.flowInstances.size() > 0) {
				return this.flowInstances.poll();
			} else {
				return null;

			}
		}
		return allData.instance(offset++);
	}

	public double calculateCorrectRate(DataManager dm) {
		double truePositive = 0;
		double falsePositive = 0;
		double normalNum = 0;
		double abnormalNum = 0;

		double correctNum = 0;
		String original, test;
		Instance testInstance, labeledInstance;
		for (int i = 0; i < allData.numInstances(); ++i) {
			testInstance = allData.instance(i);
			labeledInstance = dm.allData.instance(i);
			original = labeledInstance.classAttribute().value((int) labeledInstance.classValue());
			test = testInstance.classAttribute().value((int) testInstance.classValue());
			if (PublicVariable.ISP2P == false) {
				if (original.compareTo(NORMAL) == 0) {
					normalNum++;
				} else {
					abnormalNum++;
				}
				if (allData.instance(i).classValue() == dm.allData.instance(i).classValue()) {
					correctNum++;
					if (original.compareTo(NORMAL) != 0) {
						truePositive++;// if classification is right we can
						// calculate true positive
					}
				} else {
					if (original.compareTo(NORMAL) == 0) {
						falsePositive++;// if classification is not right, we
						// can calculate false positive.
					}
				}
			} else if (PublicVariable.ISP2P == true) {
				if (original.compareTo(BOTNET) == 0) {
					abnormalNum++;
				} else {
					normalNum++;
				}

				if (testInstance.classValue() == labeledInstance.classValue()) {
					correctNum++;
					if (original.compareTo(BOTNET) == 0) {
						truePositive++;
					}
				} else {
					if (original.compareTo(BOTNET) != 0 && test.compareTo(BOTNET) == 0) {
						falsePositive++;
					}
				}
			}
		}
		Util.info("truePositive=" + truePositive + ", falsePositive=" + falsePositive);
		Util.info("normalNum=" + normalNum + ", abnormalNum=" + abnormalNum);
		Util.info("tp=" + truePositive / abnormalNum + ",fp=" + falsePositive / normalNum);
		return correctNum / allData.numInstances();
	}

	public int getInstancesNum() {
		return allData.numInstances();
	}

	public void save() throws IOException {
		save(PublicVariable.PATH_PREFIX + "labeledResult.arff");
	}

	public void save(String filename) throws IOException {
		if (filename == null) {
			filename = PublicVariable.PATH_PREFIX + "labeledResult.arff";
		}

		BufferedWriter bw = new BufferedWriter(new FileWriter(filename));
		bw.write(allData.toString());
		bw.newLine();
		bw.flush();
		bw.close();
	}

	public void reset() {
		this.offset = 0;
	}

	public int remaining() {
		return this.allData.numInstances() - offset + this.flowInstances.size();
	}

	public boolean hasNext() {
		if (offset < allData.numInstances() || this.flowInstances.size() > 0) {
			return true;
		}
		return false;
	}

	public void addInstances(Reader reader) throws Exception {
		BufferedReader br = new BufferedReader(reader);
		String line = "";
		Instance instance;
		int cnt = 0;
		while ((line = br.readLine()) != null) {
			String[] attrs = line.split(",");
			int start = attrs[0].startsWith("[") == true ? 1 : 0;
			String addr;
			if (start == 1) {
				addr = attrs[0];
				instance = new FlowInstance(addr, (Instance) PublicVariable.sample.copy());
			} else {
				instance = (Instance) PublicVariable.sample.copy();
			}

			for (int i = 0; i < instance.numAttributes() - 1; ++i) {
				instance.setValue(i, Double.valueOf(attrs[i + start]));
			}
			instance.setClassValue(attrs[attrs.length - 1]);
			++cnt;
			addFlowInstance(instance);
		}
		Util.info("addInstance cnt = " + cnt);
	}

	synchronized protected void addIntancesCache() {
		this.InstancesCache.add(this.nextData);
		this.nextData = new Instances(PublicVariable.header);
	}

	protected void addInstance(Instances instances) {
		for (int i = 0; i < instances.numInstances(); i++) {
			this.nextData.add(instances.instance(i));
		}

		if (this.nextData != null && this.nextData.numInstances() >= PublicVariable.INSTANCE_SIZE) {
			addIntancesCache();
		}
	}

	protected void addInstance(Instance instance) {
		this.nextData.add(instance);
		if (this.nextData != null && this.nextData.numInstances() >= PublicVariable.INSTANCE_SIZE) {
			addIntancesCache();
		}
	}

	protected void addFlowInstance(Instance instance) {
		this.flowInstances.add(instance);
	}

	protected Instance getSampleInstance() {
		return (Instance) this.allData.instance(0).copy();
	}

	public void shuffle() {
		Random r = new Random();
		r.setSeed(System.currentTimeMillis());
		this.allData.randomize(r);
	}
}
