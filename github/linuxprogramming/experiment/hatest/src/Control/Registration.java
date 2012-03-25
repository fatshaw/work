package Control;
import Model.*;
import common.Util;

import DAO.RegistrationDAO;

public class Registration {
	private String username;
	private String password;
	
	public boolean registe(String username,String password){
		RegistrationDAO rdao = new RegistrationDAO();
		this.username= username;
		this.password = password;
		User user = new User(username,password);
		if(!rdao.isRegisterd(username)){
			user.setKey(Util.generateKey());
			return rdao.Registe(user);
		}
		else{
			System.out.println("you have registed.");
			return false;
		}
		
	}
}
