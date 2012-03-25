package DAO;
import java.sql.*;

import Model.User;
public class LoginDAO{
	public ResultSet getUser(String username,String password) throws SQLException{
		String str = "select count(*), * from user where username = \" "+ username + " \" " + " and password = \" "+password + "\"";
		ResultSet rs = DAO.query(str);
		return rs;
	}
	
	public ResultSet getFiles(int userid){
		String str = "select * from file where userid = \""+userid+"\"";
		ResultSet rs= DAO.query(str);
		return rs;
	}
}
