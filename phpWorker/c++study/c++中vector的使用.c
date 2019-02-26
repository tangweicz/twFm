什么是vector。

是一个数组


vector的使用示例：

    struct XData{

        xxx
        xxx

    }

    vector<XData> datas;

    datas.begin();//取出第一个元素



所有的方法列表：
    begin()                     //指向迭代器中第一个元素。
    end()                       //指向迭代器中末端元素的下一个，指向一个不存在元素。
    push_back(elem)             //在尾部加入一个数据。
    pop_back()                  //删除最后一个数据。
    capacity()                  //vector可用空间的大小。
    size()                      //返回容器中数据个数。
    empty()                     //判断容器是否为空。
    front()                     //传回第一个数据。
    back()                      //传回最后一个数据，不检查这个数据是否存在。
    at(index)                   //传回索引idx所指的数据，如果idx越界，抛出out_of_range。
    clear()                     //移除容器中所有数据。
    erase(iterator)             //删除pos位置的数据，传回下一个数据的位置。
    erase(begin,end)            //删除[beg,end)区间的数据，传回下一个数据的位置。注意：begin和end为iterator
    insert(position,elem)       //在pos位置插入一个elem拷贝，传回新数据位置。
    insert(position,n,elem)     //在pos位置插入n个elem数据，无返回值。
    insert(position,begin,end)  //在pos位置插入在[beg,end)区间的数据，无返回值。