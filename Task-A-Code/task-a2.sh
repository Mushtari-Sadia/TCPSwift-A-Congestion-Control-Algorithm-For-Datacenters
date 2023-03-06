# !/bin/bash

# varying nodes 20, 40, 60, 80, and 100
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=20 --num_flows=80 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=80 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=60 --num_flows=80 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=80 --num_flows=80 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=100 --num_flows=80 --packets_per_second=400"


# varying flow 10,20,30,40,60,80
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=10 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=20 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=30 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=40 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=60 --packets_per_second=400"

# varying packets per second 100, 200, 300, 400, and 500
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=80 --packets_per_second=100"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=80 --packets_per_second=200"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=80 --packets_per_second=300"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=80 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=80 --packets_per_second=500"

# varying coverage area Tx_range, 2 x Tx_range, 3 x Tx_range, 4 x Tx_range, and 5 x Tx_range)
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=80 --packets_per_second=400 --coverage_area=1"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=80 --packets_per_second=400 --coverage_area=2"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=80 --packets_per_second=400 --coverage_area=3"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=80 --packets_per_second=400 --coverage_area=4"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpWestwood" --num_nodes=40 --num_flows=80 --packets_per_second=400 --coverage_area=5"


# varying nodes 20, 40, 60, 80, and 100
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=20 --num_flows=80 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=80 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=60 --num_flows=80 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=80 --num_flows=80 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=100 --num_flows=80 --packets_per_second=400"


# varying flow 10,20,30,40,60,80
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=10 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=20 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=30 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=40 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=60 --packets_per_second=400"

# varying packets per second 100, 200, 300, 400, and 500
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=80 --packets_per_second=100"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=80 --packets_per_second=200"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=80 --packets_per_second=300"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=80 --packets_per_second=400"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=80 --packets_per_second=500"


# varying coverage area Tx_range, 2 x Tx_range, 3 x Tx_range, 4 x Tx_range, and 5 x Tx_range)
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=80 --packets_per_second=400 --coverage_area=1"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=80 --packets_per_second=400 --coverage_area=2"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=80 --packets_per_second=400 --coverage_area=3"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=80 --packets_per_second=400 --coverage_area=4"
./waf --run "scratch/task-a-wireless-2.cc --transport_prot="TcpNewReno" --num_nodes=40 --num_flows=80 --packets_per_second=400 --coverage_area=5"

