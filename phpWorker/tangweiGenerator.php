<?php

/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/10
 * Time: 14:14
 *
 * 协程代码概念的理解，这部分是基础
 *
 * 在php7中，yield关键字左右的“()”是不需要的，但是在低于php7的版本中，“()”是必须的
 *
 * 协程的调用类似于函数的调用，不同于进程和线程，协程不占用系统CPU资源，协程的调度取决于系统的内存。
 *
 * 协程的本质是，在同步编码的同时，实现异步调用。
 *
 * 协程三要素：
 *          1、本质是事件循环
 *          2、需要有个调度器
 *          3、可执行队列
 *
 * 执行流程：
 *          程序创建协程---------->载入协程到可执行队列----------->调度器拿到队列中的协程去运行，当遇到阻塞逻辑（yield的功能）的时候会回归到本质挂载到事件循环中去，调度器会去取下一个可执行的协程
 */

class sys{

    public function __construct(){
        echo 123;
    }

    public function m(){

        echo 456;
    }
}

function test(){

    return 123456789;
}

function gen()
{
    $ret = (yield 'yield1');//当外部调用send()时，从yield这儿往后的代码会暂停执行，并且接收到send过来的参数，并把yield后面的参数返回给调用者

    var_dump($ret);

    $ret = (yield test());

    var_dump($ret);
}

$gen = gen();

$gen->rewind();//走到第一个yield处等待执行，如果不写也没关系，因为gen()的时候会隐式调用该方法

var_dump($gen->current());//获取到当前yield处的值

//$gen->next();//从第一个yidle往下执行，直到遇到下一个yield处等待。

$m = $gen->send("hahaha");//将hahaha传递到当前的yield，并继续往下执行。

var_dump($m);

//$m->m();



//---------------------------------我写的协程开始

class Task
{
    private $coroutine;

    private $isFirstYield = true;

    public function __construct($task)
    {
        $this->coroutine = $task;
    }

    public function run()
    {
        if($this->isFirstYield){

            $this->isFirstYield = false;

            $this->coroutine->current();

        }else{

            $this->coroutine->send(10);

        }
    }

    public function isValid()
    {
        return $this->coroutine->valid();
    }
}

class diaoduTask
{
    private $queue;

    public function __construct()
    {
        $this->queue = new splQueue();
    }

    public function addTask($func)
    {
        $task = new Task($func);

        $this->queue->enqueue($task);
    }

    public function run()
    {
        while(!$this->queue->isEmpty()){

            $task = $this->queue->dequeue();

            $task->run();

            if($task->isValid()){

                $this->queue->enqueue($task);

            }

        }
    }
}

function task1(){
    for($i=0;$i<=10;$i++){

        echo "this task is task1 de $i \r\n";

        yield;
    }
}

function task2(){

    for($i=0;$i<=5;$i++){

        echo "this task is task2 de $i \r\n";

        yield;
    }
}

$diaodu = new diaoduTask();

$diaodu->addTask(task1());

$diaodu->addTask(task2());

$diaodu->run();

//---------------------------------我写的协程完毕


//---------------------------------我写的协程高阶代码开始
class systemCall
{
    private $callback;

    public function __construct($func)
    {
        $this->callback = $func;
    }

    public function __invoke($task, $diaodu)
    {
        $callback = $this->callback;

        return $callback($task, $diaodu);
    }
}

class tasks
{
    public $task;

    public $taskId;

    public $isFirstYield = true;

    public function __construct($task, $taskId)
    {
        $this->task = $task;

        $this->taskId = $taskId;
    }

    public function run()
    {
        if($this->isFirstYield){

            $this->isFirstYield = false;

            return $this->task->current();

        }else{

            return $this->task->send($this->taskId);

        }
    }

    public function isValid()
    {
        return $this->task->valid();
    }
}

class diaodu
{
    public $queue;

    public $maxTaskId = 0;

    public $task;

    public function __construct()
    {
        $this->queue = new splQueue();
    }

    public function newTask($task)
    {
        $this->maxTaskId = $this->maxTaskId + 1;

        $this->task = new tasks($task, $this->maxTaskId);

        $this->addTask($this->task);
    }

    public function addTask($task)
    {
        $this->queue->enqueue($task);
    }

    public function run()
    {
        while(!$this->queue->isEmpty()){

            $task = $this->queue->dequeue();

            $systemCall = $task->run();

            if($systemCall instanceof systemCall){

                $systemCall($task, $this);

                continue;

            }

            if($task->isValid()){

                $this->queue->enqueue($task);

            }

        }
    }

    public function killTask($taskId)
    {
        if(!$this->queue->isEmpty()){

            foreach($this->queue as $key=>$task){

                if($task->taskId == $taskId){

                    unset($this->queue[$key]);

                }

            }

        }
    }
}

function getTaskId(){
    return new systemCall(function($task, $diaodu){

        $diaodu->addTask($task);

    });
}

function newTask($coroutine){

    return new systemCall(function($task, $diaodu)use($coroutine){

        $task->taskId = $task->taskId + 1;

        $diaodu->newTask($coroutine);

        $diaodu->addTask($task);

    });
}

function killTask($taskId)
{
    return new systemCall(function($task, $diaodu)use($taskId){

        $diaodu->killTask($taskId);

        $diaodu->addTask($task);

    });
}

function childTask()
{
    $tid = (yield getTaskId());

    while(true){

        echo "childTask $tid is still alive \r\n";

        yield;

    }
}

function task()
{
    $tid = (yield getTaskId());

    $childTaskId = (yield newTask(childTask()));

    for($i = 1; $i<=6; $i++){

        echo "task $tid de $i haha \r\n";

        yield;

        if($i == 3) (yield killTask($childTaskId));

    }
}

$diaodu = new diaodu();

$diaodu->newTask(task());

$diaodu->run();

//---------------------------------我写的协程高阶代码完毕