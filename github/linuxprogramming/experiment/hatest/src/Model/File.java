package Model;

import common.Config;

public class File implements Comparable{

	/**
	 * @uml.property  name="path"
	 */
	private String path;

	/**
	 * Getter of the property <tt>path</tt>
	 * @return  Returns the path.
	 * @uml.property  name="path"
	 */
	public String getPath() {
		return path;
	}

	/**
	 * Setter of the property <tt>path</tt>
	 * @param path  The path to set.
	 * @uml.property  name="path"
	 */
	public void setPath(String path) {
		this.path = path;
	}

	/**
	 * @uml.property  name="filename"
	 */
	private String filename;

	/**
	 * Getter of the property <tt>filename</tt>
	 * @return  Returns the filename.
	 * @uml.property  name="filename"
	 */
	public String getFilename() {
		return filename;
	}

	/**
	 * Setter of the property <tt>filename</tt>
	 * @param filename  The filename to set.
	 * @uml.property  name="filename"
	 */
	public void setFilename(String filename) {
		this.filename = filename;
	}

	/**
	 * @uml.property  name="ownerId"
	 */
	private int ownerId;

	/**
	 * Getter of the property <tt>ownerId</tt>
	 * @return  Returns the ownerId.
	 * @uml.property  name="ownerId"
	 */
	public int getOwnerId() {
		return ownerId;
	}

	/**
	 * Setter of the property <tt>ownerId</tt>
	 * @param ownerId  The ownerId to set.
	 * @uml.property  name="ownerId"
	 */
	public void setOwnerId(int ownerId) {
		this.ownerId = ownerId;
	}

	/**
	 * @uml.property  name="permission"
	 */
	private int permission;

	/**
	 * Getter of the property <tt>permission</tt>
	 * @return  Returns the permission.
	 * @uml.property  name="permission"
	 */
	public int getPermission() {
		return permission;
	}

	/**
	 * Setter of the property <tt>permission</tt>
	 * @param permission  The permission to set.
	 * @uml.property  name="permission"
	 */
	public void setPermission(int permission) {
		this.permission = permission;
	}

	@Override
	public int compareTo(Object o) {
		// TODO Auto-generated method stub
		File f = (File)o;
		if(f.filename == this.filename && f.path == this.path)
			return 0;
		else return 1;
	}
}
