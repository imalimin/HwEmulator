import os

dir = './infones/mapper'
list = os.listdir(dir)
for i in range(0,len(list)):
	path = os.path.join(dir, list[i])
	if os.path.isfile(path):
		print(path)
		f = open(path, 'r+')
		content = f.read()
		f.seek(0, 0)
		f.write('#include "InfoNES_INC.h"\n')
		f.write(content)
		f.close()