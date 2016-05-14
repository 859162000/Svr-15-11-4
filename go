#!/bin/bash
#rm -f /tmp/*.*
SuperServer/SuperServer -d ~/log/superserver.log
sleep 1
FLServer/FLServer -d ~/log/flserver.log
sleep 1
RecordServer/RecordServer -d ~/log/recordserver.log
sleep 1
BillServer/BillServer -d ~/log/billserver.log
sleep 1
#InfoServer/InfoServer -d
#roleRegServer/roleRegServer -d
#MiniServer/MiniServer -d
#sleep 3
SessionServer/SessionServer -d ~/log/sessserver.log
sleep 1
ScenesServer/ScenesServer -d ~/log/sceneserver1.log
sleep 1
GatewayServer/GatewayServer -d ~/log/gatewayserver1.log
