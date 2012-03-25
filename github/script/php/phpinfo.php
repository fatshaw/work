<?php
define("NEWLINE","<br></br>");
define("LINK","href");
define("ENCODING","UTF-8");
define("COOKIE","c:\\wamp\\www\\cookie");

set_time_limit(1000000);
$conn = mysql_connect("localhost","root");
mysql_select_db("webinfo",$conn) or die(mysql_error());
mysql_query("set character set 'utf8'");//读库
mysql_query("set names 'utf8'") or die(mysql_error());//写库


//duanziwang();
//ishuo();
//yulu();
//msn();
//mtw51();
//freeheart();
//scrapeBlog();


mysql_close($conn);

function login()
{
	$user = "fatshaw@qq.com";
	$pass = "1987714";
	$url ='http://login.sina.com.cn/sso/login.php?client=ssologin.js(v1.3.7)';// 'http://reg.163.com/logins.jsp?type=1&url=http://entry.mail.163.com/coremail/fcg/ntesdoor2?lightweight%3D1%26verifycookie%3D1%26language%3D-1%26style%3D-1';
	$ch = curl_init($url);//创建一个用于存放cookie信息的临时文件
	$cookie  = "c:\\wamp\\www\\cookie";//tempnam('.','~');
	$referer_login  = 'http://login.sina.com.cn';
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);//返回结果存放在变量中,而不是默认的直接输出
	curl_setopt($ch, CURLOPT_HEADER, true);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 120);
	curl_setopt($ch, CURLOPT_POST, true); 
	curl_setopt($ch, CURLOPT_REFERER, $referer_login); 
	$fields_post = array('username'=> $user,'password'=> $pass,);
	$headers_login = array('User-Agent'=> 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; InfoPath.2; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; MS-RTC LM 8)',    'Referer'=> 'http://login.sina.com.cn');
	$fields_string = '';
	foreach($fields_post as $key => $value)
	{
		$fields_string .= $key . '=' . $value . '&';
	} 
	$fields_string = rtrim($fields_string , '&'); 
	curl_setopt($ch, CURLOPT_COOKIESESSION, true);//关闭连接时，将服务器端返回的cookie保存在以下文件中
	curl_setopt($ch, CURLOPT_COOKIEJAR, COOKIE);
	curl_setopt($ch, CURLOPT_HTTPHEADER, $headers_login);
	curl_setopt($ch, CURLOPT_POST, count($fields));
	curl_setopt($ch, CURLOPT_POSTFIELDS, $fields_string); 
	$result= curl_exec($ch);
	curl_close($ch);  
}

function get($url){
	
	$ch = curl_init($url); 
	$headers = array('User-Agent' => 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; InfoPath.2; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; MS-RTC LM 8)'); 
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	//curl_setopt($ch, CURLOPT_HEADER, true);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 120);
	curl_setopt($ch, CURLOPT_POST, true);
	curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
	curl_setopt($ch, CURLOPT_COOKIEFILE, COOKIE);
	curl_setopt($ch, CURLOPT_COOKIEJAR, COOKIE);
	$result = curl_exec($ch);
	if(curl_error($ch)){
		return false;
	}
	curl_close($ch);
	return $result;
}

