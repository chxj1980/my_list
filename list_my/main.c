/**
˫������ʾ������ʾ������������ɾ���Ȳ�����
g++ 5.8.4 & VS2010 ����ͨ��

Late Lee <latelee@163.com> http://www.latelee.org


��hostapd��listԴ���޸Ķ��� 

�ṹʾ��ͼ��

list_head
   prev ----------------------------------------------- 
      |                                                |
      -<-                                              |
   next |    node1          node2           node3      |
   ^|   |                                              |
   ||   |   data...         data...         data...    |
   ||   --- prev <--------- prev <--------- prev <-----|
   ||-----> next ---------> next ---------> next ------
   |        data...         data...         data...    |
   |-----<----------------------------------------------
   
ͷ��㲻ʹ�ã��޷���list_entry�õ�ʵ������
list_head��prevָ�����һ����㣬nextָ���һ����㡣
���һֱ��next�ɱ�����ѭ������ʼ����prev��Ȼ��

ע��list_add�����ڡ���㡱����ӣ�����list_haed������ӣ���������������ͷ��
�����node1������ӣ��������������node1���¼ӽ�㡣

*/

#include <stdio.h>
#include <stdlib.h>

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
    list_add_tail(&devinfo->list, &my_list);
}

void show(void)
{
    struct i2c_devinfo *devinfo;
    int i = 0;

    if (list_empty(&my_list))
    {
        printf("list empty, nothing to show.\n");
        return;
    }
    printf("show info in the list.\n");
    // ���ͣ���ȫ������my_list���õ�devinfo��Ϣ������listΪdevinfo���͵�һ����Ա����
    //list_for_each(devinfo, &my_list, struct i2c_devinfo, list)
    list_for_each(devinfo, &my_list, struct i2c_devinfo, list)
    {
        printf("[%d] busnum: %d, slave: %d\n", i++, devinfo->busnum, devinfo->slave);
	}
}

void delete_list(void)
{
    struct i2c_devinfo *devinfo, *tdev;
    //��ע����Ҫɾ����������ʱ��Ҫʹ��*_safe��ͬʱҪ��������һ����ʱ����
    list_for_each_safe(devinfo, tdev, &my_list, struct i2c_devinfo, list)
    {
        list_del(&devinfo->list);
        free(devinfo);
	}
}

void list_misc(void)
{
    struct i2c_devinfo* devinfo, *tdev, *tmpdev;

    struct i2c_devinfo* n;
    n = list_entry(my_list.prev, struct i2c_devinfo, list);
    printf("prev entry: %d\n", n->busnum);
    n = list_entry(my_list.next, struct i2c_devinfo, list);
    printf("next entry: %d\n", n->busnum);

    // ��ȡ��һ�������һ��
    n = list_first_entry(&my_list, struct i2c_devinfo, list);
    printf("first entry: %d\n", n->busnum);
    n = list_last_entry(&my_list, struct i2c_devinfo, list);
    printf("last entry: %d\n", n->busnum);
    
    // 
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 1;
    devinfo->slave = 25;
    list_add(&devinfo->list, &my_list); // ��my_list����ӣ�����������ͷ��
    
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 65535;
    devinfo->slave = 25;
    list_add_tail(&devinfo->list, &my_list); // ��ĩβ
    
    // ��;���롢ɾ��
    //list_for_each(tdev, &my_list, struct i2c_devinfo, list)
    list_for_each_safe(tdev, tmpdev, &my_list, struct i2c_devinfo, list)
    {
        if (tdev->busnum == 10) // �ڴ˽ڵ�����
        {
            devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
            devinfo->busnum = 250;
            devinfo->slave = 25;
            list_add(&devinfo->list, &tdev->list);
        }
        if (tdev->busnum == 12) // �ڴ˽ڵ�ǰ����
        {
            devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
            devinfo->busnum = 250;
            devinfo->slave = 25;
            list_add_prev(&devinfo->list, &tdev->list);
        }
        if (tdev->busnum == 13) // ɾ���˽ڵ�
        {
            list_del(&tdev->list);
        }
	}
}

// ����ʾ��
void list_misc1(void)
{
    LIST_HEAD(my_list);
    struct i2c_devinfo* devinfo;
    
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 1;
    devinfo->slave = 25;
    list_add_tail(&devinfo->list, &my_list);
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 2;
    devinfo->slave = 25;
    list_add_tail(&devinfo->list, &my_list);
    
    
    struct i2c_devinfo* n;
    n = list_entry(my_list.next, struct i2c_devinfo, list); // ��һ�����
    printf("next1 entry: %d\n", n->busnum);
    n = list_entry(my_list.next->next, struct i2c_devinfo, list);   // �ڶ������
    printf("next2 entry: %d\n", n->busnum);
    n = list_entry(my_list.next->next->next, struct i2c_devinfo, list); // �ص�my_list����ʱ������Ч
    printf("next3 entry: %d(0x%x)\n", n->busnum, n->busnum);
    n = list_entry(my_list.next->next->next->next, struct i2c_devinfo, list);   // ��һ�����
    printf("next4 entry: %d\n", n->busnum);
    n = list_entry(my_list.next->next->next->next->next, struct i2c_devinfo, list); // �ڶ������
    printf("next5 entry: %d\n", n->busnum);
}

int main(void)
{
    int i = 0;
    int j = 0;
    for (i = 10; i < 15; i++, j += 2)
    {
        init(i, j);
    }

    show();

    list_misc();
    show();

    printf("after delete...\n");
    delete_list();
    // ɾ��������Ѿ�û��������
    show();

    list_misc1();
    return 0;
}