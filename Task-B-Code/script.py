#! /usr/bin/python3
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
try :
    os.mkdir("tcp_plots")
except :
    print("directory exists")

try :
    os.mkdir("flow_plots")
except :
    print("directory exists")
algo = input("input your algo:\n")
tcp_params = ['cwnd','rto','rtt','ssth']
full_names = ['Congestion Window','Retransmit Timeout','Round Trip Time','Slow Start Threshold']
idx = 0
for i in tcp_params :
    with open("-"+i+".dat") as infile, open(i+".csv", "w") as outfile:
        csv_writer = csv.writer(outfile)
        prev = ''
        csv_writer.writerow(['Time', i])
        for line in infile.read().splitlines():
            vals = line.split(" ")
            csv_writer.writerow([vals[0],vals[1]])

    df = pd.read_csv(i+".csv")
    fig, ax = plt.subplots(figsize=(12,6), layout='constrained')
    ax.plot(df['Time'], df[i], label=algo,color='red', linewidth=3)  # Plot some data on the axes.
    ax.set_xlabel('Time',fontdict=font)  # Add an x-label to the axes.
    ax.set_ylabel(full_names[idx],fontdict=font)
    font['size']=18
    ax.set_title("Time vs "+full_names[idx],fontdict=font)
    ax.grid(True)
    ax.legend();

    fig.savefig("tcp_plots/"+i+".jpg")
    if os.path.exists(i+".csv"):
        os.remove(i+".csv")
    idx+=1

plt.clf()

with open("flowSummary.dat") as infile :
    flow = infile.read().split("----")

Parameters = {
    "Sent Packets" : [],
    "Received Packets " : [],
"Lost Packets " : [],
"Packet delivery ratio " : [],
"Packet loss ratio ": [],
"Delay ": [],
"Jitter ": [],
"Throughput ": [],
}
for i in flow :
        if(i.split(" ")[0]=="Flow") :
            lines = i.rstrip().split("\n")
            for line in lines :
                line = line.rstrip()
                try :
                    Parameters[line.split("=")[0]].append(line.split("=")[1].replace('Kbps','').replace('%',''))
                except :
                    print()

new_key = "Throughput (Kbps)"
old_key = "Throughput "
Parameters[new_key] = Parameters. pop(old_key)


for param, values in Parameters.items():
    k=1
    with open(param+".csv", "w") as outfile:
        csv_writer = csv.writer(outfile)
        csv_writer.writerow(['Flow', param])
        for val in values :
            csv_writer.writerow([k,val])
            k+=1
    df = pd.read_csv(param+".csv")

    font['size']=15
    plt.bar(df['Flow'], df[param], color ='maroon',
            width = 0.4)
    plt.xticks(np.arange(1,21,1))
    plt.xlabel('Flow',fontdict=font)  # Add an x-label to the axes.
    plt.ylabel(param,fontdict=font)
    font['size']=18
    plt.title("Flow vs "+param,fontdict=font)


    plt.savefig("flow_plots/"+param+".jpg")
    plt.clf()
    if os.path.exists(param+".csv"):
        os.remove(param+".csv")