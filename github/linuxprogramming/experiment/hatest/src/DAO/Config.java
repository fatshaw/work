package DAO;

import Model.File;

public class Config {
	public static String username = "root";
	public static String password = "root";
	public static String hostname = "localhost";
	public static String port = "3306";
	public static String dbName = "hadoop";
	/**
	 * @uml.property  name="file"
	 * @uml.associationEnd  inverse="config:Model.File"
	 */
	private File file;
	/**
	 * Getter of the property <tt>file</tt>
	 * @return  Returns the file.
	 * @uml.property  name="file"
	 */
	public File getFile() {
		return file;
	}
	/**
	 * Setter of the property <tt>file</tt>
	 * @param file  The file to set.
	 * @uml.property  name="file"
	 */
	public void setFile(File file) {
		this.file = file;
	}
}
