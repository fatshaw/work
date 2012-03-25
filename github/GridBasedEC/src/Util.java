import java.io.IOException;
import java.util.Random;

import weka.core.Instances;

/**
 * @author Administrator
 * @edit:обнГ03:46:13
 */
/**
 * @author Administrator
 * @edit:обнГ03:46:15
 */
/**
 * @author Administrator
 * @edit:обнГ03:47:52
 */
public class Util {

	private static Fifo fifo;

	static {
		try {
			if (isLinux()) {
				fifo = new Fifo(PublicVariable.PATH_PREFIX
						+ PublicVariable.FIFO_PATH);
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	/**
	 * shuffle the instance set
	 * 
	 * @param instance
	 *            instance set need shuffle
	 */
	public static void getRandPermutation(Instances instance) {
		Random r = new Random();
		r.setSeed(System.currentTimeMillis());
		int rnd = 0, n = instance.numInstances();
		for (int i = 0; i < n; ++i) {
			rnd = r.nextInt(n - i) + i;
			instance.swap(i, rnd);
		}
	}

	/**
	 * split instances set into v blocks
	 * 
	 * @param instances
	 *            whole set of instances needed to be splitted
	 * @param v
	 *            block number
	 * @return
	 */
	public static Instances[] splitData(Instances instances, int v) {
		getRandPermutation(instances);
		Instances[] splitedInstances = new Instances[v];
		int v_chunksize = instances.numInstances() / v; // here it will have
		// remainder caused by
		// exact division
		for (int i = 0; i < v; i++) {
			splitedInstances[i] = new Instances(PublicVariable.header);
			for (int j = 0; j < v_chunksize; j++) {
				splitedInstances[i]
						.add(instances.instance(v_chunksize * i + j));
			}
		}
		// we need think about remainder here.
		for (int i = 0; i < instances.numInstances() - v_chunksize * v; ++i) {
			splitedInstances[v - 1]
					.add(instances.instance(v_chunksize * v + i));
		}
		return splitedInstances;
	}

	/**
	 * 
	 * @param voteTable
	 *            input table
	 * @return max index of element in the table
	 */
	public static int getMaxVote(int[] voteTable) {
		int maxIndex = 0;
		for (int k = 0; k < voteTable.length; ++k) {
			if (voteTable[k] > voteTable[maxIndex]) {
				maxIndex = k;
			}
		}
		return maxIndex;
	}

	public static void clearVoteTable(int[] voteTable) {
		for (int i = 0; i < voteTable.length; ++i) {
			voteTable[i] = 0;
		}
	}

	public static boolean mergeInstances(Instances oldChunk, Instances newChunk) {
		if (newChunk == null || oldChunk == null) {
			return false;
		}
		for (int i = 0; i < newChunk.numInstances(); i++) {
			if (oldChunk.numInstances() <= 0) {
				break;
			}
			oldChunk.delete(0);
		}

		for (int i = 0; i < newChunk.numInstances(); ++i) {
			oldChunk.add(newChunk.instance(i));
		}
		return true;
	}

	public static void log(int level, Object info) {
		if (level <= PublicVariable.DEBUG_LEVEL) {
			System.out.println("function="
					+ Thread.currentThread().getStackTrace()[3].getMethodName()
					+ ",info=" + info);
		}
	}

	public static void info(Object info) {
		log(PublicVariable.INFO, info);
	}

	public static void debug(Object info) {
		log(PublicVariable.DEBUG, info);
	}

	public static void error(Object info) {
		log(PublicVariable.ERROR, info);
	}

	public static void trace(Object info) {
		log(PublicVariable.TRACE, info);
	}

	public static void fifoWrite(String o) {
		if (isLinux()) {
			fifo.write(o);
		}
	}

	public static String fifoRead() throws IOException {
		if (isLinux()) {
			return fifo.read();
		}
		return "";
	}

	/**
	 * 
	 * @return linux true;others false
	 */
	public static boolean isLinux() {
		if (PublicVariable.DATASTREAM == false) {
			return false;
		}
		String name = System.getProperty("os.name");
		if (name.indexOf("Linux") >= 0) {
			return true;
		}
		return false;
	}
}
