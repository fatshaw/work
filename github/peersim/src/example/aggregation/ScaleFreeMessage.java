package example.aggregation;

enum MsgType{StartUp, LookUp,End}

public abstract class ScaleFreeMessage {
	private MsgType type;
	private Object msg;
	
	public ScaleFreeMessage(MsgType type,Object msg){
		this.type = type;
		this.msg = msg;
	}
}

class ScaleFreeStartMsg extends ScaleFreeMessage
{
	public ScaleFreeStartMsg()
	{
		super(MsgType.StartUp, "ScaleFreeStartMsg");
	}
}

class ScaleFreeLookUpMsg extends ScaleFreeMessage
{
	public ScaleFreeLookUpMsg()
	{
		super(MsgType.LookUp, "ScaleFreeLookUpMsg");
	}
}

class ScaleFreeEndMsg extends ScaleFreeMessage
{
	public ScaleFreeEndMsg()
	{
		super(MsgType.End, "ScaleFreeEndMsg");
	}
}
