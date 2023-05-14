#!/bin/sh
# run multiple experiments in batch process.
# the lobster server is 24 cores, each run will take a core.
nohup ./waf --run "scratch/test-my --beta=0.5 --reduFactor=10 --simu=false --alpha=0.01" & 
sleep 10
nohup ./waf --run "scratch/test-my --beta=0.5 --reduFactor=10 --simu=false --alpha=0.02" &
sleep 10
nohup ./waf --run "scratch/test-my --beta=0.5 --reduFactor=10 --simu=falsie --alpha=0.03" &
sleep 10
nohup ./waf --run "scratch/test-my --beta=0.5 --reduFactor=10 --simu=false --alpha=0.04" & 
sleep 10
nohup ./waf --run "scratch/test-my --beta=0.5 --reduFactor=10 --simu=false --alpha=0.05" &
sleep 10
nohup ./waf --run "scratch/test-my --beta=1 --reduFactor=10 --simu=false --alpha=0.01" & 
sleep 10
nohup ./waf --run "scratch/test-my --beta=1 --reduFactor=10 --simu=false --alpha=0.02" &
sleep 10
nohup ./waf --run "scratch/test-my --beta=1 --reduFactor=10 --simu=false --alpha=0.03" &
sleep 10
nohup ./waf --run "scratch/test-my --beta=1 --reduFactor=10 --simu=false --alpha=0.04" & 
sleep 10
nohup ./waf --run "scratch/test-my --beta=1 --reduFactor=10 --simu=false --alpha=0.05" &
sleep 10
nohup ./waf --run "scratch/test-my --beta=1.5 --reduFactor=10 --simu=false --alpha=0.01" & 
sleep 10
nohup ./waf --run "scratch/test-my --beta=1.5 --reduFactor=10 --simu=false --alpha=0.02" &
sleep 10
nohup ./waf --run "scratch/test-my --beta=1.5 --reduFactor=10 --simu=false --alpha=0.03" &
sleep 10
nohup ./waf --run "scratch/test-my --beta=1.5 --reduFactor=10 --simu=false --alpha=0.04" & 
sleep 10
nohup ./waf --run "scratch/test-my --beta=1.5 --reduFactor=10 --simu=false --alpha=0.05" &
sleep 10
echo "experiments all running now!"
