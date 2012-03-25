import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.List;

public class Fifo {
	private String fifoName;
	private RandomAccessFile raf;
	private PrintWriter pw;
	private BufferedInputStream bis;
	private static final int STREAM_LENGTH = 1024 * 1024 * 10;
	private byte[] bytes;
	private File fifoFile;

	/**
	 * Create a fifo file on linux
	 * 
	 * @param fifoName
	 *            name of fifo pipe
	 * @throws IOException
	 */
	public Fifo(String fifoName) throws IOException {
		this.fifoFile = new File(fifoName);
		if (!fifoFile.exists()) {
			Runtime.getRuntime().exec("mkfifo " + fifoName);
		}

		this.raf = new RandomAccessFile(fifoFile, "rw");
		this.fifoName = fifoName;
		this.pw = new PrintWriter(new BufferedOutputStream(new FileOutputStream(this.fifoName)));
		this.bis = new BufferedInputStream(new FileInputStream(this.fifoName));
		this.bytes = new byte[STREAM_LENGTH];
	}

	public void write(String s) {
		try {
			if (this.bis.available() + s.length() >= 65000) {
				Util.error("fifo full");
				return;
			}
			this.pw.println(s);
			this.pw.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public String read() throws IOException {
		int offset = 0;
		while (true) {
			int avaiable = bis.available();
			if (avaiable > 0) {
				bis.read(bytes, offset, avaiable);
				offset += avaiable;
			} else {
				break;
			}
		}
		if (offset == 0) {
			return "";
		} else {
			return new String(bytes, 0, offset);
		}
	}

	public void close() throws IOException {
		if (raf != null) {
			raf.close();
		}
		if (bis != null) {
			bis.close();
		}
		if (pw != null) {
			pw.close();
		}
		if (fifoFile != null) {
			fifoFile.delete();
		}
	}
}

class WriteTest {
	public static void main(String[] args) throws IOException, InterruptedException {
		Fifo fifo = new Fifo(PublicVariable.PATH_PREFIX + PublicVariable.FIFO_PATH);
		String line;
		while (true) {
			BufferedReader br = new BufferedReader(new FileReader("./Fifo.java"));
			while ((line = br.readLine()) != null) {
				fifo.write(line);
			}
			br.close();
			Thread.sleep(2000);
		}
	}
}

class ReadTest {
	public static void main(String[] args) throws IOException, InterruptedException {
		Fifo fifo = new Fifo("/root/share/data/fifo");
		while (true) {
			String s = fifo.read();
			if (s != "") {
				System.out.println("s=" + s);
			} else {
				Thread.sleep(1000);
			}
		}
	}

	public static void processData(String s) {
		String[] lines = s.split("\n");
		List<Flow> flowList = new ArrayList<Flow>();
		for (String line : lines) {
			String[] words = line.split(",");
			Flow f = new Flow();
			f.setFlowAddr(words[0]);
			f.setCategory(words[1]);
			flowList.add(f);
		}

		for (Flow f : flowList) {
			System.out.println("addr = " + f.getFlowAddr() + ", category = " + f.getCategory());
		}
	}
}

class Flow {
	private String flowAddr;
	private String category;

	public String getFlowAddr() {
		return flowAddr;
	}

	public void setFlowAddr(String flowAddr) {
		this.flowAddr = flowAddr;
	}

	public String getCategory() {
		return category;
	}

	public void setCategory(String category) {
		this.category = category;
	}

}