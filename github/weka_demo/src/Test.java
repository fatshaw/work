import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import weka.classifiers.Evaluation;
import weka.classifiers.trees.J48;

import weka.core.Instance;
import weka.core.Instances;
import weka.core.Utils;
import weka.filters.Filter;

import weka.filters.unsupervised.attribute.Normalize;
import weka.filters.unsupervised.instance.Resample;

/**
 * @author Administrator
 * @edit:ÏÂÎç12:01:37
 */
public class Test {

	Instances m_instances = null;

	Instances m_testIns = null;

	Instances m_trainIns = null;

	Instances m_testLabeledIns = null;

	Random r = new Random(System.currentTimeMillis());

	private static final String path_prefix = "E:\\work\\eclipse\\weka_test\\test_for_bot\\";

	// private static final String path_prefix = "C:\\shaw\\work\\weka_data\\";

	public Instances getFileInstances(String fileName) throws Exception {
		FileReader frData = new FileReader(fileName);
		return m_instances = new Instances(frData);
	}

	public void writeToArffFile(String newFilePath, Instances ins)
			throws IOException {
		BufferedWriter writer = new BufferedWriter(new FileWriter(newFilePath));
		writer.write(ins.toString());
		writer.flush();
		writer.close();
	}

	public void FilterRemovePercentageTest() throws Exception {
		resample1();
		// resample2();
	}

	public void resample1() throws Exception {
		long start = System.currentTimeMillis();
		int seed = r.nextInt(100) + 1;

		Resample removePercentage = new Resample();
		String[] options = Utils.splitOptions("-Z 50 -S " + seed
				+ " -no-replacement");

		removePercentage.setOptions(options);
		removePercentage.setInputFormat(m_instances);
		m_trainIns = Filter.useFilter(m_instances, removePercentage);
		m_trainIns.setClassIndex(m_trainIns.numAttributes() - 1);
		writeToArffFile(path_prefix + "trainData.arff", m_trainIns);

		seed = r.nextInt(100) + 1;
		removePercentage = new Resample();
		options = Utils.splitOptions("-Z 80 -S " + seed + " -no-replacement");
		removePercentage.setOptions(options);
		removePercentage.setInputFormat(m_instances);
		m_testLabeledIns = Filter.useFilter(m_instances, removePercentage);
		m_testLabeledIns.setClassIndex(m_testLabeledIns.numAttributes() - 1);
		writeToArffFile(path_prefix + "testLabeledData.arff", m_testLabeledIns);

		m_testIns = new Instances(m_testLabeledIns);
		m_testIns.setClassIndex(m_testIns.numAttributes() - 1);
		/*
		 * for(int i = 0 ;i < m_testIns.numInstances();i++) {
		 * m_testIns.instance(
		 * i).setClassValue(m_testIns.attribute(m_testIns.numAttributes
		 * ()-1).numValues()-1); }
		 */
		writeToArffFile(path_prefix + "testData.arff", m_testIns);

		long end = System.currentTimeMillis();
		System.out.println("time usage = " + (end - start));
	}

	public void resample2() throws IOException {
		long start = System.currentTimeMillis();
		m_instances.randomize(r);
		m_trainIns = new Instances(m_instances);
		m_trainIns.delete();
		m_testIns = new Instances(m_trainIns);
		m_testLabeledIns = new Instances(m_trainIns);
		int per = m_instances.numInstances() * 50 / 100;
		int i = 0;
		for (; i < per; i++) {
			m_trainIns.add(m_instances.instance(i));
		}
		for (; i < m_instances.numInstances(); ++i) {
			m_testIns.add(m_instances.instance(i));
			m_testLabeledIns.add(m_instances.instance(i));
		}

		m_testIns.setClassIndex(m_testIns.numAttributes() - 1);
		m_trainIns.setClassIndex(m_trainIns.numAttributes() - 1);
		m_testLabeledIns.setClassIndex(m_testLabeledIns.numAttributes() - 1);
		writeToArffFile(path_prefix + "trainData.arff", m_trainIns);
		writeToArffFile(path_prefix + "testLabeledData.arff", m_testLabeledIns);
		/*
		 * for(int i = 0 ;i < m_testIns.numInstances();i++) {
		 * m_testIns.instance(
		 * i).setClassValue(m_testIns.attribute(m_testIns.numAttributes
		 * ()-1).numValues()-1); }
		 */
		writeToArffFile(path_prefix + "testData.arff", m_testIns);

		long end = System.currentTimeMillis();
		System.out.println("time usage = " + (end - start));
	}

