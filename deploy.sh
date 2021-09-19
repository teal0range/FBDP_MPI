#! /bin/bash

# 构建镜像
docker build -t mpi .
if [ $(docker network ls | grep mpi-net |wc -l) -eq 0 ];then
    docker network create mpi-net
fi
# 删除之前的容器
docker rm -f $(docker ps -a|awk '/mpi/ {print $1}')
# 运行容器
docker run -itd -v /mpi:/root/mpi/ --network=mpi-net --hostname host1 --name mpi-host1 mpi bash
docker run -itd -v /mpi:/root/mpi/ --network=mpi-net --hostname host2 --name mpi-host2 mpi bash
docker run -itd -v /mpi:/root/mpi/ --network=mpi-net --hostname host3 --name mpi-host3 mpi bash
# 复制hosts文件
docker exec -d mpi-host2 cat /root/hosts > /etc/hosts 
docker exec -d mpi-host3 cat /root/hosts > /etc/hosts 
docker exec -d mpi-host1 cat /root/hosts > /etc/hosts
# 修改ssh配置
docker exec -d mpi-host1 cp /root/config /root/.ssh/config
docker exec -d mpi-host2 cp /root/config /root/.ssh/config
docker exec -d mpi-host3 cp /root/config /root/.ssh/config
# 启动三台机子的ssh服务
docker exec -d mpi-host2 service ssh start
docker exec -d mpi-host1 service ssh start
docker exec -d mpi-host3 service ssh start
# 编译运行
docker exec mpi-host1 mpicc /root/mpi/$1.c -o /root/mpi/tmp -lm
docker exec mpi-host1 mpirun -np $2 -host host1,host2,host3 /root/mpi/tmp $3