# DCSClocks

# Introduction

This project implements a causal broadcast algorithm using Dynamic clock sets for distributed systems in C++ on the OMNeT++ simulator.
Causal order is defined by the *happened-before* relationship introduced by Lamport in 1978 [[1]](https://amturing.acm.org/p558-lamport.pdf). 
The *happened-before* relationship orders events in distributed systems following three rules [[1]](https://amturing.acm.org/p558-lamport.pdf):

>**Happened-before relation:**
>	Considering two events *e1* and *e2*, *e1* causally precedes *e2* (*e1*$\rightarrow$ *e2*) iff:
>	1. *e1* and *e2* occur on the same process and *e1* precedes *e2* or
>	2. for a message m *e1*=send(m) and *e2*=deliver(m) or
>	3. there exists an event *e3* such that *e1*$\rightarrow$*e3* and *e3*$\rightarrow$ *e2*.

Causal order ensures that any two causally related messages are delivered to applications respecting that order. 

>**Causal order:** 
>	Processes deliver messages while respecting the causal relation between them.
>	For any message $m$ and $m'$, if $m$ causally precedes $m'$, denoted $m\rightarrow m'$, then all processes deliver $m$ before $m'$:
>	<p align = "center">send(m) $\rightarrow$ send(m') $\Rightarrow$ deliver(m) $\rightarrow$ deliver(m'). </p>

**Causal broadcast:** Causal broadcast ensures that for any message *m* and *m'*, if $m$ causally precedes $m'$, denoted $m\rightarrow m'$, then all processes deliver $m$ before $m'$:
>	<p align = "center">broadcast(m) $\rightarrow$ broadcast(m') $\Rightarrow$ deliver(m) $\rightarrow$ deliver(m'). </p>

# Dynamic Clock Sets (DCS clocks)

DCS clocks[[3]] are logical clocks composed of Probabilistic clocks[[2]], as shown in the Figure below: 
![Example of DCS clock](readmeFigures/DCS.jpeg?raw=true)

The main feature of DCS clocks over Probabilistic ones is that their size can be modified during execution. 
The size of a DCS clock can be modified by adding and removing components to the clock. 
The clock clockManagement manages the DCS clock of processes. 

The size of DCS clocks should notably be set following the number of concurrent messages inside the system, which depends on the communication patterns and message load of the system. In the simulation we set the communication pattern and vary the message load to modify the number of concurrent messages inside the system. 

A more detailed explanation about DCS clocks can be found here [[3]].

# OMNeT++

The project was implemented on the version 5.6.1 of the OMNeT++ simulator [[4]](https://omnetpp.org/). 
This section gives a brief description of OMNeT++, which is an event based simulator primarily used for building network simulators. 
It is organized into modules that are combined in order to form a network. 

OMNeT++ uses the following specific files:
1. **Initialization file:** Contains the simulation parameters. Has the extension *.ini*.
2. **Network files:** Define modules as well as the simulation network which is composed of modules. Have the extension *.ned*.
3. **Message files:** Define the messages used by nodes. Have the extension *.msg*.


# User guide 

This section firt explains how to install the required resources to execute the project. Then it shows how to import the project in the OMNeT++ built-in IDE based on Eclipse. Finally, it describes how to execute the simulation on the terminal.

## OMNeT++ installation 

The installation of the version 5.6.1 of OMNeT++ is explained [here](https://doc.omnetpp.org/omnetpp5/InstallGuide.pdf).
Note that OMNeT++ is not (even though mostly) retro-compatible. 
Hence, some modifications might be required when using another version of OMNeT++. 
Thus you should carefully install the version 5.6.1 of OMNeT++.

## Import project in OMNeT++ IDE

OMNeT++ comes with a built-in IDE based on Eclipse. This section explains how to import the project in that IDE and compile it. 

First, launch the OMNeT++ IDE with the command:
> omnetpp

Second, see [here](http://wnss.sv.cmu.edu/teaching/14814/s15/files/hw2addendum.pdf) a guide written by Brian Ricks on how to import existing code within the Ecplise IDE.

Third, the compilation of .msg files requires adding: in Project Explorer (left bar):
> Left click on the project -> Properties -> OMNeT++ -> Makemake -> Options -> Custom -> add MSGC:=$(MSGC) --msg6 -> OK -> Apply and Close.

## How to run the simulation 

The simulation can be launched from the IDE as well as from the terminal.

To launch it from the IDE, click on the *Debug* or *Run* buttons in the bar under the *Menu* bar. 

To launch the simulation from the terminal:
1. Open a terminal in the project directory
2. Configure the files *omnetpp.ini* and *network.ned* following the desired simulation settings (number of nodes, control mechanism,...) 
3. Execute the command: 
>../out/gcc-release/src/DCS -f omnetpp.ini -u Cmdenv 

The python script 'execution.py' is also provided. It builds the *.ini* and *.ned* files, launches the simulation, copies the simulation statistics in folders, and builds several graphs with those statistics.
Configure the simulation parameters by modifying 'execution.py'. 
Launch *execution.py* by executing the following command in the *DCS* directory:
> python execution.py

## Data analysis

Statistics about the simulation are collected and written in the *simulations/data* folder. 

The folder *Graphs* contains python scripts to build graphs from those statistics. They are mainly written to be used from the 'execution.py' script, but can also be used outside of it. 

# License

This project is distributed under the license GNU 3.0. 

# References

<a id="HappenedBefore">[1]</a> Time, Clocks, and the Ordering of Events in a Distributed System. Lamport, Leslie. Communications of the ACM 1978.

<a id="PC1">[2]</a> Probabilistic Causal Message Ordering. Mostéfaoui, Achour and Weiss, Stéphane. PaCT 2017.

<a id="PC">[3]</a> A probabilistic Dynamic Clock Set to capture message causality. Wilhelm, Daniel and Arantes, Luciana and Sens, Pierre. Technical report 2022.

<a id="OMNeT++">[4]</a> The OMNET++ Discrete Event Simulation System. Varga, Andras. ESM 2001.
