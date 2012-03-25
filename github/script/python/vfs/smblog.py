import os
import codecs
import csv
import socket
table=dict({'rename':'Change Name','pread':'Open File','pwrite':'Write File','unlink':'Delete File','mkdir':'Create Directory','rmdir':'Delete Directory'})
hostname=socket.gethostname()+"/"
delimiter="\t"
connector="\""+delimiter+"\""
outputpath="/mnt/system/auditLog/download/samba/"
fd = open(outputpath+"log.csv",'wb')
fd.write("\xef\xbb\xbf")
fd.write("\"DATE"+connector+"USERNAME"+connector+"IP"+connector+"OPERATION"+connector+"PARAMETERS\"\r\n")
filecontent = open("/var/log/messages")
for line in filecontent:
        ind = line.find("full_audit")
        if ind != -1:
                text=line[ind+len("full_audit|"):]
                i=0
                start=0
                end=0
                output="\""
                while i < 3:
                        end = text.find("|",start)
                        if end != -1:
                                output+=text[start:end]+connector
                        start = end+1
                        i+=1
                end=text.find("|",start)
                #path=""
                #if end > start:
                #       path=text[start:end]+"/"
		path=hostname+text[start:end]+"/"
                start=end+1
                end=text.find("|",start)
                output+=table[text[start:end]]+connector
                start=end+1
                end=text.find("|",start)
                start=end+1
                param=text[start:]
                params=param.split("|")
                for p in params:
                        p=p.replace("///","/")
                        if p[0:2]=='./':
                                output+=path+p[2:]
                        elif p[0] == '.':
                                output+=path+p[1:]
                        else:
                                output+=path+p
                        output+="=>"
                output=output[0:-3]
		output+="\"\r\n"
                fd.write(output)
filecontent.close()
fd.close()
cmd="iconv -f utf-8 -t unicodelittle "+outputpath+"log.csv >"+outputpath+"log1.csv;mv "+outputpath+"log1.csv "+outputpath+"log.csv"
os.system(cmd)
