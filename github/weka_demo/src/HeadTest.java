import weka.core.*;
import java.io.*;

public class HeadTest {
	
	private char [] header;
	private Instances headerInstances;
	public Instances getFileInstances( String fileName ) throws Exception
    {
		char []buf = new char[1024*1024*100];
		System.arraycopy(header, 0, buf, 0, header.length);
		FileReader frData = new FileReader(fileName);
		int ret = frData.read(buf,header.length,buf.length - header.length);
		CharArrayReader car = new CharArrayReader(buf);
        return new Instances(car);
    }
	
	public void getHeader(String headerFile) throws IOException
	{
		FileReader frData = new FileReader(headerFile);
		char[] buf = new char[102400];
		int ret = frData.read(buf);
		header = new char[ret];
		System.arraycopy(buf, 0, header, 0, ret);
		CharArrayReader car = new CharArrayReader(header);
		headerInstances = new Instances(car);
		
	}
	
	private final static String path_prefix = "C:\\shaw\\work\\weka_data\\"; 
	
	public static void main(String[]args) throws Exception
	{
		
		HeadTest headTest = new HeadTest();
		headTest.getHeader(path_prefix + "header.arff");
		Instances body = headTest.getFileInstances(path_prefix + "trainData.arff");
		System.out.println(body.toString());
	}
}
