php的共享内存的知识点，在进程进通讯使用(可以是两个完全独立的进程)：
    1、shm_attach ( int $key [, int $memsize [, int $perm = 0666 ]] )//打开或创建一个共享内存

    2、shm_detach ( resource $shm_identifier )//关闭一个共享内存的连接

    3、shm_put_var ( resource $shm_identifier , int $variable_key , mixed $variable )//插入或更新一个共享内存中的值

    4、shm_get_var ( resource $shm_identifier , int $variable_key )//从共享内存中取值

    等等一系列参数

    eg：//使用过程中需要配合php的相关锁进行处理，下面的列子是不带锁的。


        $key = ftok('/tmp', 'd');

        $shm_id = shm_attach($key, 4096);

        shm_put_var($shm_id, 0, 100);

        shm_get_var($shm_id, 0);

        shm_detach($shm_id);