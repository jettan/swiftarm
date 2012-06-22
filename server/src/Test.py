#!/usr/bin/python

import sys
import time

def main():
	while True:
		print >> sys.stderr, "Hello"
		time.sleep(2)
#		sys.stdin.read()

def search():
	print >> sys.stderr, "World"
