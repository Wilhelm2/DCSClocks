import matplotlib.pyplot as plt
import sys
import ast
import subprocess
from matplotlib.ticker import FormatStrFormatter
import scipy
from scipy.stats import norm

maxtarget = 200

plt.figure()	

xfinal=[]
yfinal=[]


x = [i for i in range(0,350)]
y = norm.pdf(x,175,40)
y = [i*maxtarget/y[175] for i in y]	
xfinal = xfinal + x
yfinal = yfinal + y



x = [i for i in range(350,600)]
y = norm.pdf(x,475,30)
y = [i*maxtarget/y[125] for i in y]	
xfinal = xfinal + x
yfinal = yfinal + y


x = [i for i in range(600,750)]
y = norm.pdf(x,675,20)
y = [i*maxtarget/y[75] for i in y]	
xfinal = xfinal + x
yfinal = yfinal + y


x = [i for i in range(750,850)]
y = norm.pdf(x,800,15)
y = [i*maxtarget/y[50] for i in y]	
xfinal = xfinal + x
yfinal = yfinal + y

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
plt.tight_layout()
	
plt.savefig("MessageLoadExample.eps", format='eps',bbox_inches="tight")

plt.show()
plt.close("all")
