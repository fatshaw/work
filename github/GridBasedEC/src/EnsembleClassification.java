import weka.core.*;

import java.io.*;
import java.util.Random;

import weka.classifiers.*;
import weka.classifiers.bayes.BayesNet;
import weka.classifiers.rules.JRip;
import weka.classifiers.trees.J48;

public class EnsembleClassification {
	protected Classification [] classifiers;
	protected DataManager trainingData;
	protected DataManager labeledData;
	protected DataManager testData;
	protected Instances curChunkData;
	protected Random r;
	protected Classifier [] classifierAlgorithm;
	protected boolean isThread;
	protected double predictNum;
	protected double correctNum;
	
	public void init(Data data,boolean isThread) throws Exception
	{
		this.isThread=isThread;
		this.trainingData = data.trainingData;
		this.labeledData = data.labeledData;
		this.testData = data.testData;
	//	this.testData.shuffle();
	//	this.testData.save(PublicVariable.PATH_PREFIX+"shuffle");
		
		classifiers = new Classification[PublicVariable.K];
		classifierAlgorithm = new Classifier[PublicVariable.ALGORITHM_COUNT];
		classifierAlgorithm[0] = new JRip();
		classifierAlgorithm[1] = new BayesNet();
		classifierAlgorithm[2] = new J48();
		classifierAlgorithm[0] = new J48();

		r = new Random(System.currentTimeMillis()/1000);
	}

	
	public void train() throws Exception
	{
		curChunkData = trainingData.getChunkData(PublicVariable.R,PublicVariable.TRAINING_CHUNCK);
		classifiers[0] = new Classification(classifierAlgorithm[0],PublicVariable.V);
		classifiers[0].trainingData(curChunkData);
		for(int i = 1; i < PublicVariable.K;++i){
			if(Util.mergeInstances(curChunkData,trainingData.getChunkData(1,PublicVariable.TRAINING_CHUNCK)) == false)break;
			classifiers[i] = new Classification(classifierAlgorithm[0],PublicVariable.V);
			classifiers[i].trainingData(curChunkData);
		}
	}

	public void predict() throws Exception
	{
		while(true){
			int remaining=testData.remaining();
			if(remaining<PublicVariable.CHUNKSIZE &&PublicVariable.DATASTREAM==true)
			{
				if(PublicVariable.DATASTREAM==true){
					Util.info("sleep for data");
					Util.debug("remaining = "+testData.remaining());
					Util.debug("predictnum = "+predictNum + " correctNum="+correctNum +"ratio="+correctNum/predictNum);
					Thread.sleep(PublicVariable.INTERVAL);
					continue;
				}
			}
			for(int i =0; i <PublicVariable.CHUNKSIZE;++i){
				Util.debug("remaining = "+testData.remaining());
				Instance testInstance = testData.getNextInstance();
				if(testInstance == null)break;
				classify(testInstance);
				curChunkData.delete(0);
				curChunkData.add(testInstance);
			}
			
			if(testData.hasNext() || PublicVariable.DATASTREAM){
				Util.debug("hasnext="+testData.hasNext());
				updateClassification();
			}
			else{
				break;
			}
		}
	}

	public void classify(Instance testInstance) throws Exception{
		Util.trace("called");
		predictNum++;
		int category = 0;
		int maxIndex = 0;
		int [] voteTable = new int[PublicVariable.header.numClasses()];
		for(int j = 0 ; j < classifiers.length;++j){
			if(classifiers[j] == null)break;
			category = (int)classifiers[j].classifyInstance(testInstance);
			voteTable[category]++;
		}
		maxIndex = Util.getMaxVote(voteTable);
		if(maxIndex == testInstance.classValue()){
			correctNum++;
		}
		testInstance.setClassValue(maxIndex);
		
		if(this instanceof GridEnsembleClassification == false){
			String info;
			if(testInstance instanceof FlowInstance){
				FlowInstance flowInstance = (FlowInstance)testInstance;
				info =flowInstance.flow_addr+","+testInstance.classAttribute().value(maxIndex);
			}
			else
			{
				info =testInstance.classAttribute().value(maxIndex) ;
			}
			Util.debug(info);
			Util.fifoWrite(info);
		}
	}
	
	public void updateClassification() throws Exception
	{
		int ind = r.nextInt(PublicVariable.ALGORITHM_COUNT);
		Classification c = new Classification(classifierAlgorithm[0],PublicVariable.V);
		c.updateClassification(curChunkData,this.isThread);
		double minCorrectRate = c.getCorrectRate();
		int minIndex = -1;
		for(int i = 0 ;i < classifiers.length;++i)
		{			
			if(classifiers[i]==null){
				Util.debug("null update classification");
				minIndex = i;
				break;
			}
			if(classifiers[i].getCorrectRate() < minCorrectRate){
				minCorrectRate =classifiers[i].getCorrectRate();
				minIndex = i;
			}
			classifiers[i].resetCorrectRate();
		}
		Util.debug("remove bad classifier");
		if(minIndex != -1)
		{
			Util.debug("update classification");
			classifiers[minIndex] = c;
		}
	}
	
	public double getCorrectRate() throws IOException
	{
		return testData.calculateCorrectRate(labeledData);
		//return correctNum / predictNum;
	}
	
	
	public void getResult() throws IOException{
		Util.info("chunksize= "+ PublicVariable.CHUNKSIZE + ", r = " + PublicVariable.R + ", k = " +PublicVariable.K+ ", v = " + PublicVariable.V);
		Util.info("LEN = "+ PublicVariable.LEN + ", TRAINING CHUNKSIZE = " + PublicVariable.TRAINING_CHUNCK);
		//testData.save();
	}
	
	public void start() throws Exception{
		if(PublicVariable.DATASTREAM){
			Thread t = new Thread(new PredictThread());
			t.start();
		}
		else{
			predict();
		}
	}
	
	class PredictThread extends Thread{
		public void run()
		{
			try {
				predict();
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}

