#!/bin/sh
java  -cp "/root/share/gec/weka.jar:/root/share/gec/src" TestEnsemble /root/share/data/ true 1 2 2 1000 > log 2>&1 &