function scrapeBlog()
{
	$out = fopen(".\\out","w+");
	login();
	$urlarray = array("http://t.sina.com.cn/profile.php?uid=1640494672&page=1","http://t.sina.com.cn/profile.php?uid=1236380405&page=1","http://t.sina.com.cn/profile.php?uid=1675718025&page=1","http://t.sina.com.cn/profile.php?uid=1567852087&page=1");
	$count = count($urlarray);
	for($i = 0; $i < $count;$i++){
			$resource = fopen(".\\blog".$i,"w+");
			$newurl = $urlarray[$i];
			$str = get($newurl);
			$pos = 0;
			$pagepos = 0;
			$temp = false;
			$visitedUrl = array($newurl);
			$needSearch = true;
			while(true){
				if($needSearch == true){
					$temp = strpos($str,"<p class=\"sms\"",$pos);
				}
				//fwrite($out,"temp = ".$temp."\n");
				if($temp != false){
					$pos = $temp;		
				}else{
					$pagepos = strpos($str,"class=\"btn_num\"",$pos);	
					if($pagepos == false){
						fwrite($out,"no page over\n");
						break;
					}
					$startpos  = $pagepos;
					while(true){
							$startpos = strpos($str,LINK,$startpos);
							if($startpos == false){
								fwrite($out, "page is over\n");
								break;
							}
							$startpos += strlen(LINK) +1;
							$newurl = getUrl($str,$startpos);
							//fwrite($out,"newurl is ".$newurl."\n");
							if(in_array($newurl,$visitedUrl) == false){
								fwrite($out,"newurl is ".$newurl."\n");
								$visitedUrl[]=$newurl;
								$tempstr= get($newurl);
								if($tempstr == false){
									//fwrite($out,"get newurl is false, continue to find new url \n");
									continue;
								}else{
									$str = $tempstr;
									//fwrite($out,"get new contente \n");
									$pos  = 0;
									$needSearch =true;
									break;
								}
							}
							else
							{
								//fwrite($out,"continue to find newurl \n");
								continue;
							}
					}
					//fwrite($out,"continue\n");
					continue;
				}
				//fwrite($out,"pos = $pos \n");
				$posNext= strpos($str, "<p class=\"sms\"",$pos + 1);
				//fwrite($out,"posNext = $posNext \n");
				$posSource = strpos($str,"<p class=\"source\"",$pos);
				//fwrite($out,"posSource = $posSource \n");
				$endpos = strpos($str,"</p>",$pos);
				//fwrite($out,"endpos = $endpos \n");
				if($endpos == false){
					var_dump("can not find /p");
					break;
				}
				$text = removeHtmlMark(trim(substr($str,$pos,$endpos - $pos)));
				//fwrite($out,"text =".$text."\n");
				$needSource = false;
				if($posSource != false){
						if($posNext != false){
							if($posSource < $posNext){
								//fwrite($out,"posSource < $posNext \n");
								$needSource = true;
							}
						}
						else if($posNext == false){
							$needSource ==true;
						}
				}
				if($needSource == true){
					$sourceEndpos = strpos($str,"<span class",$posSource);
					if($sourceEndpos ==false){
						//var_dump("sourceEndpos can not be found");
						break;
					}
					$text .= trim(removeHtmlMark(substr($str,$posSource,$sourceEndpos - $posSource)));
					//fwrite($out,"temptext =".$temptext."\n");
				}
				if($posNext != false){
					$pos = $posNext;
				}else{
					$needSearch = false;
					$temp  = false;
				}
				store($resource,$newurl,"sinablog",$text);
			}
	}
}



//跳转
/*$url="http://tg6a52.mail.163.com/js3/main.jsp?sid=DBEaxAtYCYagfiJBjHYYfEDtgEYdUIuV";//'http://entry.mail.163.com/coremail/fcg/ntesdoor2?lightweight=1&verifycookie=1&language=-1&style=-1&username=fatshaw@163.com'; 
$ch = curl_init($url); 
$headers = array('User-Agent' => 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; InfoPath.2; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; MS-RTC LM 8)'); 
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_HEADER, true);
curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 120);
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
//将之前保存的cookie信息，一起发送到服务器端
curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie);
curl_setopt($ch, CURLOPT_COOKIEJAR, $cookie);
$result = curl_exec($ch);
curl_close($ch);
//取得sid
echo $result;*/
//preg_match('/sid=[^\"].*/', $result, $location);
//$sid = substr($location[0], 4, -1);
//file_put_contents('c:\\wamp\\www\\result.txt', $sid); 
 //通讯录地址
/*$url='http://g4a30.mail.163.com/jy3/address/addrlist.jsp?sid='.$sid.'&gid=all';
$ch = curl_init($url); 
$headers = array(    'User-Agent'=> 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; InfoPath.2; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; MS-RTC LM 8)');
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_HEADER, true);
curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 120);
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie);
curl_setopt($ch, CURLOPT_COOKIEJAR, $cookie);
$result = curl_exec($ch);
curl_close($ch);//
file_put_contents('.\\result.txt', $result);
unlink($cookie); //开始抓取内容
preg_match_all('/<td class="Ibx_Td_addrName"><a[^>]*>(.*?)<\/a><\/td><td class="Ibx_Td_addrEmail"><a[^>]*>(.*?)<\/a><\/td>/i', $result,$infos,PREG_SET_ORDER);
//1：姓名2：邮箱
print_r($infos);*/
 /* * 下面就可以为所欲为了- - */