	public void J48Test() throws Exception {

		System.out.println(" **************J48********** ");
		long start = 0, end = 0;
		J48 j48 = new J48();

		start = System.currentTimeMillis();
		j48.buildClassifier(m_trainIns);
		end = System.currentTimeMillis();
		System.out.println("training time usage = " + (end - start));

		start = System.currentTimeMillis();
		for (int i = 0; i < this.m_testIns.numInstances(); ++i) {
			double category = j48.classifyInstance(m_testIns.instance(i));
			m_testIns.instance(i).setClassValue(category);
		}

		double correctNum = 0;
		for (int i = 0; i < m_testIns.numInstances(); ++i) {
			if (m_testIns.instance(i).classValue() == m_testLabeledIns
					.instance(i).classValue()) {
				correctNum++;
			}
		}
		end = System.currentTimeMillis();
		System.out.println("classify time usage = " + (end - start));
		System.out.println("instance num = " + m_testIns.numInstances()
				+ "my correct rate =" + correctNum / m_testIns.numInstances());

		writeToArffFile(path_prefix + "labeled.arff", m_testIns);
		m_testIns.delete();
		writeToArffFile(path_prefix + "header.arff", m_testIns);

		start = System.currentTimeMillis();
		Evaluation eval = new Evaluation(m_trainIns);
		eval.evaluateModel(j48, m_testLabeledIns);
		end = System.currentTimeMillis();
		System.out.println("time usage = " + (end - start));
		System.out.println(eval.toSummaryString());
	}

	public void loadData() throws Exception {
		m_trainIns = getFileInstances(path_prefix + "trainData.arff");
		m_testIns = getFileInstances(path_prefix + "testData.arff");
		m_testLabeledIns = getFileInstances(path_prefix
				+ "testLabeledData.arff");

		m_trainIns.setClassIndex(m_trainIns.numAttributes() - 1);
		m_testIns.setClassIndex(m_testIns.numAttributes() - 1);
		m_testLabeledIns.setClassIndex(m_testLabeledIns.numAttributes() - 1);
	}

	private Instances normalize(Instances instances) throws Exception {
		Normalize n = new Normalize();
		n.setInputFormat(instances);
		instances = Filter.useFilter(instances, n);
		return instances;
	}

	public void normalize() throws Exception {
		m_instances.setClassIndex(m_instances.numAttributes() - 1);
		m_instances = normalize(m_instances);
		writeToArffFile(path_prefix + "filtered_data.arff", m_instances);
	}

	public void gridMapping() {
		Instances instances = m_instances;
		String coordinator = null;
		Map<String, Instance> map = new HashMap<String, Instance>();
		int len = 10;
		for (int i = 0; i < instances.numInstances(); ++i) {
			coordinator = "";
			Instance instance = instances.instance(i);
			for (int j = 0; j < instance.numAttributes() - 1; ++j) {
				int index = (int) (instance.value(j) * len);
				coordinator += index;
			}
			if (map.containsKey(coordinator)) {
				Instance mapInstance = map.get(coordinator);
				for (int j = 0; j < mapInstance.numAttributes() - 1; j++) {
					mapInstance.setValue(j, mapInstance.value(j)
							+ instance.value(j));
				}
			} else {
				Instance mapInstance = new Instance(instance);
				map.put(coordinator, mapInstance);
			}
		}
		Instance[] t = new Instance[map.values().size()];
		t = map.values().toArray(t);
		for (int i = 0; i < t.length; ++i) {
			System.out.println(t[i].toString());
		}
	}

	/*
	 * update on 2012.3.17 weka demo load data,data filter,normalize,output run
	 * classification algorithm on data and calculate correct rate.
	 */
	public static void main(String[] args) throws Exception {
		Test percentage = new Test();
		percentage.getFileInstances(path_prefix + "b.arff");

		percentage.gridMapping();
		percentage.normalize();

		percentage.getFileInstances(path_prefix + "filtered_data.arff");
		percentage.loadData();
		percentage.FilterRemovePercentageTest();
		percentage.J48Test();
	}

}