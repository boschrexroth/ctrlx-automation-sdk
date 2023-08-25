#!/usr/bin/python3

# Helper script to search and replace arbitrary strings (including special characters)

import sys

filename = sys.argv[1]
search = sys.argv[2]
replace = sys.argv[3]

print ("___________________________________________________________")
print ("Filename:",filename)
print ("Searching:",  search)
print ("Replacing:", replace[:10], " ...")

f = open(filename, 'r') 
data = f.read()
f.close()

data = data.replace(search, replace)
  
f = open(filename, 'w')
f.write(data)
f.close()