function duanziwang()
{
	$out = fopen(".\\out","w+");
	$prefix = "http://duanziwang.com/";
	$url = "http://duanziwang.com";
	$str = file_get_contents($url);
	$str = mb_convert_encoding($str,ENCODING,mb_detect_encoding($str));
	if($str == false){
		var_dump("file_get_contents error");
		exit();
	}
	$keyword1 = "互联网";
	$keyword2 = "体育";
	$keyword3 = "动植物";
	$keyword4 = "名人";
	$keyword5 = "家庭";
	$keyword6 = "工作";
	$keyword7 = "校园";
	$keyword8 = "爱情";
	$keyword9 = "社会";
	$keyword10 = "英语";
	$keyarray = array($keyword1,$keyword2,$keyword3,$keyword4,$keyword5,$keyword6,$keyword7,$keyword8,$keyword9,$keyword10);
	$count = 	count($keyarray);
	for($i = 0 ;$i < $count;$i++){
		$file=".\\".mb_convert_encoding(removeSpace($keyarray[$i]),"GB2312",ENCODING).".txt";
		$resource= fopen($file,"w+");
		if($resource == false)exit();
		$keyword = $keyarray[$i];	
		$pos =strpos($str,$keyword."</a>");
		$stride = $pos + strlen($keyword);
		$urlarray=array($url);
		if(substr($str,$stride,4) == "</a>"){
			$pos -=100;
			$pos = strpos($str,LINK,$pos);
			$pos += strlen(LINK) + 1;
			$newurl =getUrl($str,$pos);
			echo $newurl;
			$newcontent = file_get_contents($newurl);
			$urlarray[]=$newurl;
			$newcontent = mb_convert_encoding($newcontent,ENCODING,mb_detect_encoding($newcontent));
			if($newcontent != NULL){
					$time = 0;
					$newpos = 0;
					while(true){
							if($newpos  == 0){
									$newpos = strpos($newcontent, "class=\"content\"",$newpos);
									if($newpos == false){
										echo "newpos = false";
										break;
									}
									$pagepos = strpos($newcontent,"class=\"pages\"",$newpos);
									if($pagepos  == false){
										$endpos = strpos($newcontent,"<div class=\"navigation\">",$newpos);
										$pagepos = $endpos + 1;
										if($endpos == false){
											echo "endpos  = false";
											break;
										}	
									}else
									{
										$endpos = strpos($newcontent,"</div>",$pagepos);
										if($endpos == false){
											echo "endpos  = false";
											break;
										}
									}
							}
							$temp = strpos($newcontent, "class=\"content\"",$newpos);
							if($temp != false)
							{
								$newpos = $temp;
							}
							$newpos = strpos($newcontent, LINK,$newpos);
							if($newpos == false){
								echo "newpos  =0 line 68";
								continue;
							}
							if($newpos > $endpos)
							{
								break;
							}
							$newpos += strlen(LINK)+1;
							$newurl = getUrl($newcontent,$newpos);
							if(in_array($newurl,$urlarray) ==false){
								$urlarray[]=$newurl;
							}
							else
							{
								continue;
							}
							fwrite($out,$newurl."\n");
							if($newpos > $pagepos){
									$temp  = file_get_contents($newurl);
									if($temp != false){
										$newcontent = $temp;
										$newcontent = mb_convert_encoding($newcontent,ENCODING,mb_detect_encoding($newcontent));
										$newpos = 0;
									}
							}
							else{
									//if($daySpan >2 or  $daySpan < 0)break;
									$newcontent2 = file_get_contents($newurl);
									if($newcontent2 == false){
										echo "newcontent == false";
										continue;
									}
									$newcontent2 = mb_convert_encoding($newcontent2,ENCODING,mb_detect_encoding($newcontent2));
									$newpos2 = strpos($newcontent2,"<div class=\"content\">");
									$newpos2 = strpos($newcontent2,"<p>",$newpos2);
									if($newpos2 == false){
										echo "newpos2 == false";
										continue;
									}
									$text = "";
									$endpos2 = strpos($newcontent2,"<script type=\"text/javascript\">",$newpos2);
									if($endpos2 == false){
										echo "endpos2 = false";
										continue;	
									}
									$text =trim(removeHtmlMark(substr($newcontent2,$newpos2,$endpos2 - $newpos2)));
									store($resource,$newurl,$keyword,$text);
							}
					}
			}
			fclose($resource);
		}
	}
}

