#!/bin/sh
export KUBECONFIG=/root/kubeconf_ibm/kube-config-mil01-mycluster.yml
service='wsserv-deployment'

docker build --tag uk.icr.io/nodemcu/wsserv:1 .
docker push uk.icr.io/nodemcu/wsserv:1
kubectl delete "pod/`kubectl get pods --no-headers=true|awk '{print $1}'`"

#extract information in order to know IPs and ports of our services
ports=`kubectl get service/${service} --no-headers=true|awk '{print $5}'`
port_ws=`kubectl get service/${service} --no-headers=true|awk '{print $5}'|sed 's/:/ /g'|sed 's/\/TCP//g'|sed 's/,/ /g'|awk '{print $2}'`
port_http=`kubectl get service/${service} --no-headers=true|awk '{print $5}'|sed 's/:/ /g'|sed 's/\/TCP//g'|sed 's/,/ /g'|awk '{print $4}'`
ip=`kubectl get nodes -o jsonpath='{.items[*].status.addresses[?(@.type=="ExternalIP")].address}'`

echo "http://"$ip":"$port_http"\n"
echo "ws://"$ip":"$port_ws"\n"
