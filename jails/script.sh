#!/bin/sh
sudo ifconfig bridge2 create up; sudo ifconfig bridge3 create up; sudo ifconfig bridge5 create up
for i in $(jot 5); do sudo ifconfig epair${i} create up; done
for i in $(jot 4); do sudo ifconfig epair5${i} create up; done
sudo ifconfig em0.2 create vlan 2 vlandev em0 up; sudo ifconfig em0.3 create vlan 3 vlandev em0 up
sudo ifconfig bridge2 inet 192.0.2.254/24 addm em0.2 addm epair1a addm epair2a
sudo ifconfig bridge3 inet 192.0.3.254/24 addm em0.2 addm epair3a addm epair4a
sudo ifconfig bridge5 inet 192.0.5.254/24 addm epair51a addm epair52a addm epair53a addm epair54a addm epair5a
for i in $(jot 4); do sudo jail -c name=jail$i host.hostname=jail$i persist vnet vnet.interface=epair${i}b vnet.interface=epair5${i}b; done
sudo jail -c name=jail5 host.hostname=jail5 persist vnet vnet.interface=epair5b
sudo jexec jail5 ifconfig epair5b inet 192.0.5.15/24 up
sudo jexec jail1 ifconfig epair1b inet 192.0.2.11/24 up; sudo jexec jail2 ifconfig epair2b inet 192.0.2.12/24 up
sudo jexec jail3 ifconfig epair3b inet 192.0.3.13/24 up; sudo jexec jail4 ifconfig epair4b inet 192.0.3.14/24 up
sudo jexec jail1 ifconfig epair51b inet 192.0.5.11/24 up; sudo jexec jail2 ifconfig epair52b inet 192.0.5.12/24 up
sudo jexec jail3 ifconfig epair53b inet 192.0.5.13/24 up; sudo jexec jail4 ifconfig epair54b inet 192.0.5.14/24 up