function ishuo()
{
	$out = fopen(".\\out","w+");
	$prefix = "http://www.ishuo.cn";
	$url = "http://www.ishuo.cn";
	$str = file_get_contents($url);
	$str = mb_convert_encoding($str,ENCODING,mb_detect_encoding($str));
	if($str == false){
		var_dump("file_get_contents error");
		exit();
	}
	$keyword1 = "经典台词";
	$keyword2 = "名人名言";
	$keyword3 = "真理格言";
	$keyword4 = "幽默讽刺";
	$keyword5 = "爱情语录";
	$keyword6 = "经典短信";
	$keyword7 = "网络生活";
	$keyword8 = "经典诗词";
	$keyarray = array($keyword1,$keyword2,$keyword3,$keyword4,$keyword5,$keyword6,$keyword7,$keyword8);
	$count = 	count($keyarray);
	for($i = 0 ;$i < $count;$i++){
		$file=".\\".mb_convert_encoding(removeSpace($keyarray[$i]),"GB2312",ENCODING).".txt";
		$resource= fopen($file,"w+");
		if($resource == false)exit();
		$keyword = $keyarray[$i];	
		$pos =strpos($str,$keyword."</a>");
		$stride = $pos + strlen($keyword);
		$urlarray=array($url);
		if(substr($str,$stride,4) == "</a>"){
			$pos -=40;
			$pos = strpos($str,LINK,$pos);
			$pos += strlen(LINK) + 1;
			$newurl =getUrl($str,$pos);
			$newcontent = file_get_contents($newurl);
			$urlarray[]=$newurl;
			$newcontent = mb_convert_encoding($newcontent,ENCODING,mb_detect_encoding($newcontent));
			if($newcontent != NULL){
					$time = 0;
					$newpos = 0;
					while(true){
							if($newpos  == 0){
									$newpos = strpos($newcontent, "<div id=\"content\">",$newpos);
									$pagepos = strpos($newcontent,"<div class=\"page\">",$newpos);
									$endpos = strpos($newcontent,"</div>",$pagepos);
									if($endpos == false){
										echo "endpos  = false";
										break;
									}
							}
							$newpos = strpos($newcontent, LINK,$newpos);
							if($newpos == false){
								echo "newpos  =0 line 68";
								break;
							}
							if($newpos > $endpos)
							{
								break;
							}
							$newpos += strlen(LINK)+1;
							$newurl = $prefix.getUrl($newcontent,$newpos);
							if(in_array($newurl,$urlarray) ==false){
								$urlarray[]=$newurl;
							}
							else
							{
								continue;
							}
							fwrite($out,$newurl."\n");
							if($newpos > $pagepos){
									$temp  = file_get_contents($newurl);
									if($temp != false){
										$newcontent = $temp;
										$newcontent = mb_convert_encoding($newcontent,ENCODING,mb_detect_encoding($newcontent));
										$newpos = 0;
									}
							}
							else{
									//if($daySpan >2 or  $daySpan < 0)break;
									$newcontent2 = file_get_contents($newurl);
									if($newcontent2 == false){
										echo "newcontent == false";
										break;
									}
									$newcontent2 = mb_convert_encoding($newcontent2,ENCODING,mb_detect_encoding($newcontent2));
									$newpos2 = strpos($newcontent2,"<div id=\"article_ad\">");
									$newpos2 = strpos($newcontent2,"<p>",$newpos2);
									if($newpos2 == false){
										echo "newpos2 == false";
										continue;
									}
									$text = "";
									$endpos2 = strpos($newcontent2,"<div id=\"sidebar\">",$newpos2);
									if($endpos2 == false){
										echo "endpos2 = false";
										continue;	
									}
									$text =trim(removeHtmlMark(substr($newcontent2,$newpos2,$endpos2 - $newpos2)));
									store($resource,$newurl,$keyword,$text);
							}
					}
			}
			fclose($resource);
		}
	}
}

