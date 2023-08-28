import matplotlib.pyplot as plt
import sys
import ast
import subprocess
from matplotlib.ticker import FormatStrFormatter


nodes = ast.literal_eval(sys.argv[1])
clockSizes = ast.literal_eval(sys.argv[2]) #[20,10,7,5,4,3,2.5]


for c in clockSizes: # pour chaque taille d'horloge
	plt.figure()	
	x =   []
	y =   []
	
	f=open("Graphs/data/"+ str(nodes[0]) +"_" + str(c) + "/messageLoadFile.dat",'r') 
	for line in f : 
		x.append(float(line.split()[0]))
		y.append(float(line.split()[1]))

	print("Average message load "+ str(sum(y)/len(y)))
#	print(x)
#	print(y)
#	plt.plot(x,y, label="Message load (messages/second)", marker='s', color='b')
	plt.plot(x,y, marker='.', color='b')
	
		
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
	
	plt.savefig("Graphs/MessageLoad"+str(c)+".eps", format='eps',bbox_inches="tight")

#	plt.show()
	plt.close("all")
