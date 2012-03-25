package DAO;

import java.sql.ResultSet;
import java.sql.SQLException;
import Model.*;

public class RegistrationDAO {
	public boolean isRegisterd(String username){
		String str = "select count(*) from user where username = \""+username+"\"";
		ResultSet rs = DAO.query(str);
		try {
			if(rs.getInt(0)>0)return true;
			else return false;
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		finally{
			try {
				rs.close();
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return false;
	}
	
	public boolean Registe(User user){
		String str= "insert into user values(\""+user.getUsername()+"\",\""+user.getPassword()+"\",\""+user.getKey()+"\")";
		if(DAO.update(str) >  0)return true;
		else return false;
	}
}
