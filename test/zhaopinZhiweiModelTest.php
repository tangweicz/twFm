<?phpuse Models\zhaopinZhiweiModel;class zhaopinZhiweiModelTest extends PHPUnit_Framework_TestCase {    /**     * User: tangwei     * Date: 2019.03.26     * @param      * @return     * Function:测试新增一条记录     */    public function testaddOne()    {        $model = new zhaopinZhiweiModel();        $model->zhiweiName = "xxxx";        $model->isDeleted = 0;        $res = $model->addOne();        var_dump($res);        #./vendor/bin/phpunit --filter testaddOne ./test/zhaopinZhiweiModelTest.php    }    /**     * User: tangwei     * Date: 2019.03.26     * @param      * @return     * Function:测试删除记录的方法，不进回收站，直接删数据库的记录     */    public function testdeleteOne()    {        $record = zhaopinZhiweiModel::getOneById(10);        if(empty($record)){            echo "搜索无数据";            exit;        }        $record = $record[0];        $res = $record->deleteOne();        var_dump($res);        #./vendor/bin/phpunit --filter testdeleteOne ./test/zhaopinZhiweiModelTest.php    }    /**     * User: tangwei     * Date: 2019.03.26     * @param      * @return     * Function:根据ID获取一条记录     */    public function testgetOneById()    {        $record = zhaopinZhiweiModel::getOneById(1);        var_dump($record);        #./vendor/bin/phpunit --filter testgetOneById ./test/zhaopinZhiweiModelTest.php    }    /**     * User: tangwei     * Date: 2019.03.26     * @param      * @return     * Function:IN查询，根据ID获取一条记录     */    public function testgetAllUndeletedByIdUseIn()    {        $record = zhaopinZhiweiModel::getAllUndeletedByIdUseIn('1,2,3,4');        var_dump($record);        #./vendor/bin/phpunit --filter testgetAllUndeletedByIdUseIn ./test/zhaopinZhiweiModelTest.php    }    /**     * User: tangwei     * Date: 2019.03.26     * @param      * @return     * Function:根据ID获取一条记录     */    public function testgetOneByIdForUpdate()    {        $record = zhaopinZhiweiModel::getOneByIdForUpdate(1);        var_dump($record);        #./vendor/bin/phpunit --filter testgetOneByIdForUpdate ./test/zhaopinZhiweiModelTest.php    }    /**     * User: tangwei     * Date: 2019.03.26     * @param      * @return     * Function:删除记录的方法，进回收站，不是直接删数据库的记录     */    public function testdeleteUpdateOne()    {        $record = zhaopinZhiweiModel::getOneById(1);        if(empty($record)){            echo "搜索无数据";            exit;        }        $record = $record[0];        $res= $record->deleteUpdateOne();        var_dump($res);        #./vendor/bin/phpunit --filter testdeleteUpdateOne ./test/zhaopinZhiweiModelTest.php    }    /**     * User: tangwei     * Date: 2019.03.26     * @param      * @return     * Function:不带limit的获取所有的没被删除的数据     */    public function testgetAllUndeletedWithOutLimit()    {        $record = zhaopinZhiweiModel::getAllUndeletedWithOutLimit();        var_dump($record);        #./vendor/bin/phpunit --filter testgetAllUndeletedWithOutLimit ./test/zhaopinZhiweiModelTest.php    }    /**     * User: tangwei     * Date: 2019.03.26     * @param      * @return     * Function:带limit的获取所有的没被删除的数据     */    public function testgetAllUndeletedWithLimit()    {        $record = zhaopinZhiweiModel::getAllUndeletedWithLimit("limit 0,1");        var_dump($record);        #./vendor/bin/phpunit --filter testgetAllUndeletedWithLimit ./test/zhaopinZhiweiModelTest.php    }    /**     * User: tangwei     * Date: 2019.03.26     * @param      * @return     * Function:不带limit的获取所有的被删除的数据     */    public function testgetAllDeletedWithOutLimit()    {        $record = zhaopinZhiweiModel::getAllDeletedWithOutLimit();        var_dump($record);        #./vendor/bin/phpunit --filter testgetAllDeletedWithOutLimit ./test/zhaopinZhiweiModelTest.php    }    /**     * User: tangwei     * Date: 2019.03.26     * @param      * @return     * Function:带limit的获取所有的被删除的数据     */    public function testgetAllDeletedWithLimit()    {        $record = zhaopinZhiweiModel::getAllDeletedWithLimit("limit 0,1");        var_dump($record);        #./vendor/bin/phpunit --filter testgetAllDeletedWithLimit ./test/zhaopinZhiweiModelTest.php    }    /**     * User: tangwei     * Date: 2019.03.26     * @param      * @return     * Function:测试编辑一条记录     */    public function testeditOne()    {        $record = zhaopinZhiweiModel::getOneById(1);        if(empty($record)){            echo "搜索无数据";            exit;        }        $record = $record[0];        $record->zhiweiName = "xxxx";        $record->isDeleted = 10;        $res = $record->editOne();        var_dump($res);        #./vendor/bin/phpunit --filter testeditOne ./test/zhaopinZhiweiModelTest.php    }}