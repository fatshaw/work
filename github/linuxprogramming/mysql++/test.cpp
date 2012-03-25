#include <mysql++/mysql++.h>
using namespace mysqlpp;
using namespace std;
int main()
{
	Connection conn(false);
	if(conn.connect("mysql","localhost","root","123456"))
	{
		Query query =conn.query("select User from user");
		StoreQueryResult res = query.store();
		if(res){
			for(size_t i = 0; i< res.num_rows();++i){
				cout<<res[i]["User"];
			}
		}else
		{
			cerr<<"failed to get stock table: "<<query.error()<<endl;
		}
	}
	
	return 0;
}
