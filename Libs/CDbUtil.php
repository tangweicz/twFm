<?php

namespace Libs;

class CDbUtil {

    static public $dbh = NULL;

    /*
     * 初始化数据库连接
     */
    function __construct() {
        $db = require __DIR__."/../Config/Config.php";
        if (self::$dbh == NULL) {
            $dsn = sprintf("mysql:dbname=%s;host=%s;port=%d", $db['dbname'], $db['host'], $db['port']);
            try {
                self::$dbh = new \PDO($dsn, $db['user'], $db['pass']);
				self::$dbh->query("set names utf8");//设置读取的字符编码
            }
            catch(PDOException $e) {
                print __METHOD__.": line ".__LINE__.": cannot connect db. ".$e->getMessage()." \n";
                exit(-1);
            }
        }
    }

    /*
     * 基础的数据sql语句操作
     */
    public static function query($sql, array $params = NULL) {

        if(!self::$dbh instanceof \PDO) new self;

        $sql = trim($sql);

        if (empty($sql)) return NULL;

        try {
            $stmt = self::$dbh->prepare($sql);
            if ($params == NULL) {
                $stmt->execute();
            }
            else {
                $stmt->execute($params);
            }

            $sqlcmd = strtoupper(substr($sql, 0, 6));
            switch($sqlcmd) {
                case "SELECT":
                    $rows = $stmt->fetchAll(\PDO::FETCH_ASSOC);
                    if (empty($rows)) return false;
                    return $rows;
                    break;
                case "INSERT":
                    return self::$dbh->lastInsertId();
                    break;
                case "UPDATE":
					$rowCount=$stmt->rowCount();
					if($rowCount<0)return false;
					return $rowCount;
                case "DELETE":
                    return $stmt->rowCount();

            }
        }
        catch(PDOException $e) {
            print __METHOD__." line ".__LINE__.": ".$e->getMessage()."\n";
            exit(-1);
        }
    }

    public function beginTransaction()
    {

        self::$dbh->beginTransaction();

    }

    public function commit()
    {

        self::$dbh->commit();

    }

    public function rollBack()
    {

        self::$dbh->rollBack();

    }
}