function freeheart()
{
	$out = fopen(".\\out","w+");
	$prefix = "http://www.myfreeheart.cn";
	$url = "http://www.myfreeheart.cn";
	$str = file_get_contents($url);
	$str = mb_convert_encoding($str,ENCODING,mb_detect_encoding($str));
	if($str == false){
		var_dump("file_get_contents error");
		exit();
	}
	$keyword1 = "名人经典语录";
	$keyword2 = "年度经典语录";
	$keyword3 = "网络经典语录";
	$keyword4 = "爱情经典语录";
	$keyword5 = "经典小说语录";
	$keyarray = array($keyword1,$keyword2,$keyword3,$keyword4,$keyword5);
	$count = 	count($keyarray);
	for($i = 0 ;$i < $count;$i++){
		$file=".\\".mb_convert_encoding(removeSpace($keyarray[$i]),"GB2312",ENCODING).".txt";
		$resource= fopen($file,"w+");
		if($resource == false)exit();
		$keyword = $keyarray[$i];	
		$pos =strpos($str,$keyword."</a>");
		$stride = $pos + strlen($keyword);
		$urlarray=array($url);
		if(substr($str,$stride,4) == "</a>"){
			$pos -=100;
			$pos = strpos($str,LINK,$pos);
			$pos += strlen(LINK) + 1;
			$newurl =$prefix.getUrl($str,$pos);
			$newcontent = file_get_contents($newurl);
			$urlarray[]=$newurl;
			$newcontent = mb_convert_encoding($newcontent,ENCODING,mb_detect_encoding($newcontent));
			if($newcontent != NULL){
					$time = 0;
					$newpos = 0;
					while(true){
							if($newpos  == 0){
									$newpos = strpos($newcontent, "<div class=\"ggad2\">",$newpos);
									$pagepos = strpos($newcontent,"class=\"showpage\"",$newpos);
									$endpos = strpos($newcontent,"下一页",$pagepos);
									if($endpos == false){
										echo "endpos ";
										break;
									}
							}
							$newpos = strpos($newcontent, LINK,$newpos);
							if($newpos == false){
								echo "newpos  =0 line 68";
								break;
							}
							if($newpos > $endpos)
							{
								echo "newpos > endpos";
								break;
							}
							$newpos += strlen(LINK)+1;
							$newurl = $prefix.getUrl($newcontent,$newpos);
							if(in_array($newurl,$urlarray) ==false){
								$urlarray[]=$newurl;
							}
							else
							{
								continue;
							}
							fwrite($out,$newurl."\n");
							if($newpos > $pagepos){
									$temp  = file_get_contents($newurl);
									if($temp != false){
										$newcontent = $temp;
										$newcontent = mb_convert_encoding($newcontent,ENCODING,mb_detect_encoding($newcontent));
										$newpos = 0;
									}
							}
							else{
									//if($daySpan >2 or  $daySpan < 0)break;
									$newcontent2 = file_get_contents($newurl);
									if($newcontent2 == false){
										echo "newcontent == false";
										continue;
									}
									$newcontent2 = mb_convert_encoding($newcontent2,ENCODING,mb_detect_encoding($newcontent2));
									$newpos2 = strpos($newcontent2,"<div id=\"content\">");
									if($newpos2 == false){
										echo "$newpos2 == ";
										continue;
									}
									$text = "";
									$endpos2 = strpos($newcontent2,"<div id=\"artcat\">",$newpos2);
									if($endpos2 == false){
										echo "endpos2 = false";
										continue;	
									}
									$text =removeHtmlMark(trim(substr($newcontent2,$newpos2,$endpos2 - $newpos2)));
									store($resource,$newurl,$keyword,$text);
									
							}
					}
			}
			fclose($resource);
		}
	}
}

