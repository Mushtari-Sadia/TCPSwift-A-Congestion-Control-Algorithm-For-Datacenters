#! /usr/bin/python3
from cProfile import label
from re import L
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


for i in [40]:
    try :
        os.mkdir("nodes_"+str(i))
    except :
        print("directory exists")
    flow_vs_throughput = {}
    flow_vs_delay = {}
    flow_vs_pdr = {}
    flow_vs_plr = {}
    with open("nodes="+str(i)+",flow_summary.dat") as infile :
        for line in infile.read().splitlines():
            vals = line.split("\t")
            print(vals)
            flow_vs_throughput[vals[0]] = float(vals[1].replace("Kbps",""))
            flow_vs_delay[vals[0]] = vals[2];
            flow_vs_pdr[vals[0]] = vals[3];
            flow_vs_plr[vals[0]] = vals[4];
    
        # throughput
        with open("flow_vs_throughput.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Flow","Average Throughput(KBps)"])
            for param, values in flow_vs_throughput.items():
                csv_writer.writerow([param,values])
        df = pd.read_csv("flow_vs_throughput.csv")

        font['size']=15
        plt.scatter(df['Flow'], df["Average Throughput(KBps)"], color ='maroon',
                 label="nodes="+str(i))
        plt.plot(df['Flow'], df["Average Throughput(KBps)"], color ='maroon',
                 label="nodes="+str(i))
        plt.xticks(np.arange(10,90,10))
        plt.xlabel('Flow',fontdict=font)  # Add an x-label to the axes.
        plt.ylabel("Average Throughput(KBps)",fontdict=font)
        font['size']=18
        plt.title("Flow vs Average Throughput(KBps)",fontdict=font)
        
        plt.savefig("nodes_"+str(i)+"/Flow vs Average Throughput(KBps).jpg")
        plt.clf()
        if os.path.exists("flow_vs_throughput.csv"):
            os.remove("flow_vs_throughput.csv")

        #end to end delay
        with open("flow_vs_delay.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Flow","Delay"])
            for param, values in flow_vs_delay.items():
                csv_writer.writerow([param,values])
        df = pd.read_csv("flow_vs_delay.csv")

        font['size']=15
        plt.scatter(df['Flow'], df["Delay"], color ='maroon',
                 label="nodes="+str(i))
        plt.plot(df['Flow'], df["Delay"], color ='maroon',
                 label="nodes="+str(i))
        plt.xticks(np.arange(10,90,10))
        plt.xlabel('Flow',fontdict=font)  # Add an x-label to the axes.
        plt.ylabel("Delay",fontdict=font)
        font['size']=18
        plt.title("Flow vs Delay",fontdict=font)
        
        plt.savefig("nodes_"+str(i)+"/Flow vs Delay.jpg")
        plt.clf()
        if os.path.exists("flow_vs_delay.csv"):
            os.remove("flow_vs_delay.csv")

        #packet delivery ratio
        with open("flow_vs_pdr.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Flow","Packet Delivery Ratio"])
            for param, values in flow_vs_pdr.items():
                csv_writer.writerow([param,values])
        df = pd.read_csv("flow_vs_pdr.csv")

        font['size']=15
        plt.scatter(df['Flow'], df["Packet Delivery Ratio"], color ='maroon',
                 label="nodes="+str(i))
        plt.plot(df['Flow'], df["Packet Delivery Ratio"], color ='maroon',
                 label="nodes="+str(i))
        plt.xticks(np.arange(10,90,10))
        plt.xlabel('Flow',fontdict=font)  # Add an x-label to the axes.
        plt.ylabel("Packet Delivery Ratio",fontdict=font)
        font['size']=18
        plt.title("Flow vs Packet Delivery Ratio",fontdict=font)
        
        plt.savefig("nodes_"+str(i)+"/Flow vs Packet Delivery Ratio.jpg")
        plt.clf()
        if os.path.exists("flow_vs_pdr.csv"):
            os.remove("flow_vs_pdr.csv")
        
        #packet drop ratio
        with open("flow_vs_plr.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Flow","Packet Drop Ratio"])
            for param, values in flow_vs_plr.items():
                csv_writer.writerow([param,values])
        df = pd.read_csv("flow_vs_plr.csv")

        font['size']=15
        plt.scatter(df['Flow'], df["Packet Drop Ratio"], color ='maroon',
                 label="nodes="+str(i))
        plt.plot(df['Flow'], df["Packet Drop Ratio"], color ='maroon',
                 label="nodes="+str(i))
        plt.xticks(np.arange(10,90,10))
        plt.xlabel('Flow',fontdict=font)  # Add an x-label to the axes.
        plt.ylabel("Packet Drop Ratio",fontdict=font)
        
        font['size']=18
        plt.title("Flow vs Packet Drop Ratio",fontdict=font)
        
        plt.savefig("nodes_"+str(i)+"/Flow vs Packet Drop Ratio.jpg")
        plt.clf()
        if os.path.exists("flow_vs_plr.csv"):
            os.remove("flow_vs_plr.csv")





