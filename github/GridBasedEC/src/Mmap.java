import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;

public class Mmap {

	private MappedByteBuffer mbb;
	private StringBuffer sb;

	/**
	 * 
	 * @param fileName
	 *            used for mmap
	 * @param isPrivate
	 *            private=true,other=false
	 * @param readOnly
	 *            read only = true,rw=false
	 * @param length
	 *            length of file for mmap
	 */
	public Mmap(String fileName, boolean isPrivate, boolean readOnly, long length) {
		try {
			sb = new StringBuffer((int) length);
			mbb = new RandomAccessFile(fileName, "rw").getChannel().map
					(isPrivate ? FileChannel.MapMode.PRIVATE : readOnly ? FileChannel.MapMode.READ_ONLY : FileChannel.MapMode.READ_WRITE, 0, length);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * read String from mmap util end
	 * 
	 * @return String read from mmap
	 */
	public String read() {
		if (mbb == null) {
			return "";
		}
		char t;
		mbb.clear();
		for (;;) {
			t = (char) mbb.get();
			if (t == 0) {
				break;
			}
			sb.append(t);

		}
		String ret;
		if (sb.length() <= 0) {
			ret = "";
		} else {
			ret = sb.toString();
		}
		sb.delete(0, sb.length());
		return ret;
	}

	/**
	 * write string into mmap
	 * 
	 * @param str
	 *            string written into mmap
	 * @return
	 */
	public int write(String str) {
		if (mbb == null) {
			return -1;
		}
		mbb.clear();
		mbb.put(str.getBytes());
		return 0;
	}

	public int writeZero() {
		if (mbb == null) {
			return -1;
		}
		mbb.clear();
		mbb.put((byte) (0));
		return 0;
	}

}