function yulu()
{
	$prefix = "http://www.yulu.info";
	$url = "http://www.yulu.info/sort";
	$str = file_get_contents($url);
	$keyword1 = "格言 / 真理";
	$keyword2 = "谚语 / 现实";
	$keyword3 = "娱乐 / 反语";
	$keyarray = array($keyword1,$keyword2,$keyword3);
	$count = 	count($keyarray);
	for($i = 0 ;$i < $count;$i++){
		$file=".\\".mb_convert_encoding(removeSpace($keyarray[$i]),"GB2312",ENCODING).".txt";
		$resource= fopen($file,"w+");
		if($resource == false)exit();
		$keyword = $keyarray[$i];	
		$pos =strpos($str,$keyword);
		$stride = $pos + strlen($keyword);
		if(substr($str,$stride,4) == "</a>"){
			$pos -=25;
			$pos = strpos($str,LINK,$pos);
			$pos += strlen(LINK) + 1;
			$newurl =$prefix. getUrl($str,$pos);		
			$newcontent = file_get_contents($newurl);
			if($newcontent != NULL){
				$newpos = 0;
				while(true){
						$newpos = strpos($newcontent, "class=\"ana\"",$newpos);
						if($newpos == false){break;}
						$newpos += strlen( "class=\"ana\"") + 1;
						$endpos = strpos($newcontent,"</p>",$newpos);
						$text1 = substr($newcontent,$newpos,$endpos - $newpos);
						$authorpos = strpos($newcontent,"class=\"auther\"",$endpos);
						if($authorpos == false){break;}
						$authorpos += strlen("class=\"auther\"") + 1;
						$authorendpos = strpos($newcontent,"</p>",$authorpos);
						$text1 .= removeHtmlMark(trim(substr($newcontent,$authorpos,$authorendpos - $authorpos)));
						store($resource,$newurl,$keyword,$text1);
						
				}
			}	
			fclose($resource);
		}
	}
}

function getTime($str,$pos)
{
	$pos = strpos($str,"<span style=\"color:red\">",$pos);
	if($pos == false)return false;
	$pos +=strlen("<span style=\"color:red\">");
	$endpos = strpos($str,"</span>",$pos);
	$time =  substr($str,$pos,$endpos- $pos);
	$timearray = explode("-",$time);
	return $timearray;
	/*$pos = strpos($str,"</span>",$pos);
	if($pos == false)return false;
	$startpos =$pos - strlen("2010-01-01");
	$time =  substr($str,$startpos,$pos- $startpos);
	$timearray = explode("-",$time);
	return $timearray;*/
}

function getDaySpan($time,$out)
{
	if($time == false)return 100;
	$result = 0;
	$now = getdate();
	$nowyear = intval($now["year"] );
	$timeyear = intval($time[0]);
	if($nowyear != $timeyear)$result = -3;
	$nowmonth = intval($now["mon"]);
	$timemonth = intval($time[1]);
	$nowday = intval($now["mday"]);
	$timeday = intval($time[2]);
	if($nowmonth == $timemonth)
	{
		 $result  =$nowday- $timeday;
	}else
	{	
		if($nowmonth - $timemonth > 1){
			$result  = -1;
		}
		else if($nowmonth - $timemonth == 1){
			$result  =  $nowday - $timeday + 30;
		}else
		{
			$result  = -2;
		}
	}
	return $result;
}

