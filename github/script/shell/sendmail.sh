#!/bin/bash
/usr/sbin/sendmail -t <<EOF
From:fatshaw <fatshaw@gmail.com>
To: chjxiao@gmail.com
Cc: chjxiao@gmail.com
Bcc: chjxiao@gmail.com
Subject: mail testing
----------------------------------
This is the mail content ...
---------------------------------
EOF