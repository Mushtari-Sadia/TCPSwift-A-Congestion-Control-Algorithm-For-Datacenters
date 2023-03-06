# ! /usr/bin/python3
# from cProfile import label
from re import L
# from turtle import color
# from tkinter import Label
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import csv
import seaborn as sns
import os

sns.set()
font = {'color':  'black',
        'weight': 'bold',
        'size': 15,
        }

algos = ["Westwood","NewReno"]
t_dfs,del_dfs,pdr_dfs,plr_dfs = [],[],[],[]
try :
    os.mkdir("nodes_40")
except :
    print("directory exists")

for algo in algos :
    flow_vs_throughput = {}
    flow_vs_delay = {}
    flow_vs_pdr = {}
    flow_vs_plr = {}
    with open(algo+"/nodes=40,flow_summary.dat") as infile :
        for line in infile.read().splitlines():
            vals = line.split("\t")
            print(vals)
            flow_vs_throughput[vals[0]] = float(vals[1].replace("Kbps",""))
            # times = vals[2].replace("e","").replace("ns","").replace("+","",1).split("+")
            # print(times)
            # if 'e' in vals[2] :
            #     times = vals[2].replace("e","").replace("ns","").replace("+","",1).split("+")
            #     # print(times)
            # else :
            #     times = vals[2].replace("e","").replace("ns","").split("+")
            #     # print(times)
            # if times[0]=='' :
            #     times[0] = '0'
            # flow_vs_delay[vals[0]] = float(times[0])*10**float((times[1]))
            flow_vs_delay[vals[0]] = float(vals[2].replace("ns",""))
            flow_vs_pdr[vals[0]] = vals[3]
            flow_vs_plr[vals[0]] = vals[4]

        # throughput
        with open(algo+"flow_vs_throughput.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Flow","Average Throughput(KBps)"])
            for param, values in flow_vs_throughput.items():
                csv_writer.writerow([param,values])
        csv_name = algo+"flow_vs_throughput.csv"
        t_dfs.append(pd.read_csv(csv_name))
        if os.path.exists(csv_name):
            os.remove(csv_name)

        #delay
        with open(algo+"flow_vs_delay.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Flow","Delay"])
            for param, values in flow_vs_delay.items():
                csv_writer.writerow([param,values])
        csv_name = algo+"flow_vs_delay.csv"
        del_dfs.append(pd.read_csv(csv_name))
        if os.path.exists(csv_name):
            os.remove(csv_name)
        
        #pdr
        with open(algo+"flow_vs_pdr.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Flow","Packet Delivery Ratio"])
            for param, values in flow_vs_pdr.items():
                csv_writer.writerow([param,values])
        csv_name = algo+"flow_vs_pdr.csv"
        pdr_dfs.append(pd.read_csv(csv_name))
        if os.path.exists(csv_name):
            os.remove(csv_name)
        #plr
        with open(algo+"flow_vs_plr.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Flow","Packet Drop Ratio"])
            for param, values in flow_vs_plr.items():
                csv_writer.writerow([param,values])
        csv_name = algo+"flow_vs_plr.csv"
        plr_dfs.append(pd.read_csv(csv_name))
        if os.path.exists(csv_name):
            os.remove(csv_name)

#tput
font['size']=15
plt.scatter(t_dfs[0]['Flow'], t_dfs[0]["Average Throughput(KBps)"], color ='maroon')
plt.plot(t_dfs[0]['Flow'], t_dfs[0]["Average Throughput(KBps)"], color ='maroon',
            label=algos[0])
plt.scatter(t_dfs[1]['Flow'], t_dfs[1]["Average Throughput(KBps)"], color ='blue')
plt.plot(t_dfs[1]['Flow'], t_dfs[1]["Average Throughput(KBps)"], color ='blue',
            label=algos[1])