function mtw51()
{
	$out= fopen(".\\out","wb");
	$prefix = "http://www.51mtw.cn/";
	$url = "http://www.51mtw.cn";
	$str = file_get_contents($url);
	$str = mb_convert_encoding($str,ENCODING,mb_detect_encoding($str));
	$keyword1 = "笑话集锦";
	$keyword2 = "爆笑网文";
	$keyarray = array($keyword1,$keyword2);
	$count = 	count($keyarray);
	for($i = 0 ;$i < $count;$i++){
		$file=".\\".removeSpace($keyarray[$i]).".txt";
		$file = mb_convert_encoding($file,"GBK",ENCODING);
		$resource= fopen($file,"wb");
		if($resource == false)exit();
		$keyword = $keyarray[$i];//mb_convert_encoding($keyarray[$i],ENCODING);
		//$keyword = $keyarray[$i];
		$pos =strpos($str,$keyword);
		$stride = $pos + strlen($keyword);
		$urlarray = array($url);
		$newpos = 0;
		if(substr($str,$stride,4) == "</a>"){
			$pos -=70;
			$pos = strpos($str,LINK,$pos);
			$pos += strlen(LINK) + 1;
			$newurl =getUrl($str,$pos);
			fwrite($out,"new url is ".$newurl."\n");
			$newcontent = file_get_contents($newurl);
			if($newcontent != NULL){
					$time = 0;
					while(true){
							if($newpos  == 0){
									$newpos = strpos($newcontent, "class=\"listbox\"",$newpos);
							}
							$newpos = strpos($newcontent, LINK,$newpos);
							if($newpos == false){break;}
							$newpos += strlen(LINK)+1;
							$daySpan = getDaySpan(getTime($newcontent,$newpos),$out);
							//fwrite($out,"dayspan is ".$daySpan."\n");
							$newurl = getUrl($newcontent,$newpos);
							if(!isLegalUrl($newurl)){
									$pagepos = strpos($newcontent,"class=\"num\"",$newpos);
									if($pagepos != false){
											while(true){
													$pagepos = strpos($newcontent,LINK,$pagepos);
													if($pagepos == false){
														break;
													}
													$pagepos += strlen(LINK)+1;
													$newurl = getUrl($newcontent,$pagepos);
													if($newurl == "#")continue;
													$newurl = $prefix.$newurl;
													if(strpos($newurl,"htm") == false)break;
													if(in_array($newurl,$urlarray) == false){
															$urlarray[]=$newurl;
															$temp  = file_get_contents($newurl);
															if($temp == false){
																continue;
															}
															else{
																$newcontent = $temp;
																$newpos = 0;
																break;
															}
													}
											}
									}else{
										break;
									}
							}
							else{
									//if($daySpan >7 or  $daySpan < 0)break;
									$newcontent2 = file_get_contents($newurl);
									if($newcontent2 == false)continue;
									//$newcontent2 = mb_convert_encoding($newcontent2,ENCODING,mb_detect_encoding($newcontent2));
									$newpos2 = strpos($newcontent2,"<div id=\"MyContent\">");
									if($newpos2 == false)break;
									else if($newpos2 != false)$newpos2 += strlen("<div id=\"MyContent\">");
									$endpos2 = strpos($newcontent2,"</div>",$newpos2);
									$text = removeHtmlMark(trim(substr($newcontent2,$newpos2,$endpos2 - $newpos2)));
									if($i ==1){
										$newpage = $newpos2;
										while(true)
										{
											$newpage = strpos($newcontent2, "class=\"num\"",$newpage);
											if($newpage == false)break;
											$newpage += strlen("class=\"num\"");
											$newpage = strpos($newcontent2,LINK,$newpage);
											$newpage += strlen(LINK) + 1;
											$newurl   = getUrl($newcontent2,$newpage);

											$newcontent2 = file_get_contents($newurl);
											//$newcontent2 = mb_convert_encoding($newcontent2,ENCODING,mb_detect_encoding($newcontent2));
											$newpos2 = strpos($newcontent2,"<div id=\"MyContent\">");
											if($newpos2 == false)break;
											else if($newpos2 != false)$newpos2 += strlen("<div id=\"MyContent\">");
											$endpos2 = strpos($newcontent2,"</div>",$newpos2);
											$text .= removeHtmlMark(trim(substr($newcontent2,$newpos2,$endpos2 - $newpos2)));
										}
									}
									$text = mb_convert_encoding($text,ENCODING,mb_detect_encoding($text));
									store($resource,$newurl,$keyword,$text);
							}
					}
			}
			fclose($resource);
		}
	}
}



