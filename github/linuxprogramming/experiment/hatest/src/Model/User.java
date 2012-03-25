package Model;
import java.util.*;
public class User implements Comparable{

	/** 
	 * @uml.property name="userId"
	 */
	private int userId;
	
	/** 
	 * @uml.property  name="userId"
	 */
	public void setUserId(int userId){
		this.userId = userId;
	}
	
	/** 
	 * @uml.property  name="userId"
	 */
	public int getUserId(){
		return userId;
	}
	
	/**
	 * @uml.property  name="username"
	 */
	private String username;

	/**
	 * Getter of the property <tt>username</tt>
	 * @return  Returns the username.
	 * @uml.property  name="username"
	 */
	public String getUsername() {
		return username;
	}

	/**
	 * Setter of the property <tt>username</tt>
	 * @param username  The username to set.
	 * @uml.property  name="username"
	 */
	public void setUsername(String username) {
		this.username = username;
	}

	/**
	 * @uml.property  name="password"
	 */
	private String password;

	/**
	 * Getter of the property <tt>password</tt>
	 * @return  Returns the password.
	 * @uml.property  name="password"
	 */
	public String getPassword() {
		return password;
	}

	/**
	 * Setter of the property <tt>password</tt>
	 * @param password  The password to set.
	 * @uml.property  name="password"
	 */
	public void setPassword(String password) {
		this.password = password;
	}

	/**
	 * @uml.property  name="key"
	 */
	private String key;

	/**
	 * Getter of the property <tt>key</tt>
	 * @return  Returns the key.
	 * @uml.property  name="key"
	 */
	public String getKey() {
		return key;
	}

	/**
	 * Setter of the property <tt>key</tt>
	 * @param key  The key to set.
	 * @uml.property  name="key"
	 */
	public void setKey(String key) {
		this.key = key;
	}
	
	public User(String username,String password){
		this.username = username;
		this.password = password;
	}

	/**
	 * @uml.property  name="files"
	 */
	private Set<File> files;

	/**
	 * Getter of the property <tt>files</tt>
	 * @return  Returns the files.
	 * @uml.property  name="files"
	 */
	public Set<File> getFiles() {
		return files;
	}

	/**
	 * Setter of the property <tt>files</tt>
	 * @param files  The files to set.
	 * @uml.property  name="files"
	 */
	public void addFile(File files) {
		this.files.add(files);
	}

	@Override
	public int compareTo(Object o) {
		// TODO Auto-generated method stub
		User u = (User)o;
		if(u.userId == this.userId)return 0;
		else if(u.userId > this.userId)return 1;
		else return -1;
	}

}