for i in [40]:
    try :
        os.mkdir("nodes_"+str(i))
    except :
        print("directory exists")
    dr_vs_throughput = {}
    dr_vs_delay = {}
    dr_vs_pdr = {}
    dr_vs_plr = {}
    with open("nodes="+str(i)+",dr_summary.dat") as infile :
        for line in infile.read().splitlines():
            vals = line.split("\t")
            print(vals)
            dr_vs_throughput[vals[0]] = float(vals[1].replace("Kbps",""))
            dr_vs_delay[vals[0]] = vals[2];
            dr_vs_pdr[vals[0]] = vals[3];
            dr_vs_plr[vals[0]] = vals[4];
    
        # throughput
        with open("dr_vs_throughput.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Packets Per Second","Average Throughput(KBps)"])
            for param, values in dr_vs_throughput.items():
                csv_writer.writerow([param,values])
        df = pd.read_csv("dr_vs_throughput.csv")

        font['size']=15
        plt.scatter(df['Packets Per Second'], df["Average Throughput(KBps)"], color ='maroon',
                 label="nodes="+str(i))
        plt.plot(df['Packets Per Second'], df["Average Throughput(KBps)"], color ='maroon',
                 label="nodes="+str(i))
        plt.xticks(np.arange(100,600,100))
        plt.xlabel('Packets Per Second',fontdict=font)  # Add an x-label to the axes.
        plt.ylabel("Average Throughput(KBps)",fontdict=font)
        font['size']=18
        plt.title("Packets Per Second vs Average Throughput(KBps)",fontdict=font)
        
        plt.savefig("nodes_"+str(i)+"/Packets Per Second vs Average Throughput(KBps).jpg")
        plt.clf()
        if os.path.exists("dr_vs_throughput.csv"):
            os.remove("dr_vs_throughput.csv")

        #end to end delay
        with open("dr_vs_delay.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Packets Per Second","Delay"])
            for param, values in dr_vs_delay.items():
                csv_writer.writerow([param,values])
        df = pd.read_csv("dr_vs_delay.csv")

        font['size']=15
        plt.scatter(df['Packets Per Second'], df["Delay"], color ='maroon',
                 label="nodes="+str(i))
        plt.plot(df['Packets Per Second'], df["Delay"], color ='maroon',
                 label="nodes="+str(i))
        plt.xticks(np.arange(100,600,100))
        plt.xlabel('Packets Per Second',fontdict=font)  # Add an x-label to the axes.
        plt.ylabel("Delay",fontdict=font)
        font['size']=18
        plt.title("Packets Per Second vs Delay",fontdict=font)
        
        plt.savefig("nodes_"+str(i)+"/Packets Per Second vs Delay.jpg")
        plt.clf()
        if os.path.exists("dr_vs_delay.csv"):
            os.remove("dr_vs_delay.csv")

        #packet delivery ratio
        with open("dr_vs_pdr.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Packets Per Second","Packet Delivery Ratio"])
            for param, values in dr_vs_pdr.items():
                csv_writer.writerow([param,values])
        df = pd.read_csv("dr_vs_pdr.csv")

        font['size']=15
        plt.scatter(df['Packets Per Second'], df["Packet Delivery Ratio"], color ='maroon',
                 label="nodes="+str(i))
        plt.plot(df['Packets Per Second'], df["Packet Delivery Ratio"], color ='maroon',
                 label="nodes="+str(i))
        plt.xticks(np.arange(100,600,100))
        plt.xlabel('Packets Per Second',fontdict=font)  # Add an x-label to the axes.
        plt.ylabel("Packet Delivery Ratio",fontdict=font)
        font['size']=18
        plt.title("Packets Per Second vs Packet Delivery Ratio",fontdict=font)
        
        plt.savefig("nodes_"+str(i)+"/Packets Per Second vs Packet Delivery Ratio.jpg")
        plt.clf()
        if os.path.exists("dr_vs_pdr.csv"):
            os.remove("dr_vs_pdr.csv")
        
        #packet drop ratio
        with open("dr_vs_plr.csv", "w") as outfile:
            csv_writer = csv.writer(outfile)
            csv_writer.writerow(["Packets Per Second","Packet Drop Ratio"])
            for param, values in dr_vs_plr.items():
                csv_writer.writerow([param,values])
        df = pd.read_csv("dr_vs_plr.csv")

        font['size']=15
        plt.scatter(df['Packets Per Second'], df["Packet Drop Ratio"], color ='maroon',
                 label="nodes="+str(i))
        plt.plot(df['Packets Per Second'], df["Packet Drop Ratio"], color ='maroon',
                 label="nodes="+str(i))
        plt.xticks(np.arange(100,600,100))
        plt.xlabel('Packets Per Second',fontdict=font)  # Add an x-label to the axes.
        plt.ylabel("Packet Drop Ratio",fontdict=font)
        
        font['size']=18
        plt.title("Packets Per Second vs Packet Drop Ratio",fontdict=font)
        
        plt.savefig("nodes_"+str(i)+"/Packets Per Second vs Packet Drop Ratio.jpg")
        plt.clf()
        if os.path.exists("dr_vs_plr.csv"):
            os.remove("dr_vs_plr.csv")








