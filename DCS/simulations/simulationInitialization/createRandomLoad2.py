import matplotlib.pyplot as plt
import sys
import ast
import subprocess
from matplotlib.ticker import FormatStrFormatter
import scipy
from scipy.stats import norm
import random 

maxtarget = 200

plt.figure()	

xfinal=[]
yfinal=[]

xfinal = [i for i in range(0,850)]
yfinal = []
for i in range(0,3):
	for j in range(0,190):
		yfinal.append(random.randint(10,30))
	yfinal = yfinal + [40,50,62,75,87,100,113,125,138,150]
	for j in range(0,50):
		yfinal.append(random.randint(150,200))
for j in range(0,100):
	yfinal.append(random.randint(10,30))


with open("../loadFile.txt","w") as f:
	for l in yfinal:
		f.write(str(l)+'\n')

print("Average message load "+ str(sum(yfinal)/len(yfinal)))
#	print(x)
#	print(y)
#	plt.plot(x,y, label="Message load (messages/second)", marker='s', color='b')
plt.plot(xfinal,yfinal, marker='.', color='b')

		
#	plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
plt.tick_params(axis='both', which='major', labelsize=14)
plt.tick_params(axis='both', which='minor', labelsize=14)

#	plt.gca().set_ylim([0,0.0012])
plt.gca().yaxis.offsetText.set_fontsize(16)
plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.1f'))


#	plt.xlabel('Number of nodes', fontsize=18)
plt.xlabel('Time (seconds)', fontsize=18)
plt.ylabel('Message load (messages/second)', fontsize=18)
		
	# ~ plt.title('Average size of BufferReceive of MH in time ')
#	plt.legend(fontsize=14)
plt.show()
plt.tight_layout()
