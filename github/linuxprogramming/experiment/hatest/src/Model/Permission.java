package Model;

public class Permission {
	
	/** 
	 * @uml.property name="permission"
	 */
	private int permission;//read write and share.
	
	public Permission(){
		permission = 0;
	}
	
	public boolean read(){
		return false;
	}
	
	public boolean write(){
		return false;
	}

	//add here
}