nodes_vs_throughput = {}
nodes_vs_delay = {}
nodes_vs_pdr = {}
nodes_vs_plr = {}

try :
    os.mkdir("flow=80")
except :
    print("directory exists")

for i in [20, 40, 60, 80, 100]:
    with open("nodes="+str(i)+",flow_summary.dat") as infile :
        for line in infile.read().splitlines():
            vals = line.split("\t")
            if vals[0]=="80":
                nodes_vs_throughput[i] = float(vals[1].replace("Kbps",""))
                nodes_vs_delay[i] = vals[2];
                nodes_vs_pdr[i] = vals[3];
                nodes_vs_plr[i] = vals[4];
                break
    
# throughput
with open("nodes_vs_throughput.csv", "w") as outfile:
    csv_writer = csv.writer(outfile)
    csv_writer.writerow(["nodes","Average Throughput(KBps)"])
    for param, values in nodes_vs_throughput.items():
        csv_writer.writerow([param,values])
df = pd.read_csv("nodes_vs_throughput.csv")

font['size']=15
plt.scatter(df['nodes'], df["Average Throughput(KBps)"], color ='maroon')
plt.plot(df['nodes'], df["Average Throughput(KBps)"], color ='maroon')
plt.xticks(np.arange(20,110,20))
plt.xlabel('Nodes',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Average Throughput(KBps)",fontdict=font)
font['size']=18
plt.title("Nodes vs Average Throughput(KBps)",fontdict=font)

plt.savefig("flow=80"+"/nodes vs Average Throughput(KBps).jpg")
plt.clf()
if os.path.exists("nodes_vs_throughput.csv"):
    os.remove("nodes_vs_throughput.csv")

#end to end delay
with open("nodes_vs_delay.csv", "w") as outfile:
    csv_writer = csv.writer(outfile)
    csv_writer.writerow(["nodes","Delay"])
    for param, values in nodes_vs_delay.items():
        csv_writer.writerow([param,values])
df = pd.read_csv("nodes_vs_delay.csv")

font['size']=15
plt.scatter(df['nodes'], df["Delay"], color ='maroon')
plt.plot(df['nodes'], df["Delay"], color ='maroon')
plt.xticks(np.arange(20,110,20))
plt.xlabel('Nodes',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Delay",fontdict=font)
font['size']=18
plt.title("Nodes vs Delay",fontdict=font)

plt.savefig("flow=80"+"/nodes vs Delay.jpg")
plt.clf()
if os.path.exists("nodes_vs_delay.csv"):
    os.remove("nodes_vs_delay.csv")

#packet delivery ratio
with open("nodes_vs_pdr.csv", "w") as outfile:
    csv_writer = csv.writer(outfile)
    csv_writer.writerow(["nodes","Packet Delivery Ratio"])
    for param, values in nodes_vs_pdr.items():
        csv_writer.writerow([param,values])
df = pd.read_csv("nodes_vs_pdr.csv")

font['size']=15
plt.scatter(df['nodes'], df["Packet Delivery Ratio"], color ='maroon')
plt.plot(df['nodes'], df["Packet Delivery Ratio"], color ='maroon')
plt.xticks(np.arange(20,110,20))
plt.xlabel('Nodes',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Packet Delivery Ratio",fontdict=font)
font['size']=18
plt.title("Nodes vs Packet Delivery Ratio",fontdict=font)

plt.savefig("flow=80"+"/nodes vs Packet Delivery Ratio.jpg")
plt.clf()
if os.path.exists("nodes_vs_pdr.csv"):
    os.remove("nodes_vs_pdr.csv")

#packet drop ratio
with open("nodes_vs_plr.csv", "w") as outfile:
    csv_writer = csv.writer(outfile)
    csv_writer.writerow(["nodes","Packet Drop Ratio"])
    for param, values in nodes_vs_plr.items():
        csv_writer.writerow([param,values])
df = pd.read_csv("nodes_vs_plr.csv")

font['size']=15
plt.scatter(df['nodes'], df["Packet Drop Ratio"], color ='maroon')
plt.plot(df['nodes'], df["Packet Drop Ratio"], color ='maroon')
plt.xticks(np.arange(20,110,20))
plt.xlabel('Nodes',fontdict=font)  # Add an x-label to the axes.
plt.ylabel("Packet Drop Ratio",fontdict=font)

font['size']=18
plt.title("Nodes vs Packet Drop Ratio",fontdict=font)

plt.savefig("flow=80"+"/nodes vs Packet Drop Ratio.jpg")
plt.clf()
if os.path.exists("nodes_vs_plr.csv"):
    os.remove("nodes_vs_plr.csv")
            
        
            
        

