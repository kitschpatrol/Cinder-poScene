#!/usr/bin/python

import os
import sys
import shutil
import platform
from subprocess import call
from os.path import join, dirname

#Run Clang-Format on all eligible files
for root, dirs, files in os.walk(sys.argv[1]):
	for name in files:
		if not name.endswith(('.h', '.cpp', '.c', '.mm')):
			continue
		call(['clang-format', '-i', '-style=file', join(root, name)])