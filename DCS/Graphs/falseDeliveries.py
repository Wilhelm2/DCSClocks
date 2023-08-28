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
	outOfCausalOrderDeliveries =   []

	f=open("Graphs/data/"+ str(nodes[0]) +"_" + str(c) + "/deliveriesFile.dat",'r') 
	for line in f : 
		x.append(float(line.split()[0]))
		outOfCausalOrderDeliveries.append(float(line.split()[1]))

	# To only keep the differences
	outOfCausalOrderDeliveriesNew = [outOfCausalOrderDeliveries[0]]
	for i in range(1,len(x)):
		outOfCausalOrderDeliveriesNew.append(outOfCausalOrderDeliveries[i]-outOfCausalOrderDeliveries[i-1])
	print(outOfCausalOrderDeliveriesNew)
	outOfCausalOrderDeliveries = outOfCausalOrderDeliveriesNew
		
	xnew = []
	outOfCausalOrderDeliveriesNew = []
	for i in range(0,len(x)):
		if outOfCausalOrderDeliveries[i] != 0:
			xnew.append(x[i])
			outOfCausalOrderDeliveriesNew.append(outOfCausalOrderDeliveries[i])

	print(xnew)
	print(outOfCausalOrderDeliveriesNew)
	plt.scatter(xnew,outOfCausalOrderDeliveriesNew, marker='+', color='r')
	#plt.plot(x,detected, label="Detected", marker='+', color='r')

	plt.tick_params(axis='both', which='major', labelsize=14)
	plt.tick_params(axis='both', which='minor', labelsize=14)

	plt.gca().yaxis.offsetText.set_fontsize(16)
	plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.1f'))

	plt.xlabel('Time (seconds)', fontsize=18)
	plt.ylabel('Out of causal order deliveries', fontsize=18)
		
	# ~ plt.title('Average size of BufferReceive of MH in time ')
#	plt.legend(fontsize=14)
	plt.tight_layout()
	
	plt.savefig("Graphs/FalseDeliveries"+str(c)+".eps", format='eps',bbox_inches="tight")

#	plt.show()
	plt.close("all")
