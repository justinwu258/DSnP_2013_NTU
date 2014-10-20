#include <stdio.h>
#include <stdlib.h>
 
int main(void)
{
    FILE *fPtr;
    char c;
     
    fPtr = fopen("oldname.txt", "w");
    if (!fPtr) {
        printf("檔案建立失敗...\n");
        exit(1);
    }
     
    while ((c = getchar()) != EOF) {
        fputc(c, fPtr);
//        fflush(fPtr);
    }
     
    fclose(fPtr);
     
    return 0;
}
 
 
/* 《程式語言教學誌》的範例程式
    http://pydoing.blogspot.com/
    檔名：cfflush.c
    功能：示範 stdio.h 中函數 fflush() 的使用
    作者：張凱慶
    時間：西元2010年6月 */
