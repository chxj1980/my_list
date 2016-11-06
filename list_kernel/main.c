/**
linux����ʾ������ʾ������������ɾ���Ȳ�����

ע���ں�listʵ�֣��ƺ��޷������������ĳ���ڵ㡣������ҷ���ĳ����ʱ�ڴ˽ڵ�ǰ/������½ڵ㡣
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "list.h"

struct i2c_devinfo
{
	struct list_head list;
	int busnum;
    int slave;
};

// ��������
LIST_HEAD(my_list);

void init(int busnum, int slave)
{
    struct i2c_devinfo* devinfo;
    // ����ռ�
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    if (devinfo == NULL) return;
    
    // ��ֵ
    devinfo->busnum = busnum;
    devinfo->slave = slave;
    
    // ��ӵ�������
    //list_add_tail(&devinfo->list, &my_list); // ����β��
    list_add(&devinfo->list, &my_list); // ����ͷ��
}

void show(void)
{
    struct i2c_devinfo *devinfo;
    int i = 0;

    printf("show info in the list.\n");
    // ���ͣ���ȫ������my_list���õ�devinfo��Ϣ������listΪdevinfo���͵�һ����Ա����
    list_for_each_entry(devinfo, &my_list, list)
    {
        printf("[%d] busnum: %d, slave: %d\n", i++, devinfo->busnum, devinfo->slave);
	}
}

void delete_list(void)
{
    struct i2c_devinfo *devinfo, *tdev;
    //��ע����Ҫɾ����������ʱ��Ҫʹ��*_safe��ͬʱҪ��������һ����ʱ����
    list_for_each_entry_safe(devinfo, tdev, &my_list, list)
    {
        list_del(&devinfo->list);
        free(devinfo);
	}
}

void add_del(void)
{
    struct i2c_devinfo* devinfo, *tdev, *pdev;
    // ����ռ�
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    if (devinfo == NULL) return;
    
    // ��ֵ
    devinfo->busnum = 250;
    devinfo->slave = 25;
    
    list_for_each_entry_safe(tdev, pdev, &my_list, list)
    {
        if (tdev->busnum == 1)
        {
            //list_move(&tdev->list, &devinfo->list);
            break;
        }
    }
}

int main(void)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < 5; i++, j += 2)
    {
        init(i, j);
    }
    
    show();
    
    //delete_list();
add_del();
    // ɾ��������Ѿ�û��������
    show();

    return 0;
}