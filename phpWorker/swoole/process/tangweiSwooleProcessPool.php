<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/9/22
 * Time: 20:24
 *
 * SIGCHLD SIGTERM SIGALAM三种信号
 */

class processPool
{
    private $_pro;

    private $_minProNum = 3;//初始化进程之后fork出来的子进程数量

    private $_maxProNum = 6;//能够后续添加的最大的子进程数量

    private $_proList = array();//存放所有的fork出来的子进程

    private $_usedProList = array();//存放所有的被fork出来的子进程，用value=1/0来表示该进程是否正在使用中

    public function __construct()
    {
        $this->_pro = new swoole_process(array($this, 'run'), false, true);

        $this->_pro->start();

    }

    public function run()
    {
        for($i=0; $i<$this->_minProNum; $i++){

            $process = new swoole_process(array($this, 'task_run'), false, true);

            $pid = $process->start();

            $this->_proList[$pid] = $process;

            $this->_usedProList[$pid] = 0;

        }

        if(!empty($this->_proList)){//在父进程中接收子进程写入管道的数据，在这儿用于判断一个进程是否已经完成工作了

            foreach($this->_proList as $pid=>$process){

                swoole_event_add($process->pipe, function($pipe) use($process){

                    $data = $process->read();

                    var_dump($data."\r\n");

                    $this->_usedProList[$data] = 0;

                });

            }

        }

        swoole_timer_tick(1000, function($timer_id){

            static $index = 0;

            $index ++;

            $canTurn = true;

            if(!empty($this->_proList)){

                foreach($this->_proList as $pid=>$proce){

                    if($this->_usedProList[$pid] == 0){

                        $canTurn = false;

                        $proce->write("hello");

                        $this->_usedProList[$pid] = 1;
                    }

                }

            }

            if($canTurn && $this->_minProNum < $this->_maxProNum){

                $process = new swoole_process(array($this, 'task_run'), false, true);

                $pid = $process->start();

                $this->_proList[$pid] = $process;

                $this->_usedProList[$pid] = 1;

                $process->write("hello");

                $this->_minProNum ++;

            }

            if($index == 10){

                if(!empty($this->_proList)){

                    foreach($this->_proList as $pid => $pro){

                        $pro->write("exit");

                    }

                }

                swoole_timer_clear($timer_id);

                $this->_pro->exit();

            }

        });


    }
    public function task_run($worker)
    {

        swoole_event_add($worker->pipe,function($pipe) use($worker){

            $data = $worker->read();

            var_dump($worker->pid.$data."\r\n");

            if($data == "exit"){

                $worker->exit();

                exit;

            }

            sleep(5);

            $worker->write($worker->pid);

        });

    }
}

new processPool();

