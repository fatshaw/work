#! /usr/bin/python

import os
import sys
import re
import shutil

shutil.copyfile("../../../../webagent/trunk/src/msg/pmsg/coremessage.proto", "./coremessage.proto")
shutil.copyfile("../../../../webagent/trunk/src/msg/pmsg/pmsgforphp.proto", "./pmsgforphp.proto")
