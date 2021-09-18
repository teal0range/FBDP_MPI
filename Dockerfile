FROM ubuntu

RUN apt -y update 2>>/dev/null \
    && apt -y install openssh-server \
    && apt -y install mpich\
    && ssh-keygen -t rsa -f /root/.ssh/id_rsa\
    && cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys

COPY ./resources/* /root/
