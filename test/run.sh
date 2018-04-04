#!/bin/bash

dnsmasq_pre() {
  echo -n $(date +"%b %e %H:%M:%S")
  echo -n "dnsmasq[123]:"
}

# Install deps
apt-get install libcap2-bin sqlite3 dnsutils

# Create necessary infrastructure
mkdir "/etc/pihole"
mkdir "/var/run/pihole"
touch "/var/log/pihole-FTL.log"

# Copy file
cp pihole-FTL-linux-x86_64 pihole-FTL
chmod +x pihole-FTL

# Start FTL
./pihole-FTL

# Prepare BATS
mkdir -p test/libs
cd test/libs
git clone https://github.com/sstephenson/bats
git clone https://github.com/ztombol/bats-support
cd ../..

# Block until FTL is ready, retry once per second for 45 seconds
n=0
until [ $n -ge 45 ]; do
  nc -vv -z -w 30 127.0.0.1 4711 && break
  n=$[$n+1]
  echo "..."
  tail -n2 pihole-FTL.log
  echo "..."
  sleep 1
done

# Run tests
test/libs/bats/bin/bats "test/test_suite.sh"
exit $?
