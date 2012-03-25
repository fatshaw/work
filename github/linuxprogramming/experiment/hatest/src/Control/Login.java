package Control;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Map;
import java.util.Set;

import DAO.LoginDAO;
import Model.*;
public class Login {
	
	private Map<Integer,User>users;

	public Set<File> login(String username,String password){
		LoginDAO ldao = new LoginDAO();
		ResultSet rs = null;
		ResultSet filers = null;
		try {
			rs = ldao.getUser(username, password);
			if(rs.getInt(0)>0){
				User user = new User(username,password);
				user.setUserId(rs.getInt("userid"));
				user.setKey(rs.getString("key"));
				filers = ldao.getFiles(user.getUserId());
				while(filers.next()){
					File f = new File();
					f.setFilename(filers.getString("filename"));
					f.setOwnerId(filers.getInt("ownerid"));
					f.setPath(filers.getString("path"));
					f.setPermission(filers.getInt("permission"));
					user.addFile(f);
				}
				users.put(user.getUserId(), user);
				return user.getFiles();
			}else{
				return null;
			}
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		finally{
			try {
				rs.close();
				filers.close();
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return null;
	}
}
