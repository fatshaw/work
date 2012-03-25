import java.util.ArrayList;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import weka.classifiers.Classifier;
import weka.core.Instance;
import weka.core.Instances;

/**
 * Ensemble classifier,each containing v sub-classifiers.
 * 
 * @author Administrator
 * @edit:ÏÂÎç04:21:14
 */
public class Classification {
	protected Classifier[] classifiers;
	protected int v;
	private double correctNum;
	private double testedNum;

	public Classification() {
		this.v = PublicVariable.V;
		classifiers = new Classifier[v];
	}

	public Classification(Classifier classifier, int v) throws Exception {
		this.v = v;
		classifiers = Classifier.makeCopies(classifier, this.v);
	}

	public void trainingData(Instances trainingInstances) throws Exception {
		Instances[] splitedInstances = Util.splitData(trainingInstances, this.v);
		for (int i = 0; i < splitedInstances.length; ++i) {
			classifiers[i].buildClassifier(splitedInstances[i]);
		}
	}

	public double classifyInstance(Instance testInstance) throws Exception {
		int[] voteTable = new int[PublicVariable.header.numClasses()];
		int category;
		for (int i = 0; i < v; i++) {
			testedNum++;
			category = (int) classifiers[i].classifyInstance(testInstance);
			voteTable[category]++;
		}

		int maxIndex = Util.getMaxVote(voteTable);
		correctNum += voteTable[maxIndex];
		return maxIndex;
	}

	public void updateClassification(Instances updateInstances, boolean isThread) throws Exception {
		ExecutorService executor = Executors.newFixedThreadPool(this.v);
		Instances[] splitedInstances = Util.splitData(updateInstances, this.v);
		ArrayList<Future<Integer>> results = new ArrayList<Future<Integer>>();
		for (int i = 0; i < this.v; ++i) {
			testedNum += splitedInstances[i].numInstances();
			if (isThread) {
				UpdateThread ut = new UpdateThread();
				ut.setClassifiers(classifiers);
				ut.setSplitedInstances(splitedInstances);
				ut.setTestInstanceIndex(i);
				results.add(executor.submit(ut));
			} else {
				correctNum += buildAndClassify(splitedInstances, i);
			}
		}
		if (isThread) {
			for (Future<Integer> fs : results) {
				correctNum += fs.get();
			}
		}
		executor.shutdown();
	}

	protected int buildAndClassify(Instances[] splitedInstances, int i) throws Exception {
		int correctNumTemp = 0;
		Instances trainingInstances;
		Instances testInstances;
		trainingInstances = new Instances(PublicVariable.header);
		for (int j = 0; j < this.v; ++j) {
			if (i == j) {
				continue;
			}
			Util.mergeInstances(trainingInstances, splitedInstances[j]);
		}
		classifiers[i].buildClassifier(trainingInstances);
		testInstances = splitedInstances[i];
		for (int k = 0; k < testInstances.numInstances(); ++k) {
			if (classifiers[i].classifyInstance(testInstances.instance(k)) == testInstances.instance(k).classValue()) {
				correctNumTemp++;
			}
		}
		return correctNumTemp;
	}

	public double getCorrectRate() {
		return correctNum / testedNum;
	}

	public void resetCorrectRate() {
		this.correctNum = 0;
		this.testedNum = 0;
	}

}

class UpdateThread extends Classification implements Callable<Integer> {
	private Instances[] splitedInstances;
	private int testInstanceIndex;

	public void setSplitedInstances(Instances[] splitedInstances) {
		this.splitedInstances = splitedInstances;
	}

	public void setTestInstanceIndex(int testInstanceIndex) {
		this.testInstanceIndex = testInstanceIndex;
	}

	public void setClassifiers(Classifier[] classifiers) {
		this.classifiers = classifiers;
	}

	@Override
	public Integer call() {
		int correctNumTemp = 0;
		try {
			correctNumTemp = buildAndClassify(splitedInstances, testInstanceIndex);
		} catch (Exception ex) {

		}
		return correctNumTemp;
	}
}