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

ʹ��list_for_each���������û���ҵ���������ʱ���ָ��list_head
��&n->list�룽&my_listֵ��ȡ�

---------------------------------
���н����
--------show list------------------
show info in the list.
[0] busnum: 10, slave: 0
[1] busnum: 11, slave: 2
[2] busnum: 12, slave: 4
[3] busnum: 13, slave: 6
[4] busnum: 14, slave: 8
--------[show list end]------------------
==================================...
prev entry: 14
next entry: 10
first entry: 10
last entry: 14
before 12 is 11
next 12 is 13
--------show list------------------
show info in the list.
[0] busnum: 1, slave: 25
[1] busnum: 10, slave: 0
[2] busnum: 250, slave: 25
[3] busnum: 11, slave: 2
[4] busnum: 250, slave: 25
[5] busnum: 12, slave: 4
[6] busnum: 14, slave: 9
[7] busnum: 65535, slave: 25
--------[show list end]------------------
==================================...
after delete...
--------show list------------------
list empty, nothing to show.
==================================...
devinfo1: 0x9788068 0x9788070 0x9788074
devinfo2: 0x9788068 0x9788070 0x9788070 0x9788074 2
next1 entry: 1
next2 entry: 2
next3 entry: 0(0x0)
next4 entry: 1
next5 entry: 2
==================================...
Adding node[0]: 0x9788038
Adding node[1]: 0x97880c8
Adding node[2]: 0x9788020
Adding node[3]: 0x97880b0
Adding node[4]: 0x9788008
Deleting node[0]: 0x9788038
Deleting node[1]: 0x97880c8
Deleting node[2]: 0x9788020
Deleting node[3]: 0x97880b0
Deleting node[4]: 0x9788008
==================================...

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

void show(list_head* list)
{
    struct i2c_devinfo *devinfo;
    int i = 0;

    printf("--------show list------------------\n");
    if (list_empty(list))
    {
        printf("list empty, nothing to show.\n");
        return;
    }
    printf("show info in the list.\n");
    // ���ͣ���ȫ������my_list���õ�devinfo��Ϣ������listΪdevinfo���͵�һ����Ա����
    //list_for_each(devinfo, &list, struct i2c_devinfo, list)
    list_for_each(devinfo, list, struct i2c_devinfo, list)
    //list_for_each_reverse(devinfo, list, struct i2c_devinfo, list)
    {
        printf("[%d] busnum: %d, slave: %d\n", i++, devinfo->busnum, devinfo->slave);
	}
    printf("--------[show list end]------------------\n");
}

void delete_list(list_head* list)
{
    struct i2c_devinfo *devinfo, *tdev;
    //��ע����Ҫɾ����������ʱ��Ҫʹ��*_safe��ͬʱҪ��������һ����ʱ����
    list_for_each_safe(devinfo, tdev, list, struct i2c_devinfo, list)
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

    // ��ӡָ�����ǰ������
    list_for_each_safe(tdev, tmpdev, &my_list, struct i2c_devinfo, list)
    {
        if (tdev->busnum == 12)
        {
            n = list_entry_prev(&tdev->list, struct i2c_devinfo, list);
            printf("before %d is %d\n", tdev->busnum, n->busnum);
            n = list_entry_next(&tdev->list, struct i2c_devinfo, list);
            printf("next %d is %d\n", tdev->busnum, n->busnum);
        }
    }
    
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
            free(tdev);
        }
        
        if (tdev->busnum == 14) // �޸Ĵ˽������
        {
            tdev->slave += 1;
        }
	}
}

