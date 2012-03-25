import java.io.*;
import java.lang.Thread;
import java.lang.StringBuffer;
class fifo
{
	public static void main(String[]args) throws Exception
	{
		FileInputStream fis = new FileInputStream("./fifo");
		while(true)
		{
			StringBuffer sb = new StringBuffer();
			while(true){
				char t =(char) fis.read();
				if(t==0){
					System.out.println(sb.toString());
					break;
				}
				else{
					sb.append(t);
				}
			}
			Thread.sleep(1000);
		}
	}
}

