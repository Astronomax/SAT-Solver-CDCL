#!/bin/bash

base_url="https://www.cs.ubc.ca/~hoos/SATLIB/"

sats=(
    "Benchmarks/SAT/RND3SAT/uf20-91.tar.gz"
    "Benchmarks/SAT/RND3SAT/uf50-218.tar.gz"
    "Benchmarks/SAT/RND3SAT/uf75-325.tar.gz"
    "Benchmarks/SAT/RND3SAT/uf100-430.tar.gz"
#    "Benchmarks/SAT/RND3SAT/uf125-538.tar.gz"
#    "Benchmarks/SAT/RND3SAT/uf150-645.tar.gz"
#    "Benchmarks/SAT/RND3SAT/uf175-753.tar.gz"
#    "Benchmarks/SAT/RND3SAT/uf200-860.tar.gz"
#    "Benchmarks/SAT/RND3SAT/uf225-960.tar.gz"
#    "Benchmarks/SAT/RND3SAT/uf250-1065.tar.gz"
#    "Benchmarks/SAT/BMS/RTI_k3_n100_m429.tar.gz"
#    "Benchmarks/SAT/BMS/BMS_k3_n100_m429.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m403_b10.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m403_b30.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m403_b50.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m403_b70.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m403_b90.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m411_b10.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m411_b30.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m411_b50.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m411_b70.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m411_b90.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m418_b10.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m418_b30.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m418_b50.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m418_b70.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m418_b90.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m423_b10.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m423_b30.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m423_b50.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m423_b70.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m423_b90.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m429_b10.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m429_b30.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m429_b50.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m429_b70.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m429_b90.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m435_b10.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m435_b30.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m435_b50.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m435_b70.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m435_b90.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m441_b10.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m441_b30.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m441_b50.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m441_b70.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m441_b90.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m449_b10.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m449_b30.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m449_b50.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m449_b70.tar.gz"
#    "Benchmarks/SAT/CBS/CBS_k3_n100_m449_b90.tar.gz"
#    "Benchmarks/SAT/GCP/flat30-60.tar.gz"
#    "Benchmarks/SAT/GCP/flat50-115.tar.gz"
#    "Benchmarks/SAT/GCP/flat75-180.tar.gz"
#    "Benchmarks/SAT/GCP/flat100-239.tar.gz"
#    "Benchmarks/SAT/GCP/flat125-301.tar.gz"
#    "Benchmarks/SAT/GCP/flat150-360.tar.gz"
#    "Benchmarks/SAT/GCP/flat175-417.tar.gz"
#    "Benchmarks/SAT/GCP/flat200-479.tar.gz"
#    "Benchmarks/SAT/SW-GCP/sw100-8-lp0-c5.tar.gz"
#    "Benchmarks/SAT/SW-GCP/sw100-8-lp1-c5.tar.gz"
#    "Benchmarks/SAT/SW-GCP/sw100-8-lp2-c5.tar.gz"
#    "Benchmarks/SAT/SW-GCP/sw100-8-lp3-c5.tar.gz"
#    "Benchmarks/SAT/SW-GCP/sw100-8-lp4-c5.tar.gz"
#    "Benchmarks/SAT/SW-GCP/sw100-8-lp5-c5.tar.gz"
#    "Benchmarks/SAT/SW-GCP/sw100-8-lp6-c5.tar.gz"
#    "Benchmarks/SAT/SW-GCP/sw100-8-lp7-c5.tar.gz"
#    "Benchmarks/SAT/SW-GCP/sw100-8-lp8-c5.tar.gz"
#    "Benchmarks/SAT/SW-GCP/sw100-8-p0-c5.tar.gz"
#    "Benchmarks/SAT/PLANNING/BlocksWorld/blocksworld.tar.gz"
#    "Benchmarks/SAT/PLANNING/Logistics/logistics.tar.gz"
#    "Benchmarks/SAT/AIS/ais.tar.gz"
#    "Benchmarks/SAT/DIMACS/LRAN/f.tar.gz"
#    "Benchmarks/SAT/DIMACS/GCP/gcp-large.tar.gz"
#    "Benchmarks/SAT/DIMACS/PARITY/parity.tar.gz"
#    "Benchmarks/SAT/DIMACS/II/inductive-inference.tar.gz"
#    "Benchmarks/SAT/DIMACS/HANOI/hanoi.tar.gz"
)

unsats=(
    "Benchmarks/SAT/RND3SAT/uuf50-218.tar.gz"
    "Benchmarks/SAT/RND3SAT/uuf75-325.tar.gz"
    "Benchmarks/SAT/RND3SAT/uuf100-430.tar.gz"
    "Benchmarks/SAT/RND3SAT/uuf125-538.tar.gz"
#    "Benchmarks/SAT/RND3SAT/uuf150-645.tar.gz"
#    "Benchmarks/SAT/RND3SAT/uuf175-753.tar.gz"
#    "Benchmarks/SAT/RND3SAT/uuf200-860.tar.gz"
#    "Benchmarks/SAT/RND3SAT/uuf225-960.tar.gz"
#    "Benchmarks/SAT/RND3SAT/uuf250-1065.tar.gz"
#    "Benchmarks/SAT/DIMACS/DUBOIS/dubois.tar.gz"
#    "Benchmarks/SAT/DIMACS/BF/bf.tar.gz"
#    "Benchmarks/SAT/DIMACS/PHOLE/pigeon-hole.tar.gz"
#    "Benchmarks/SAT/DIMACS/PRET/pret.tar.gz"
)

for t in ${sats[@]}; do
  wget -O sats.tar.gz "$base_url$t"
  mkdir sats
  tar -xvzf sats.tar.gz -C sats | while read -r filename ; do
    echo $filename
    answer=$($1 "sats/$filename")
    [ "$answer" == "SAT" ] && echo "OK" || exit 1
  done
  rm sats.tar.gz
  rm -r sats
done

for t in ${unsats[@]}; do
  wget -O unsats.tar.gz "$base_url$t"
  mkdir unsats
  tar -xvzf unsats.tar.gz -C unsats | while read -r filename ; do
    echo $filename
    answer=$($1 "unsats/$filename")
    [ "$answer" == "UNSAT" ] && echo "OK" || exit 1
  done
  rm unsats.tar.gz
  rm -r unsats
done

exit 0