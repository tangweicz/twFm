<?php

namespace Libs;

class CObject{

    /*
     * @author tangwei
     *
     * @time 2017-04-17 09:20:33
     *
     * @function 处理数据库进行数据处理，并对返回的数据进行转化操作，具体是操作CDbUtil中的query方法
     *
     */
    protected static function query($sql, array $params = NULL, $changeToObj = false){

        $tablename = self::parseSql($sql);

        $result = CDbUtil::query($sql, $params);

        if($result === false || is_numeric($result)){//数据库操作结果有误或者是数字，那么这儿就直接返回false给用户
            if($result == false){
                return 0;
            }else{
                return $result;
            }
        }

        if($changeToObj){//如果需要把数据转换成对象，那么才会走到这儿

            if(is_array($result) && !empty($result)){

                $res = array();

                if(strpos($tablename,"_") !== false){

                    $classname = "Models\\";
                    $nameArray = explode("_", $tablename);
                    if(!empty($nameArray)){
                        foreach($nameArray as $key=>$value){
                            if($value != ""){
                                if($key > 0){
                                    $classname .= ucfirst($value);
                                }else{
                                    $classname .= $value;
                                }

                            }

                        }
                    }
                    $classname .= "Model";

                }else{

                    $classname = "Models\\".$tablename."Model";

                }

                foreach($result as $key=>$val){

                    if(is_array($val) && !empty($val)){

                        $selfclass = new $classname;//实例化自己，然后把自己扔到数组里面，外面就可以当成对象用了

                        foreach($val as $k=>$v){

                            $selfclass->$k = $v;

                        }

                        $res[] = $selfclass;

                    }

                }

                return $res;

            }else{

                return 0;

            }
        }else{//不需要将数据转换成对象，那么直接返回，都是数组，在写APP接口的时候可以使用

            return $result;

        }





    }

    /*
     * @author tangwei
     *
     * @time 2017-04-18 12:40:07
     *
     * @function 解析sql语句，从中拿到表名
     */
    private static function parseSql($sql)
    {
        $sqlcmd = strtoupper(substr(trim($sql), 0, 6));
        switch($sqlcmd) {
            case "SELECT":
                preg_match('/((SELECT.+?FROM)|(LEFT\\s+JOIN|JOIN|LEFT))[\\s`]+?(\\w+)[\\s`]+?/ismU', $sql, $matchs);
                return $matchs[4];

                break;
            case "INSERT":
                preg_match('/INSERT\\s+?INTO[\\s`]+?(\\w+)[\\s`]+?/ismU', $sql, $matchs);

                return $matchs[1];

                break;
            case "UPDATE":

                preg_match('/UPDATE[\\s`]+?(\\w+)[\\s`]+?/ismU', $sql, $matchs);

                return $matchs[1];

                break;

            case "DELETE":

                preg_match('/DELETE[\\s`]+?(\\w+)[\\s`]+?/ismU', $sql, $matchs);

                return $matchs[1];

                break;

        }
    }

}






//namespace Libs;
//
//class CObject{
//    /*
//     * 实例化对象，如果传入参数，则返回带参数的对象
//     */
//    public function __construct(array $fields = null){
//        if ($fields != null){
//            $class_name = get_class($this);
//            foreach($fields as $k=>$v) {
//
//                $this->$k = $v;
//
//            }
//        }
//    }
//
//    /*
//     * 具体是操作CDbUtil中的query方法
//     */
//    protected static function query($sql, array $params = NULL){
//
//        return CDbUtil::query($sql, $params);
//
//    }
//
//}

?>