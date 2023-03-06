# !/bin/bash

# varying nodes 20, 40, 60, 80, and 100
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=20 --num_flows=40 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=40 --num_flows=40 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=60 --num_flows=40 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=80 --num_flows=40 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=100 --num_flows=40 --packets_per_second=100"

# varying flow 10,20,30,40,60,80
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=20 --num_flows=10 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=20 --num_flows=20 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=20 --num_flows=30 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=20 --num_flows=40 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=20 --num_flows=60 --packets_per_second=100"

# varying packets per second 100, 200, 300, 400, and 500
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=20 --num_flows=40 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=20 --num_flows=40 --packets_per_second=200"
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=20 --num_flows=40 --packets_per_second=300"
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=20 --num_flows=40 --packets_per_second=400"
./waf --run "scratch/task-b.cc --transport_prot="TcpDctcp" --num_nodes=20 --num_flows=40 --packets_per_second=500"

# varying nodes 20, 40, 60, 80, and 100
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=20 --num_flows=40 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=40 --num_flows=40 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=60 --num_flows=40 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=80 --num_flows=40 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=100 --num_flows=40 --packets_per_second=100"


# varying flow 10,20,30,40,60,80
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=20 --num_flows=10 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=20 --num_flows=20 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=20 --num_flows=30 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=20 --num_flows=40 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=20 --num_flows=60 --packets_per_second=100"

# varying packets per second 100, 200, 300, 100, and 500
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=20 --num_flows=40 --packets_per_second=100"
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=20 --num_flows=40 --packets_per_second=200"
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=20 --num_flows=40 --packets_per_second=300"
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=20 --num_flows=40 --packets_per_second=400"
./waf --run "scratch/task-b.cc --transport_prot="TcpSwift" --num_nodes=20 --num_flows=40 --packets_per_second=500"

