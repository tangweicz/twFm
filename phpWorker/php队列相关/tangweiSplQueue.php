<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/12
 * Time: 16:27
 */



//通常情况下，我们在进程中一般用第一种方式操作消息队列

$message_queue_key = ftok(__FILE__, 'a');//时间证明，这里面的数据不能无限放。当数据到达最大个数的时候，那么再往里面放数据就会阻塞，等到有一条数据被取走才会断开阻塞！！！！！！

$message_queue = msg_get_queue($message_queue_key, 0666);//使用system V 信号 创建或连接一个消息队列

msg_send( $message_queue , 1 , "消息内容");//第二个参数 必须大于0。

msg_receive($message_queue, 0, $message_type, 1024 , $message , true,  MSG_IPC_NOWAIT);//MSG_IPC_NOWAIT 这个参数 是指 如果获取不到数据不会一直等待。不写这个就会一直等待

                                                                                         //第二个参数：

                                                                                                    //为0，那么就返回该队列中的第一个消息。每一个消息队列都是作为一个先进先出的链表维护，因此0，返回该队列中最早的消息。

                                                                                                    //大于0，那就返回其类型值为desiredmsgtype的第一个消息

                                                                                                    //小于0，那就返回其类型值小于或等于desiredmsgtype参数的绝对值的消息中类型最小的第一个消息

                                                                                         //第三个参数是 就固定放$message_type这个变量
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

$queue = new SplQueue();//普通队列，继承自SplDoublyLinkedList实现的队列模式,先进先出
/**
 * 可见队列和双链表的区别就是IteratorMode改变了而已，栈的IteratorMode只能为：
 * （1）SplDoublyLinkedList::IT_MODE_FIFO | SplDoublyLinkedList::IT_MODE_KEEP  （默认值,迭代后数据保存）
 * （2）SplDoublyLinkedList::IT_MODE_FIFO | SplDoublyLinkedList::IT_MODE_DELETE （迭代后数据删除）
 */
$queue->setIteratorMode(SplDoublyLinkedList::IT_MODE_FIFO | SplDoublyLinkedList::IT_MODE_DELETE);

$queue->enqueue("a");

$queue->enqueue("b");

$queue->enqueue("c");

print_r($queue->dequeue());//打出a,取了就没了

echo "-------";

while(!$queue->isEmpty()) {

    print_r($queue->dequeue());
}

foreach($queue as $item) {//打出bc
    echo $item . PHP_EOL;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

$stack = new SplStack();//继承自SplDoublyLinkedList实现的堆栈模式，先进后出

/**
 * 可见栈和双链表的区别就是IteratorMode改变了而已，栈的IteratorMode只能为：
 * （1）SplDoublyLinkedList::IT_MODE_LIFO | SplDoublyLinkedList::IT_MODE_KEEP  （默认值,迭代后数据保存）
 * （2）SplDoublyLinkedList::IT_MODE_LIFO | SplDoublyLinkedList::IT_MODE_DELETE （迭代后数据删除）
 */
$stack->setIteratorMode(SplDoublyLinkedList::IT_MODE_LIFO | SplDoublyLinkedList::IT_MODE_DELETE);

$stack->push('a');

$stack->push('b');

$stack->push('c');

$stack->offsetSet(0, 'first');//设置index为0的值为first，此时index为0的是c

var_dump($stack->pop()); //取出一个此时为first

foreach($stack as $item) {

    echo $item . PHP_EOL; //此时为b a

}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

$pq = new SplPriorityQueue();//数据优先级队列

$pq->insert('a', 10);//a为值，10为优先级，优先级越高越早被current取到

$pq->insert('b', 1);

$pq->insert('c', 80);

echo $pq->count() .PHP_EOL; //3

echo $pq->current() . PHP_EOL; //a,去除并不会删除

/**
 * 设置元素出队模式
 * SplPriorityQueue::EXTR_DATA 仅提取值
 * SplPriorityQueue::EXTR_PRIORITY 仅提取优先级
 * SplPriorityQueue::EXTR_BOTH 提取数组包含值和优先级
 */
$pq->setExtractFlags(SplPriorityQueue::EXTR_DATA);

while($pq->valid()) {

    print_r($pq->current());

    $pq->next();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

$maxHeap = new SplMaxHeap();//集成自SplHeap类，用于从大到小排序使用

$maxHeap->insert(10);

$maxHeap->insert(11);

$maxHeap->insert(9);

echo $maxHeap->top();  //11，这儿并不会取掉其中的值

echo $maxHeap->count(); //3

foreach( $maxHeap as $number ) {

    echo $number;

}


