import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;
import java.lang.reflect.Constructor;

import weka.core.Instances;

class Data {
	public DataManager trainingData;
	public DataManager labeledData;
	public DataManager testData;
	public String dataManagerType;
	public String header;

	public Data() {
	}

	public Data(String dataManagerType) throws Exception {
		init(dataManagerType);
	}

	public void init(String dataManagerType) throws Exception {
		this.dataManagerType = dataManagerType;
		getHeader(new FileReader(PublicVariable.PATH_PREFIX + PublicVariable.HEADER_DATA));
		PublicVariable.header = new Instances(new FileReader(PublicVariable.PATH_PREFIX + PublicVariable.HEADER_DATA));
		PublicVariable.header.setClassIndex(PublicVariable.header.numAttributes() - 1);
		trainingData = initDataManager(this.dataManagerType, PublicVariable.TRAINING_DATA);
		// if data stream test just add head data,else add all test data
		if (PublicVariable.DATASTREAM == true) {
			testData = initDataManager(this.dataManagerType, PublicVariable.HEADER_DATA);
		} else {
			testData = initDataManager(this.dataManagerType, PublicVariable.TEST_DATA);
		}
		labeledData = initDataManager(this.dataManagerType, PublicVariable.LABELED_DATA);
		PublicVariable.sample = trainingData.getSampleInstance();
	}

	public DataManager initDataManager(String DataManagerType, String filename) throws Exception {
		return initDataManager(DataManagerType, new FileReader(PublicVariable.PATH_PREFIX + filename));
	}

	/**
	 * Create Data Manager with input data manager type
	 * 
	 * @param DataManagerType
	 *            Data Manager Type
	 * @param reader
	 *            Input File Reader
	 * @return Data Manager Instance
	 * @throws Exception
	 */
	public DataManager initDataManager(String DataManagerType, Reader reader) throws Exception {
		Class c = Class.forName(DataManagerType);
		Constructor dataManagerClass = c.getConstructor(Reader.class);
		return (DataManager) dataManagerClass.newInstance(reader);
	}

	public void getHeader(FileReader reader) throws IOException {
		char[] h = new char[1024000];
		reader.read(h);
		header = new String(h);
		h = null;
	}

	public void reset() {
		trainingData.reset();
		testData.reset();
	}

	public void inputData(String data) throws Exception {
		if (data == null || data == "") {
			return;
		}
		testData.addInstances(new StringReader(data));
		Util.info("remaining = " + testData.remaining());
	}
}

/**
 * Data RetrieveThread Read data from mmap <br/>
 * Control synchronization with File Lock
 * 
 * @author Administrator
 * @edit:ÏÂÎç04:07:32
 */
class DataRetrieveThread extends Thread {
	private Lock lock;
	private Mmap mmap;
	private Data data;

	public DataRetrieveThread(Data data) {
		this.lock = new Lock(PublicVariable.PATH_PREFIX + PublicVariable.LOCK_FILE, false);
		this.mmap = new Mmap(PublicVariable.PATH_PREFIX + PublicVariable.SHARE_FILE, false, false, PublicVariable.SHARE_SIZE);
		this.data = data;
	}

	@Override
	public void run() {
		long last = 0;
		long cur = 0;
		while (true) {
			getNetFlowData();
			try {
				last = System.currentTimeMillis();
				while (true) {
					cur = System.currentTimeMillis();
					if (cur - last >= PublicVariable.INTERVAL) {
						break;
					}
					Util.debug("thread is running");
					Thread.sleep(1000);
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	public int getNetFlowData() {
		if (lock.lock() < 0) {
			return -1;
		}
		String packet = mmap.read();
		mmap.writeZero();
		lock.unlock();
		if (packet.compareTo("") == 0) {
			return -1;
		}
		Util.debug("packet=" + packet);
		try {
			data.inputData(packet);
		} catch (Exception e) {
			return -1;
		}
		return 0;
	}
}