// ����ʾ��
void list_misc1(void)
{
    LIST_HEAD(my_list);
    struct i2c_devinfo* devinfo;
    struct i2c_devinfo* n = NULL;
    
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 1;
    devinfo->slave = 25;
    list_add_tail(&devinfo->list, &my_list);
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 2;
    devinfo->slave = 25;
    list_add_tail(&devinfo->list, &my_list);
    
    printf("devinfo1: %p %p %p\n", devinfo, &devinfo->busnum, &devinfo->slave);
    
    // ����container_of
    int* aa = &devinfo->busnum;
    n = container_of(aa, struct i2c_devinfo, busnum);
    
    printf("devinfo2: %p %p %p %p %d\n", n, aa, &n->busnum, &n->slave, n->busnum);

    n = list_entry(my_list.next, struct i2c_devinfo, list); // ��һ�����
    printf("next1 entry: %d\n", n->busnum);
    n = list_entry(my_list.next->next, struct i2c_devinfo, list);   // �ڶ������
    printf("next2 entry: %d\n", n->busnum);
    n = list_entry(my_list.next->next->next, struct i2c_devinfo, list); // �ص�my_list����ʱ������Ч
    printf("next3 entry(invalid): %d(0x%x)\n", n->busnum, n->busnum);
    n = list_entry(my_list.next->next->next->next, struct i2c_devinfo, list);   // ��һ�����
    printf("next4 entry: %d\n", n->busnum);
    n = list_entry(my_list.next->next->next->next->next, struct i2c_devinfo, list); // �ڶ������
    printf("next5 entry: %d\n", n->busnum);
}

// ��֤���ݽ��������ͷŵ�ַ�Ƿ�һ��
void list_misc2(void)
{
    LIST_HEAD(my_list);
    
    struct i2c_devinfo* devinfo, *tdev;
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        // ����ռ�
        devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
        if (devinfo == NULL) return;

        // ��ֵ
        devinfo->busnum = i;
        devinfo->slave = i+100;

        printf("Adding node[%d]: %p\n", i, devinfo);
        // ��ӵ�������
        list_add_tail(&devinfo->list, &my_list);
    }
    
    i=0;
    list_for_each_safe(devinfo, tdev, &my_list, struct i2c_devinfo, list)
    {
        list_del(&devinfo->list);
        printf("Deleting node[%d]: %p\n", i++, devinfo);
        free(devinfo);
    }
}

//////////////////////////////////
// ��˳����ӵ�������
LIST_HEAD(list_misc3_head);
void list_misc3_add(struct i2c_devinfo* devinfo)
{
    struct i2c_devinfo* tdev;

    list_for_each(tdev, &list_misc3_head, struct i2c_devinfo, list)
    {
        if (devinfo->busnum > tdev->busnum) // ����  ���Ϊ��>�����ǽ���
        {
            list_add_prev(&devinfo->list, &tdev->list); // ʹ�õ���prev����Ϊ������tdev�ڵ�ʱ������С�ģ�Ҫ�嵽ǰ�棬�����Ǻ��棬ͬ����������prev
            return;
        }
    }

    // ���������������������Ϊ�գ���ﵽ�˴�
    list_add_tail(&devinfo->list, &list_misc3_head);
}

#include <time.h>
#include <unistd.h>
void list_misc3(void)
{
    struct i2c_devinfo* devinfo;
#if 01
    for (int i = 0; i < 5; i++)
    {
        // ����ռ�
        devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
        if (devinfo == NULL) return;

        srand(time(NULL));
        sleep(1);
        // ��ֵ
        devinfo->busnum = rand() % 100;
        devinfo->slave = i+100;
        
        // ��ӵ�������
        list_misc3_add(devinfo);
        //list_add_tail(&devinfo->list, &list_misc3_head);
    }
#else
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 32;
    devinfo->slave = 100;
    list_misc3_add(devinfo);
    show(&list_misc3_head);
    
    
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 3;
    devinfo->slave = 100;
    list_misc3_add(devinfo);
    show(&list_misc3_head);
    
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 37;
    devinfo->slave = 100;
    list_misc3_add(devinfo);

    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 1;
    devinfo->slave = 100;
    list_misc3_add(devinfo);
    #endif
    
    // ��ʾ
    show(&list_misc3_head);
    
    delete_list(&list_misc3_head);
}

int main(void)
{
    int i = 0;
    int j = 0;
    for (i = 10; i < 15; i++, j += 2)
    {
        init(i, j);
    }
    
    show(&my_list);

    printf("==================================...\n");
    list_misc();
    show(&my_list);
    
    printf("==================================...\n");

    printf("after delete...\n");
    delete_list(&my_list);
    // ɾ��������Ѿ�û��������
    show(&my_list);
    
    printf("==================================...\n");
    
    list_misc1();
    
    printf("==================================...\n");
    
    list_misc2();
    
    printf("==================================...\n");
    
    //list_misc3();
    
    return 0;
}