#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    int num = 10;
    char str[100];
    itoa(num, str, 8);  //将整数10转换为八进制保存在str字符数组中  
    printf("%s\n", str);
    system("pause");
    return 0;
}  