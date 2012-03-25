package Control;
import Model.*;
import java.util.*;
public class FileManagement {

	/**
	 * @uml.property  name="users"
	 */
	private Set<User> users;

	/**
	 * Getter of the property <tt>users</tt>
	 * @return  Returns the users.
	 * @uml.property  name="users"
	 */
	public Set<User> getUsers() {
		return users;
	}

	/**
	 * Setter of the property <tt>users</tt>
	 * @param users  The users to set.
	 * @uml.property  name="users"
	 */
	public void setUsers(Set<User> users) {
		this.users = users;
	}
	
	public void addUser(User user){
		this.users.add(user);
	}
	
	public 
	
}
