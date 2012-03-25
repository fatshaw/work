public class TestEnsemble {
	private static Data data;
	private static EnsembleClassification ec;
	private static double time;
	private static double correct;

	/**
	 * start data stream thread to capture data input from fcapture
	 * 
	 * @author Shaw
	 * @Time 2012.3.17
	 * @throws Exception
	 */
	public static void p2pDetection() throws Exception {
		data.reset();

		long start = System.currentTimeMillis();
		ec.train();
		long end = System.currentTimeMillis();
		Util.debug("training time = " + (end - start));
		start = System.currentTimeMillis();
		ec.start();
		if (PublicVariable.DATASTREAM) {
			DataRetrieveThread drt = new DataRetrieveThread(data);
			drt.start();
		}
		end = System.currentTimeMillis();
		Util.debug("predict time = " + (end - start));
		ec.getResult();

		time += (end - start);
		correct += ec.getCorrectRate();

	}

	/**
	 * Batch Test for ensemble learning
	 * 
	 * @throws Exception
	 */
	public static void batchTest() throws Exception {
		int times = 10;
		int b = 0;
		int type = 3;
		int span = 2;

		for (int k = 0; k < 10; k++) {
			b = 0;
			for (int i = 0; i < type; i++) {
				for (int j = 0; j < times; j++) {
					if (b <= 1) {
						data = new Data("DataManager");
						ec = new EnsembleClassification();
					} else {
						data = new Data("GridDataManager");
						ec = new GridEnsembleClassification();
					}

					ec.init(data, b > 0);
					p2pDetection();
				}

				System.out.println("TYPE: "
						+ (b == 0 ? "BASE" : b == 1 ? "MMC" : "Grid"));
				System.out.println("average time = " + (time / times));
				System.out.println("average correct = " + (correct / times));

				time = 0;
				correct = 0;

				b++;

			}

			PublicVariable.V += span;

		}
	}

	/**
	 * Single Test for ensemble Learning
	 * 
	 * @throws Exception
	 */
	public static void singleTest() throws Exception {
		int b = 1;// PublicVariable.TYPE;
		for (int i = 0; i < 1; ++i) {
			if (b <= 1) {
				data = new Data("DataManager");
				ec = new EnsembleClassification();
			} else {
				data = new Data("GridDataManager");
				ec = new GridEnsembleClassification();
			}

			ec.init(data, b > 0);
			p2pDetection();

			Util.info("TYPE: " + (b == 0 ? "BASE" : b == 1 ? "MMC" : "Grid"));
			Util.info("average time = " + (time));
			Util.info("average correct = " + (correct));

			time = 0;
			correct = 0;
			b++;
		}
	}

	public static void main(String[] args) throws Exception {
		// read parameters from command line in scripts
		if (args.length >= 5) {
			PublicVariable.PATH_PREFIX = args[0];
			PublicVariable.DATASTREAM = Boolean.valueOf(args[1]);
			PublicVariable.INTERVAL = Integer.valueOf(args[2]) * 1000;
			PublicVariable.setLogLevel(Integer.valueOf(args[3]));
			PublicVariable.TYPE = Integer.valueOf(args[4]);
			PublicVariable.CHUNKSIZE = Integer.valueOf(args[5]);
		}
		// batchTest();
		singleTest();
	}
}
