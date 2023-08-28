
import subprocess
from shutil import copyfile
import sys
import ast

nbNodes 	= [1000] # [150,175,200,225,250,300]
clockSize	= [50]#25,50,75] #[100,150,200]

subprocess.check_call(["bash","-c","mkdir -p Graphs/data"])
subprocess.check_call(["bash","-c","mkdir -p simulations/data"])

subprocess.check_call(["bash","-c","make"])
subprocess.check_call(["bash","-c","make MODE=debug"])

for c in clockSize:
	for n in nbNodes:
		print "Number of nodes " + str(n)

		subprocess.check_call(["bash","-c","python buildNetwork.py " + str(n) + " " + str(c)], cwd='simulations/simulationInitialization/') # builds the network
		
		subprocess.check_call(["bash","-c","./out/gcc-release/src/DCS -f simulations/omnetpp.ini -n src -u Cmdenv > res.txt" ])	
#		subprocess.check_call(["bash","-c","./out/gcc-release/src/DynamicClockSet -f omnetpp.ini -n src -u Cmdenv > /dev/null" ], cwd='./')	

		
#		subprocess.check_call(["bash","-c","./out/gcc-debug/src/DynamicClockSet_dbg -f ../codeGraphes/omnetpp.ini -n src -u Cmdenv > /dev/null" ], cwd='../codeDCS')	
#		subprocess.check_call(["bash","-c","gdb --args ./out/gcc-debug/src/DCS_dbg -f simulations/omnetpp.ini -n src -u Cmdenv " ])	
#		subprocess.check_call(["bash","-c","valgrind ./out/gcc-debug/src/DCS_dbg -f simulations/omnetpp.ini -n src -u Cmdenv " ])	
		
		subprocess.check_call(["bash","-c","mkdir -p Graphs/data/" + str(n) + "_" + str(c)])
		subprocess.check_call(["bash","-c","cp -r simulations/data/* Graphs/data/" + str(n) + "_" + str(c)  ])


subprocess.check_call(["bash","-c","python Graphs/messageLoad.py '"+ str(nbNodes)+ "' '" + str(clockSize) + "'" ])
subprocess.check_call(["bash","-c","python Graphs/clockSize.py '"+ str(nbNodes)+ "' '" + str(clockSize) + "'" ])
subprocess.check_call(["bash","-c","python Graphs/falseDeliveries.py '"+ str(nbNodes)+ "' '" + str(clockSize) + "'" ])