function msn()
{
	$out = fopen(".\\out","w+");
	$prefix = "http://show.msn.com.cn/TopNews.aspx?dt=";
	$timearray = explode('/',date("Y/n/j"));
	$year = $timearray[0];
	$month  =$timearray[1];
	$day = $timearray[2];
	$file = ".\\msn.txt";
	$bigmonth = array(1,3,5,7,8,10,12);
	$smallmonth = array(4,6,9,11);
	$resource = fopen($file,"wb");
	$urlarray = array();
	for($i = 0; $i < 365 ;$i++)
	{
			if($day <=0){
				$month -=1;
				if($month <=0)
				{
					$month = 12;
					$year -=1;
				}

				if(in_array($month,$bigmonth) == true){
					$day = 31;
				}
				else
				{
					if($month == 2){
						if($year%400 == 0)
						{
							$day =29;
						}
						else
						{
							$day = 28;
						}
					}else
					{
						$day = 30;
					}
				}
			}
			$url = $prefix.$year."/".$month."/".$day;
			if(in_array($url,$urlarray)== true)continue;
			$urlarray[]=$url;
			//fwrite($out,"url = ".$url."\n");
			$content = file_get_contents($url);
			$newpos = 0;
			$day -= 1;
			while(true)
			{
					$temp = strpos($content,"class=\"top10\"",$newpos);
					if($temp == false){
						if($newpos == 0){
							break;
						}
						break;
					}
					$newpos = strpos($content,"<p>",$newpos);
					if($newpos == false)break;
					$newpos += strlen("<p>");
					$endpos = strpos($content,"<",$newpos);
					if($endpos == false)break;
					$text =removeHtmlMark(trim(substr($content,$newpos,$endpos- $newpos)));
					store($resource,$url,"top10",$text);
			}
			if($newpos == 0)break;
	}
}


function getnowdate()
{
		$now = getdate();
		return $now["year"]."_".$now["month"]."_".$now["mday"];
}

function removeSpace($str)
{
	$str=preg_replace('/[\s\/]+/','',$str);  
	$str =str_replace("　",'',$str);
	return $str;
}

function removeHtmlMark($str)
{
	//$str = removeSpace($str);
	$str =str_replace("　",'',$str);
	$str = preg_replace("/<.*?>/",'',$str);
	$str = preg_replace("/\s*&.*?;/",'',$str);
	return  $str;
}

//$pos:start position of url, not including double quotation 
function getUrl($str,$pos){
	$start = $pos;
	if(ord($str[$pos]) == ord("\"") or ord($str[$pos]) == ord("'")){
			$pos+=1;
	}
	while(ord($str[$pos]) != ord("\"") and ord($str[$pos]) != ord("'")){
			$pos+=1;
	}
	return substr($str,$start+1,$pos-$start-1);
}

function isLegalUrl($str)
{
	$res = preg_match("/https?:\/\/.*$/",$str);
	return $res;
}

function removePunc($str)
{
	$punc= array("，","。","？","：","　"," ","；","—","！");
	$count = count($punc);
	for($i = 0; $i < $count;$i++)
	{
			$str = 	str_replace($punc[$i],'',$str);
	}
	$str = preg_replace("/[[:punct:]]/",'',$str);
	return $str;
}

function store($resource,$url,$tab,$text)
{
	$twitter = addslashes(trim($text));
	$twitter_nopunc = trim(removePunc($twitter));
	if(strlen($twitter_nopunc) > 2048)return false;
	if(empty($twitter_nopunc)){
		var_dump($twitter_nopunc);
	}
	$sql = "select count(*) from webinfo where twitter_nopunc='".$twitter_nopunc."' and tab = '".$tab."'";
	$res = mysql_query($sql);
	if($res == false){
		echo mysql_error();
		return false;
	}
	$row = mysql_fetch_array($res);
	if(intval($row[0]) == 0){
		$sql = "insert into webinfo values('','".$url."'".",'".$tab."','".$twitter."','".$twitter_nopunc."','".time()."')";
		mysql_query($sql) or die("mysql error = ".mysql_error());
		fwrite($resource,$twitter."\r\n"."\r\n");
	}else{
		//echo "ROW[0] = ".$row[0].", ".$sql.NEWLINE;
	}
	mysql_free_result($res);
	return true;
}

?>