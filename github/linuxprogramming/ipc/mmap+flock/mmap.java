import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.BufferUnderflowException;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;


public class mmap {

	/**
	 * @param args
	 * @throws IOException 
	 * @throws FileNotFoundException 
	 * @throws IOException 
	 * @throws FileNotFoundException 
	 * @throws InterruptedException 
	 */
	public static void main(String[] args) throws FileNotFoundException, IOException, InterruptedException {
		// TODO Auto-generated method stub
		int length = 1024*1024*10;
		MappedByteBuffer mbb = new FileInputStream("./a.txt").getChannel().map(FileChannel.MapMode.READ_ONLY,0,length);
		FileOutputStream fos = new FileOutputStream("./b.txt");
		String s="";
		
		for(;;){
			FileLock fl = fos.getChannel().lock(0, 1, false);
			for(;;){
				try{
					char t = (char)mbb.get();
					if(t==0){
						System.out.println(s);
						mbb.rewind();
						s="";
						break;
					}
					else{
						s+=t;
					}
				}
				catch(BufferUnderflowException be)
				{
				}
			}
			fl.release();
			Thread.sleep(1000);
		}
	}
}

