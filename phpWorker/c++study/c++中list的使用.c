什么是list。

list是一个线性双向链表结构，它的数据由若干个节点构成，每一个节点都包括一个信息块（即实际存储的数据）、一个前驱指针和一个后驱指针。
它无需分配指定的内存大小且可以任意伸缩，这是因为它存储在非连续的内存空间中，并且由指针将有序的元素链接起来。
由于其结构的原因，list 随机检索的性能非常的不好，因为它不像vector 那样直接找到元素的地址，而是要从头一个一个的顺序查找，
这样目标元素越靠后，它的检索时间就越长。检索时间与目标元素的位置成正比。虽然随机检索的速度不够快，但是它可以迅速地在任何节点进行插入和删除操作。因为list 的每个节点保存着它在链表中的位置，
插入或删除一个元素仅对最多三个元素有所影响，不像vector 会对操作点之后的所有元素的存储地址都有所影响，这一点是vector 不可比拟的。


list的使用示例：

    struct XData{

        xxx
        xxx

    }

    list<XData> datas;

    datas.front();//取出第一个元素



所有的方法列表：
    assign() 给list赋值
    back() 返回最后一个元素
    begin() 返回指向第一个元素的迭代器
    clear() 删除所有元素
    empty() 如果list是空的则返回true
    end() 返回末尾的迭代器
    erase() 删除一个元素
    front() 返回第一个元素
    get_allocator() 返回list的配置器
    insert() 插入一个元素到list中
    max_size() 返回list能容纳的最大元素数量
    merge() 合并两个list
    pop_back() 删除最后一个元素
    pop_front() 删除第一个元素
    push_back() 在list的末尾添加一个元素
    push_front() 在list的头部添加一个元素
    rbegin() 返回指向第一个元素的逆向迭代器
    remove() 从list删除元素
    remove_if() 按指定条件删除元素
    rend() 指向list末尾的逆向迭代器
    resize() 改变list的大小
    reverse() 把list的元素倒转
    size() 返回list中的元素个数
    sort() 给list排序
    splice() 合并两个list
    swap() 交换两个list
    unique() 删除list中重复的元素