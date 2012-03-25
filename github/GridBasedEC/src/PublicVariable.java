import weka.core.*;

public class PublicVariable {
	public static int CHUNKSIZE =1000;
	public static int TRAINING_CHUNCK =3000;
	public static int R = 3;
	public static int K = 3;
	public static int V = 3;
	
	public static int LEN =5000 ;
	
	public static int ALGORITHM_COUNT = 3;
	public static Instances header;
	
	public static String PATH_PREFIX = "E:\\work\\eclipse\\weka_test\\bot\\";
	//public final static String PATH_PREFIX = "C:\\shaw\\work\\weka_data\\";
	
	public final static String TRAINING_DATA = "trainData.arff";
	
	public final static String TEST_DATA = "testData.arff";
	
	public final static String LABELED_DATA = "testLabeledData.arff";
	
	public final static String HEADER_DATA = "header.arff";
		
	public final static String SHARE_FILE	=	"SHARE.FILE";
	
	public final static String LOCK_FILE 	= 	"LOCK.FILE";	
	
	public static String FIFO_PATH = "fifo";
	
	public final static long SHARE_SIZE	= 1024*1024*10;
	
	public final static long LOCK_START	=	0;
	
	public final static long LOCK_SIZE	= 	1;
	
	public final static int INSTANCE_SIZE=	1000;
	
	public static long INTERVAL = 1000;
	
	public static boolean DATASTREAM = false;
	
	public static Instance sample;

	public static int TRACE = 4;
	public static int DEBUG = 3;
	public static int INFO = 2;
	public static int ERROR = 1;
	public static int NO_LOG = 0;
	
	public static int DEBUG_LEVEL= PublicVariable.INFO;
	
	public static int TYPE = 2;
	
	public static void setLogLevel(int level)
	{
		DEBUG_LEVEL = level;
	}
	
	public static boolean ISP2P=true;
}