plt.xticks(np.arange(10,90,10))
plt.xlabel('Flow',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Average Throughput(KBps)",fontdict=font)
font['size']=18
plt.title("Flow vs Average Throughput(KBps)",fontdict=font)

plt.legend()
plt.savefig("nodes_40/Flow vs Average Throughput(KBps).jpg")
plt.clf()
#del
font['size']=15
plt.scatter(del_dfs[0]['Flow'], del_dfs[0]["Delay"], color ='maroon')
plt.plot(del_dfs[0]['Flow'], del_dfs[0]["Delay"], color ='maroon',
            label=algos[0])
plt.scatter(del_dfs[1]['Flow'], del_dfs[1]["Delay"], color ='blue')
plt.plot(del_dfs[1]['Flow'], del_dfs[1]["Delay"], color ='blue',
            label=algos[1])
plt.xticks(np.arange(10,90,10))
plt.xlabel('Flow',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Delay (ns)",fontdict=font)
font['size']=18
plt.title("Flow vs Delay",fontdict=font)

plt.legend()
plt.savefig("nodes_40/Flow vs Delay.jpg")
plt.clf()
#pdr
font['size']=15
plt.scatter(pdr_dfs[0]['Flow'], pdr_dfs[0]["Packet Delivery Ratio"], color ='maroon')
plt.plot(pdr_dfs[0]['Flow'], pdr_dfs[0]["Packet Delivery Ratio"], color ='maroon',
            label=algos[0])
plt.scatter(pdr_dfs[1]['Flow'], pdr_dfs[1]["Packet Delivery Ratio"], color ='blue')
plt.plot(pdr_dfs[1]['Flow'], pdr_dfs[1]["Packet Delivery Ratio"], color ='blue',
            label=algos[1])
plt.xticks(np.arange(10,90,10))
plt.xlabel('Flow',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Packet Delivery Ratio",fontdict=font)
font['size']=18
plt.title("Flow vs Packet Delivery Ratio",fontdict=font)

plt.legend()
plt.savefig("nodes_40/Flow vs Packet Delivery Ratio.jpg")
plt.clf()
#plr
font['size']=15
plt.scatter(plr_dfs[0]['Flow'], plr_dfs[0]["Packet Drop Ratio"], color ='maroon')
plt.plot(plr_dfs[0]['Flow'], plr_dfs[0]["Packet Drop Ratio"], color ='maroon',
            label=algos[0])
plt.scatter(plr_dfs[1]['Flow'], plr_dfs[1]["Packet Drop Ratio"], color ='blue')
plt.plot(plr_dfs[1]['Flow'], plr_dfs[1]["Packet Drop Ratio"], color ='blue',
            label=algos[1])
plt.xticks(np.arange(10,90,10))
plt.xlabel('Flow',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Packet Drop Ratio",fontdict=font)
font['size']=18
plt.title("Flow vs Packet Drop Ratio",fontdict=font)

plt.legend()
plt.savefig("nodes_40/Flow vs Packet Drop Ratio.jpg")
plt.clf()


# packets per second
t_dfs,del_dfs,pdr_dfs,plr_dfs = [],[],[],[]
try :
    os.mkdir("nodes_40")
except :
    print("directory exists")

for algo in algos :
    dr_vs_throughput = {}
    dr_vs_delay = {}
    dr_vs_pdr = {}
    dr_vs_plr = {}
    with open(algo+"/nodes=40,dr_summary.dat") as infile :
        for line in infile.read().splitlines():
            vals = line.split("\t")
            print(vals)
            dr_vs_throughput[vals[0]] = float(vals[1].replace("Kbps",""))
            # if 'e' in vals[2] :
            #     times = vals[2].replace("e","").replace("ns","").replace("+","",1).split("+")
            #     # print(times)
            # else :
            #     times = vals[2].replace("e","").replace("ns","").split("+")
            #     # print(times)
            # if times[0]=='' :
            #     times[0] = '0'
            # dr_vs_delay[vals[0]] = float(times[0])*10**float((times[1]))
            dr_vs_delay[vals[0]] = float(vals[2].replace("ns",""));
            dr_vs_pdr[vals[0]] = vals[3];
            dr_vs_plr[vals[0]] = vals[4];

        # throughput
        with open(algo+"dr_vs_throughput.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Packets Per Second","Average Throughput(KBps)"])
            for param, values in dr_vs_throughput.items():
                csv_writer.writerow([param,values])
        csv_name = algo+"dr_vs_throughput.csv"
        t_dfs.append(pd.read_csv(csv_name))
        if os.path.exists(csv_name):
            os.remove(csv_name)

        #delay
        with open(algo+"dr_vs_delay.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Packets Per Second","Delay"])
            for param, values in dr_vs_delay.items():
                csv_writer.writerow([param,values])
        csv_name = algo+"dr_vs_delay.csv"
        del_dfs.append(pd.read_csv(csv_name))
        if os.path.exists(csv_name):
            os.remove(csv_name)
        
        #pdr
        with open(algo+"dr_vs_pdr.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Packets Per Second","Packet Delivery Ratio"])
            for param, values in dr_vs_pdr.items():
                csv_writer.writerow([param,values])
        csv_name = algo+"dr_vs_pdr.csv"
        pdr_dfs.append(pd.read_csv(csv_name))
        if os.path.exists(csv_name):
            os.remove(csv_name)
        #plr
        with open(algo+"dr_vs_plr.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Packets Per Second","Packet Drop Ratio"])
            for param, values in dr_vs_plr.items():
                csv_writer.writerow([param,values])
        csv_name = algo+"dr_vs_plr.csv"
        plr_dfs.append(pd.read_csv(csv_name))
        if os.path.exists(csv_name):
            os.remove(csv_name)

