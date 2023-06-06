#include <stdio.h>
#include <stdlib.h>

int add1(int num)
{
        int ret = 0;
        int *pTmp = NULL;
        *pTmp = 1;	//对未分配内存空间的指针进行赋值，模拟访问非法内存段错误
        ret = num + *pTmp;
        return ret;
}

int add(int num)
{
        int ret = 0;
        ret = add1(num);
        return ret;
}


