#include <iostream>
#include <stdio.h>

#define PIC_WIDTH   800
#define PIC_HEIGTH  600

void conver_yuyv_2_y(void)
{
    FILE *src = NULL;
    FILE *dst = NULL;

    char buffer[PIC_WIDTH * PIC_HEIGTH * 2]   = {0X00,};
    //char buffer[PIC_WIDTH * PIC_HEIGTH * 3/2] = {0X00,};

    char y_index[PIC_WIDTH * PIC_HEIGTH] = {0x00,};

    char *y_d = y_index;


    src = fopen("./yuyv_test.yuv", "rb");
    fread(buffer, PIC_WIDTH * PIC_HEIGTH * 2, 1, src);

    dst = fopen("./y_test.yuv", "wa+");    

    for(int i = 0; i < PIC_WIDTH * PIC_HEIGTH * 2; i += 2)
    {
        *y_d++ = buffer[i];
    }

    fwrite(y_index, PIC_WIDTH * PIC_HEIGTH, 1, dst);

    fclose(src);
    fclose(dst);

}













//yu12
void conver_yuyv_2_420p(void)
{
    FILE *src = NULL;
    FILE *dst = NULL;

    char buffer[PIC_WIDTH * PIC_HEIGTH * 2]  = {0X00,};
    
    char y_index[PIC_WIDTH * PIC_HEIGTH]     = {0x00,};
    char u_index[PIC_WIDTH * PIC_HEIGTH / 4] = {0x00,};
    char v_index[PIC_WIDTH * PIC_HEIGTH / 4] = {0x00,};

    char *y_d = y_index;
    char *u_d = u_index;
    char *v_d = v_index;

    src = fopen("./yuyv_test.yuv", "rb");
    fread(buffer, PIC_WIDTH * PIC_HEIGTH * 2, 1, src);

    dst = fopen("./yu12_test.yuv", "wa+");    
    bool flag = true;

    for(int i = 0, j = 0; i < PIC_WIDTH * PIC_HEIGTH * 2; i += 2, j++)
    {
        *y_d++ = buffer[i];
        if(flag)
        {
            if(i%4 == 0)
            {
                *u_d++ = buffer[i+1];
            }
        }
        else
        {
            if(i%4 == 0)
            {
                *v_d++ = buffer[i+3];
            }
        }

        if(j == PIC_WIDTH)
        {
            j = 0;
            if(flag)
            {
                flag = false;
            }
            else
            {
                flag = true;
            }
        }
    }
    fwrite(y_index, PIC_WIDTH * PIC_HEIGTH, 1, dst);
    fwrite(u_index, PIC_WIDTH * PIC_HEIGTH / 4, 1, dst);
    fwrite(v_index, PIC_WIDTH * PIC_HEIGTH / 4, 1, dst);
    fclose(src);
    fclose(dst);

}






int main(int argc, char **argv)
{
    std::cout << "This is a test" << std::endl;
    conver_yuyv_2_420p();
    return 0;
}