#tput
font['size']=15
plt.scatter(t_dfs[0]['Packets Per Second'], t_dfs[0]["Average Throughput(KBps)"], color ='maroon')
plt.plot(t_dfs[0]['Packets Per Second'], t_dfs[0]["Average Throughput(KBps)"], color ='maroon',
            label=algos[0])
plt.scatter(t_dfs[1]['Packets Per Second'], t_dfs[1]["Average Throughput(KBps)"], color ='blue')
plt.plot(t_dfs[1]['Packets Per Second'], t_dfs[1]["Average Throughput(KBps)"], color ='blue',
            label=algos[1])
plt.xticks(np.arange(100,600,100))
plt.xlabel('Packets Per Second',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Average Throughput(KBps)",fontdict=font)
font['size']=18
plt.title("PPS vs Average Throughput(KBps)",fontdict=font)

plt.legend()
plt.savefig("nodes_40/Packets Per Second vs Average Throughput(KBps).jpg")
plt.clf()
#del
font['size']=15
plt.scatter(del_dfs[0]['Packets Per Second'], del_dfs[0]["Delay"], color ='maroon')
plt.plot(del_dfs[0]['Packets Per Second'], del_dfs[0]["Delay"], color ='maroon',
            label=algos[0])
plt.scatter(del_dfs[1]['Packets Per Second'], del_dfs[1]["Delay"], color ='blue')
plt.plot(del_dfs[1]['Packets Per Second'], del_dfs[1]["Delay"], color ='blue',
            label=algos[1])
plt.xticks(np.arange(100,600,100))
plt.xlabel('Packets Per Second',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Delay (ns)",fontdict=font)
font['size']=18
plt.title("PPS vs Delay",fontdict=font)

plt.legend()
plt.savefig("nodes_40/Packets Per Second vs Delay.jpg")
plt.clf()
#pdr
font['size']=15
plt.scatter(pdr_dfs[0]['Packets Per Second'], pdr_dfs[0]["Packet Delivery Ratio"], color ='maroon')
plt.plot(pdr_dfs[0]['Packets Per Second'], pdr_dfs[0]["Packet Delivery Ratio"], color ='maroon',
            label=algos[0])
plt.scatter(pdr_dfs[1]['Packets Per Second'], pdr_dfs[1]["Packet Delivery Ratio"], color ='blue')
plt.plot(pdr_dfs[1]['Packets Per Second'], pdr_dfs[1]["Packet Delivery Ratio"], color ='blue',
            label=algos[1])
plt.xticks(np.arange(100,600,100))
plt.xlabel('Packets Per Second',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Packet Delivery Ratio",fontdict=font)
font['size']=18
plt.title("PPS vs Packet Delivery Ratio",fontdict=font)

plt.legend()
plt.savefig("nodes_40/Packets Per Second vs Packet Delivery Ratio.jpg")
plt.clf()
#plr
font['size']=15
plt.scatter(plr_dfs[0]['Packets Per Second'], plr_dfs[0]["Packet Drop Ratio"], color ='maroon')
plt.plot(plr_dfs[0]['Packets Per Second'], plr_dfs[0]["Packet Drop Ratio"], color ='maroon',
            label=algos[0])
plt.scatter(plr_dfs[1]['Packets Per Second'], plr_dfs[1]["Packet Drop Ratio"], color ='blue')
plt.plot(plr_dfs[1]['Packets Per Second'], plr_dfs[1]["Packet Drop Ratio"], color ='blue',
            label=algos[1])
plt.xticks(np.arange(100,600,100))
plt.xlabel('Packets Per Second',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Packet Drop Ratio",fontdict=font)
font['size']=18
plt.title("PPS vs Packet Drop Ratio",fontdict=font)

plt.legend()
plt.savefig("nodes_40/Packets Per Second vs Packet Drop Ratio.jpg")
plt.clf()


# Tx range
t_dfs,del_dfs,pdr_dfs,plr_dfs = [],[],[],[]
try :
    os.mkdir("nodes_40")
except :
    print("directory exists")

for algo in algos :
    dr_vs_throughput = {}
    dr_vs_delay = {}
    dr_vs_pdr = {}
    dr_vs_plr = {}
    with open(algo+"/nodes=40,cx_summary.dat") as infile :
        for line in infile.read().splitlines():
            vals = line.split("\t")
            print(vals)
            dr_vs_throughput[vals[0]] = float(vals[1].replace("Kbps",""))
            # if 'e' in vals[2] :
            #     times = vals[2].replace("e","").replace("ns","").replace("+","",1).split("+")
            #     # print(times)
            # else :
            #     times = vals[2].replace("e","").replace("ns","").split("+")
            #     # print(times)
            # if times[0]=='' :
            #     times[0] = '0'
            # dr_vs_delay[vals[0]] = float(times[0])*10**float((times[1]))
            dr_vs_delay[vals[0]] = float(vals[2].replace("ns",""));
            dr_vs_pdr[vals[0]] = vals[3];
            dr_vs_plr[vals[0]] = vals[4];

        # throughput
        with open(algo+"dr_vs_throughput.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Coverage Area","Average Throughput(KBps)"])
            for param, values in dr_vs_throughput.items():
                csv_writer.writerow([param,values])
        csv_name = algo+"dr_vs_throughput.csv"
        t_dfs.append(pd.read_csv(csv_name))
        if os.path.exists(csv_name):
            os.remove(csv_name)

        #delay
        with open(algo+"dr_vs_delay.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Coverage Area","Delay"])
            for param, values in dr_vs_delay.items():
                csv_writer.writerow([param,values])
        csv_name = algo+"dr_vs_delay.csv"
        del_dfs.append(pd.read_csv(csv_name))
        if os.path.exists(csv_name):
            os.remove(csv_name)
        
        #pdr
        with open(algo+"dr_vs_pdr.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Coverage Area","Packet Delivery Ratio"])
            for param, values in dr_vs_pdr.items():
                csv_writer.writerow([param,values])
        csv_name = algo+"dr_vs_pdr.csv"
        pdr_dfs.append(pd.read_csv(csv_name))
        if os.path.exists(csv_name):
            os.remove(csv_name)
        #plr
        with open(algo+"dr_vs_plr.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Coverage Area","Packet Drop Ratio"])
            for param, values in dr_vs_plr.items():
                csv_writer.writerow([param,values])
        csv_name = algo+"dr_vs_plr.csv"
        plr_dfs.append(pd.read_csv(csv_name))
        if os.path.exists(csv_name):
            os.remove(csv_name)

