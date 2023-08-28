import matplotlib.pyplot as plt
import sys
import ast
import subprocess
from matplotlib.ticker import FormatStrFormatter


nodes = ast.literal_eval(sys.argv[1])
clockSizes = ast.literal_eval(sys.argv[2]) #[20,10,7,5,4,3,2.5]


for c in clockSizes: # pour chaque taille d'horloge
	plt.figure()	
	x =  []
	y1 = []
	y2 = []
	y3 = []
	y4 = []

	f=open("Graphs/data/"+ str(nodes[0]) +"_" + str(c) + "/clockSizeFile.dat",'r') 
	for line in f : 
		x.append(float(line.split()[0]))
		y1.append(float(line.split()[1]))
		y2.append(float(line.split()[2]))
		y3.append(float(line.split()[3]))
		y4.append(float(line.split()[4]))

	print("components " + str(float(sum(y2)/len(y2))))
	print("Average active components " + str(float(sum(y3)/len(y2))))
	print("avereage active components when broadcast message " + str(float(sum(y4)/len(y2))))
#	print(x)
#	print(y)
#	plt.plot(x,y, label="Message load (messages/second)", marker='s', color='b')

#	plt.plot(x,y1, label="Components", marker='s', color='r')
	plt.plot(x,y2, label="Active Components", marker='+', color='b')
#	plt.plot(x,y3, label="Average Active Components", marker='s', color='g')
#	plt.plot(x,y4, label="Average Active Components Broadcast", marker='v', color='y')

	plt.yticks([0,100,200,300,400,500,600,700,800,900,1000])
#	plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
	plt.tick_params(axis='both', which='major', labelsize=14)
	plt.tick_params(axis='both', which='minor', labelsize=14)

#	plt.gca().set_ylim([0,0.0012])
	plt.gca().yaxis.offsetText.set_fontsize(16)
	plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.0f'))


#	plt.xlabel('Number of nodes', fontsize=18)
	plt.xlabel('Time (seconds)', fontsize=18)
	plt.ylabel('ClockSize (entries)', fontsize=18)
		
	# ~ plt.title('Average size of BufferReceive of MH in time ')
#	plt.legend(fontsize=14)
	plt.tight_layout()
	
	plt.savefig("Graphs/ClockSize"+str(c)+".eps", format='eps',bbox_inches="tight")

#	plt.show()
	plt.close("all")
