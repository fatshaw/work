#alarm
alarm.mailhost=mail1-in.baidu.com
alarm.signature=<table cellpadding=\"0\" cellspacing=\"1\"><tr><td><img src=\"cid:%s\"></td><td width=164 nowrap style='width:98.45pt;border:solid white 1.0pt; background:#E10601;padding:1.4pt 5.4pt 1.4pt 5.4pt'><p class=MsoNormal align=center style='text-align:center'><b><span lang=EN-US style='font-size:10.0pt;font-family:\"\u5FAE\u8F6F\u96C5\u9ED1\",\"sans-serif\";color:white'>\u767E\u5EA6\u4EA7\u54C1\u5E93<o:p></o:p></span></b></p></td><td wnowrap style='width:61.55pt;border:solid white 1.0pt; border-left:none;background:#2319DC;padding:1.4pt 5.4pt 1.4pt 5.4pt'><p class=MsoNormal align=center style='text-align:center'><b><span lang=EN-US style='font-size:10.0pt;color:white'>product.baidu.com<o:p></o:p></span></b></p></td></tr></table> 
alarm.tolist=nothing@baidu.com
alarm.from=no-reply-pic@baidu.com
alarm.prodblogourl=http://s3.bae.baidu.com/prodb-image/6593_2593.png

#gsm
gsm.bin=/bin/sh /home/work/local/prodb/pic-service/sentgsm.sh

# products file pusher
filepusher.defaultuploaduser=upload
filepusher.defaultuploadpassword=upload
filepusher.basethreadpoolsize=5
filepusher.increthreadpoolsize=5
filepusher.defaultpollinterval=1000
filepusher.defaultbasepollinterval=10000
filepusher.basesemaphoreprefixname=semaphore_
filepusher.defaultscptimeout=30000
filepusher.basefilepusherwaittime=60
filepusher.defaultfilepushsizeincrement=10000
filepusher.defaultfilepushfetchsize=10000
filepusher.basefiledir=/home/work/pushfiles
filepusher.defaultwaitbasefiletime=7200000
filepusher.dasdefaultprefix=Increment.
filepusher.dasdefaultfilesize=100000
filepusher.weigo.kvconfpath=service-conf/weigo_adapter_conf/adapter_settings
filepusher.weigo.kvimplclasspath=com.baidu.adrc.picservice.weigo.kvinterface.WeigouKVInterfaceImpl
filepusher.weigo.kvbatchsize=2000
filepusher.weigo.pricefilterconfid=20
filepusher.task.whitelist=1
filepusher.weigo.pricemaillist=prodb-mon@baidu.com,prodb-pm@baidu.com
# webservice
webservice.notoken=fbef4dcf-812c-43dc-90f1-0d704dc7fa40
webservice.whitelist=10.42.86.12;10.46.99.20;10.65.211.44;10.26.7.150
webservice.productLineValidate=/bin/sh /home/work/local/prodb/pic-core/productLineValidate.sh
#webservice IP/Port setting
webservice.cxf.ip=10.42.86.12
webservice.cxf.port=8745
webservice.cdt.ip=10.42.86.12
webservice.cdt.port=8746

#index helper
indexhelper.defaultbatchsize=3000

#kv

kv.defaultbatchsize=3000
kv.confpath=
kv.confname=kvconf.conf
kv.rangename=

# se
se.defaultbatchsize=2000
se.sessionexpiredseconds=600

#basecheck
basecheck.batchsize=500
basecheck.batchtrytimes=3
basecheck.comparebatchsize=10000
basecheck.threadpoolsize=5
basecheck.morefilenum=10
basecheck.runbasecheck=sh /home/work/local/prodb/pic-service/basecheck.sh

das.realtime.config_file=service-conf/weigo_adapter_conf/adapter_settings
das.realtime.threadsize=1

price.blacklist.dumpfile=./price_blacklist/price_blacklist_dump
webservice.processor.nonationmerchantid=56,56,56,56