#tput
font['size']=15
plt.scatter(t_dfs[0]['Coverage Area'], t_dfs[0]["Average Throughput(KBps)"], color ='maroon')
plt.plot(t_dfs[0]['Coverage Area'], t_dfs[0]["Average Throughput(KBps)"], color ='maroon',
            label=algos[0])
plt.scatter(t_dfs[1]['Coverage Area'], t_dfs[1]["Average Throughput(KBps)"], color ='blue')
plt.plot(t_dfs[1]['Coverage Area'], t_dfs[1]["Average Throughput(KBps)"], color ='blue',
            label=algos[1])
plt.scatter(80,0,color='orange',label='Max Range')
plt.xticks(np.arange(20,120,20))
plt.xlabel('Coverage Area',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Average Throughput(KBps)",fontdict=font)
font['size']=18
plt.title("Coverage Area vs Average Throughput(KBps)",fontdict=font)

plt.legend()
plt.savefig("nodes_40/Coverage Area vs Average Throughput(KBps).jpg")
plt.clf()
#del
font['size']=15
plt.scatter(del_dfs[0]['Coverage Area'], del_dfs[0]["Delay"], color ='maroon')
plt.plot(del_dfs[0]['Coverage Area'], del_dfs[0]["Delay"], color ='maroon',
            label=algos[0])
plt.scatter(del_dfs[1]['Coverage Area'], del_dfs[1]["Delay"], color ='blue')
plt.plot(del_dfs[1]['Coverage Area'], del_dfs[1]["Delay"], color ='blue',
            label=algos[1])
plt.scatter(80,0,color='orange',label='Max Range')
plt.xticks(np.arange(20,120,20))
plt.xlabel('Coverage Area',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Delay (ns)",fontdict=font)
font['size']=18
plt.title("Coverage Area vs Delay",fontdict=font)

plt.legend()
plt.savefig("nodes_40/Coverage Area vs Delay.jpg")
plt.clf()
#pdr
font['size']=15
plt.scatter(pdr_dfs[0]['Coverage Area'], pdr_dfs[0]["Packet Delivery Ratio"], color ='maroon')
plt.plot(pdr_dfs[0]['Coverage Area'], pdr_dfs[0]["Packet Delivery Ratio"], color ='maroon',
            label=algos[0])
plt.scatter(pdr_dfs[1]['Coverage Area'], pdr_dfs[1]["Packet Delivery Ratio"], color ='blue')
plt.plot(pdr_dfs[1]['Coverage Area'], pdr_dfs[1]["Packet Delivery Ratio"], color ='blue',
            label=algos[1])
plt.scatter(80,0,color='orange',label='Max Range')
plt.xticks(np.arange(20,120,20))
plt.xlabel('Coverage Area',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Packet Delivery Ratio",fontdict=font)
font['size']=18
plt.title("Coverage Area vs Packet Delivery Ratio",fontdict=font)

plt.legend()
plt.savefig("nodes_40/Coverage Area vs Packet Delivery Ratio.jpg")
plt.clf()
#plr
font['size']=15
plt.scatter(plr_dfs[0]['Coverage Area'], plr_dfs[0]["Packet Drop Ratio"], color ='maroon')
plt.plot(plr_dfs[0]['Coverage Area'], plr_dfs[0]["Packet Drop Ratio"], color ='maroon',
            label=algos[0])
plt.scatter(plr_dfs[1]['Coverage Area'], plr_dfs[1]["Packet Drop Ratio"], color ='blue')
plt.plot(plr_dfs[1]['Coverage Area'], plr_dfs[1]["Packet Drop Ratio"], color ='blue',
            label=algos[1])
plt.scatter(80,0,color='orange',label='Max Range')
plt.xticks(np.arange(20,120,20))
plt.xlabel('Coverage Area',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Packet Drop Ratio",fontdict=font)
font['size']=18
plt.title("Coverage Area vs Packet Drop Ratio",fontdict=font)

plt.legend()
plt.savefig("nodes_40/Coverage Area vs Packet Drop Ratio.jpg")
plt.clf()


t_dfs,del_dfs,pdr_dfs,plr_dfs = [],[],[],[]

for algo in algos :
    nodes_vs_throughput = {}
    nodes_vs_delay = {}
    nodes_vs_pdr = {}
    nodes_vs_plr = {}
    try :
        os.mkdir("flow=80")
    except :
        print("directory exists")

    for i in [20, 40, 60, 80, 100]:
        with open(algo+"/nodes="+str(i)+",flow_summary.dat") as infile :
            for line in infile.read().splitlines():
                vals = line.split("\t")
                if vals[0]=="80":
                    nodes_vs_throughput[i] = float(vals[1].replace("Kbps",""))
                    # if 'e' in vals[2] :
                    #     times = vals[2].replace("e","").replace("ns","").replace("+","",1).split("+")
                    #     # print(times)
                    # else :
                    #     times = vals[2].replace("e","").replace("ns","").split("+")
                    #     # print(times)
                    # if times[0]=='' :
                    #     times[0] = '0'
                    # nodes_vs_delay[i] = float(times[0])*10**float((times[1]))
                    nodes_vs_delay[i] = float(vals[2].replace("ns",""))
                    nodes_vs_pdr[i] = vals[3];
                    nodes_vs_plr[i] = vals[4];
                    break
        
    # throughput
    with open("nodes_vs_throughput.csv", "w") as outfile:
        csv_writer = csv.writer(outfile)
        csv_writer.writerow(["nodes","Average Throughput(KBps)"])
        for param, values in nodes_vs_throughput.items():
            csv_writer.writerow([param,values])
    t_dfs.append(pd.read_csv("nodes_vs_throughput.csv"))

    #end to end delay
    with open("nodes_vs_delay.csv", "w") as outfile:
        csv_writer = csv.writer(outfile)
        csv_writer.writerow(["nodes","Delay"])
        for param, values in nodes_vs_delay.items():
            csv_writer.writerow([param,values])
    del_dfs.append(pd.read_csv("nodes_vs_delay.csv"))

    #packet delivery ratio
    with open("nodes_vs_pdr.csv", "w") as outfile:
        csv_writer = csv.writer(outfile)
        csv_writer.writerow(["nodes","Packet Delivery Ratio"])
        for param, values in nodes_vs_pdr.items():
            csv_writer.writerow([param,values])
    pdr_dfs.append(pd.read_csv("nodes_vs_pdr.csv"))

    #packet drop ratio
    with open("nodes_vs_plr.csv", "w") as outfile:
        csv_writer = csv.writer(outfile)
        csv_writer.writerow(["nodes","Packet Drop Ratio"])
        for param, values in nodes_vs_plr.items():
            csv_writer.writerow([param,values])
    plr_dfs.append(pd.read_csv("nodes_vs_plr.csv"))

font['size']=15
plt.scatter(t_dfs[0]['nodes'], t_dfs[0]["Average Throughput(KBps)"], color ='maroon',label=algos[0])
plt.plot(t_dfs[0]['nodes'], t_dfs[0]["Average Throughput(KBps)"], color ='maroon')
plt.scatter(t_dfs[1]['nodes'], t_dfs[1]["Average Throughput(KBps)"], color ='blue',label=algos[1])
plt.plot(t_dfs[1]['nodes'], t_dfs[1]["Average Throughput(KBps)"], color ='blue')

plt.xticks(np.arange(20,110,20))
plt.xlabel('Nodes',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Average Throughput(KBps)",fontdict=font)
font['size']=18
plt.title("Nodes vs Average Throughput(KBps)",fontdict=font)
plt.legend()
plt.savefig("flow=80"+"/nodes vs Average Throughput(KBps).jpg")
plt.clf()
if os.path.exists("nodes_vs_throughput.csv"):
    os.remove("nodes_vs_throughput.csv")



font['size']=15
plt.scatter(del_dfs[0]['nodes'], del_dfs[0]["Delay"], color ='maroon',label=algos[0])
plt.plot(del_dfs[0]['nodes'], del_dfs[0]["Delay"], color ='maroon')
plt.scatter(del_dfs[1]['nodes'], del_dfs[1]["Delay"], color ='blue',label=algos[1])
plt.plot(del_dfs[1]['nodes'], del_dfs[1]["Delay"], color ='blue')

plt.xticks(np.arange(20,110,20))
plt.xlabel('Nodes',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Delay (ns)",fontdict=font)
font['size']=18
plt.title("Nodes vs Delay",fontdict=font)
plt.legend()
plt.savefig("flow=80"+"/nodes vs Delay.jpg")
plt.clf()
if os.path.exists("nodes_vs_delay.csv"):
    os.remove("nodes_vs_delay.csv")



font['size']=15
font['size']=15
plt.scatter(pdr_dfs[0]['nodes'], pdr_dfs[0]["Packet Delivery Ratio"], color ='maroon',label=algos[0])
plt.plot(pdr_dfs[0]['nodes'], pdr_dfs[0]["Packet Delivery Ratio"], color ='maroon')
plt.scatter(pdr_dfs[1]['nodes'], pdr_dfs[1]["Packet Delivery Ratio"], color ='blue',label=algos[1])
plt.plot(pdr_dfs[1]['nodes'], pdr_dfs[1]["Packet Delivery Ratio"], color ='blue')

plt.xticks(np.arange(20,110,20))
plt.xlabel('Nodes',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Packet Delivery Ratio",fontdict=font)
font['size']=18
plt.title("Nodes vs Packet Delivery Ratio",fontdict=font)
plt.legend()
plt.savefig("flow=80"+"/nodes vs Packet Delivery Ratio.jpg")
plt.clf()
if os.path.exists("nodes_vs_pdr.csv"):
    os.remove("nodes_vs_pdr.csv")



font['size']=15
plt.scatter(plr_dfs[0]['nodes'], plr_dfs[0]["Packet Drop Ratio"], color ='maroon',label=algos[0])
plt.plot(plr_dfs[0]['nodes'], plr_dfs[0]["Packet Drop Ratio"], color ='maroon')
plt.scatter(plr_dfs[1]['nodes'], plr_dfs[1]["Packet Drop Ratio"], color ='blue',label=algos[1])
plt.plot(plr_dfs[1]['nodes'], plr_dfs[1]["Packet Drop Ratio"], color ='blue')

plt.xticks(np.arange(20,110,20))
plt.xlabel('Nodes',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Packet Drop Ratio",fontdict=font)

font['size']=18
plt.title("Nodes vs Packet Drop Ratio",fontdict=font)
plt.legend()
plt.savefig("flow=80"+"/nodes vs Packet Drop Ratio.jpg")
plt.clf()
if os.path.exists("nodes_vs_plr.csv"):
    os.remove("nodes_vs_plr.csv")
            