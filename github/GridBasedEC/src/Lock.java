import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileLock;

/**
 * @author Administrator
 * @edit:ÏÂÎç04:03:49
 */
public class Lock {
	private FileOutputStream fos;
	private FileLock fl;
	private boolean isShared;
	private long lockStart;
	private long lockSize;

	public Lock(String lockFile, boolean isShared) {
		this(lockFile, isShared, PublicVariable.LOCK_START, PublicVariable.LOCK_SIZE);
	}

	/**
	 * Create a File Lock
	 * 
	 * @param lockFile
	 *            File used to create a file lock
	 * @param isShared
	 *            true=shared,false=private
	 * @param lockStart
	 *            start position of filelock
	 * @param lockSize
	 *            end position of filelock
	 */
	public Lock(String lockFile, boolean isShared, long lockStart, long lockSize) {
		try {
			this.fos = new FileOutputStream(lockFile);
			this.isShared = isShared;
			this.lockStart = lockStart;
			this.lockSize = lockSize;
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	/**
	 * Lock filelock
	 * 
	 * @return 0:success,-1:fails
	 */
	public int lock() {
		if (fl != null || fos == null) {
			return -1;
		}

		try {
			fl = fos.getChannel().lock(this.lockStart, this.lockSize, isShared);
		} catch (IOException ex) {
			return -1;
		}

		return 0;
	}

	/**
	 * unlock filelock
	 * 
	 * @return 0:success,-1:fails
	 */
	public int unlock() {
		if (fl == null) {
			return -1;
		}

		try {
			fl.release();
			fl = null;
		} catch (IOException ex) {
			return -1;
		}

		return 0;
	}
}
