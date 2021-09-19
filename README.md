## 说明

### 源代码
- Integral.c
使用MPI_Recv以及MPI_Send接口编写的程序，计算$y=x^3$从10到100的积分，参数N可指定分割的长方形个数。
- sqrtSum.c
使用MPI_Reduce接口编写的开放求和程序，参数N可指定求和数列的大小。

### 构建文件
- Dockerfile
  构建MPI环境的镜像。
- deploy.sh
  一键启动容器的shell脚本，参数为编译目标、进程数、参数N
  ```shell
  ./deploy.sh Integral 2 1000
  ```
  