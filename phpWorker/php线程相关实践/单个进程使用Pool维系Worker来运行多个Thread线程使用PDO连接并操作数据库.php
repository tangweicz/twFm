<?php


    class myWorker extends Worker{

        public static $dbh;

        public function run()
        {

            $dsn = "mysql:dbname=ceshi;host=127.0.0.1;port=3306";

            $username = "root";

            $password = "";

            try{

                self::$dbh = new PDO($dsn, $username, $password);

            }catch(PDOException $e){

                print __METHOD__.": line ".__LINE__.": cannot connect db. ".$e->getMessage()." \n";

                exit(-1);

            }



        }

        public function getDbh()
        {

            return self::$dbh ;

        }

    }

    class MyThread extends Thread{

        public $sql;

        public $isComplete = false;

        public function __construct($sql)
        {

            $this->sql = $sql;

        }

        public function run(){

            echo "运行MyThread的run function.......\r\n";

            try{

                $threadDbh = $this->worker->getDbh();

                $stmt = $threadDbh->prepare($this->sql);

                $stmt->execute();

                $rows = $stmt->fetchAll(PDO::FETCH_ASSOC);

                var_dump($rows);

            }catch(PDOException $e){

                print __METHOD__.": line ".__LINE__.": 操作错误. ".$e->getMessage()." \n";

                exit(-1);

            }

            $this->isComplete = true;

        }

    }

$pool = new Pool(2, "myWorker");

for($i = 0; $i < 2; $i++){

    $pool->submit(new MyThread("select * from members"));

}

$pool->collect(function($work){

    return $work->isComplete;

});

$pool->shutdown();