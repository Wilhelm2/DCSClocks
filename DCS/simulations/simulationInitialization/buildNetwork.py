import sys
from shutil import copyfile


copyfile("model.txt", "../DynamicClockSet.ned")
with open("../DynamicClockSet.ned","a") as f:
	
	f.write("\t\tut:SimulationParameters;\n\t\tNodes ["+sys.argv[1] +"]: Node;\n\t\tcontrol : DeliveryController;\n\t\tstat	: Stats;\n\t\tcentralNode : CentralCommunicationNode;\n\t\tconnections:\n//    for i=0..5{\n\t//for j=i..5{\n//             	Nodes[i].neighbour++ <--> Channel <--> Nodes[j].neighbour++;\n            //}\n  //      }\n")
	for i in range(0,int(sys.argv[1])) :
		f.write("\t\tNodes["+str(i)+"].neighbour++ <--> Channel <--> centralNode.neighbour++;\n");
	f.write("}")

with open("../omnetpp.ini","w") as f:
	f.write("[General]\n")
	f.write("ned-path=.:/home/wilhelm/Documents/omnetpp-5.5.1/samples/inet/src\n")
	f.write("network = dcs.DCS\nsim-time-limit = 1000s\n**.**.nbNodes =" + sys.argv[1] + "\n**.**.clockLength = " + sys.argv[2]+"\n")
	f.write("\n**.vector-recording = false\n**.scalar-recording = false\n**.statistic-recording = false\n")


