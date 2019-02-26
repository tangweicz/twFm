<?php

namespace Libs;
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 15/5/26
 * Time: 13:24
 */

/* 文件上传类 */
class Upload{
    public $InputName = 'myfile';    // 文件提交的表单名（file栏的名称）

    public $FileType; //允许上传的图片类型

    public $userId;

    public $FileMaxSize; //数组，各类型数据最大上传限制

    public $FileSavePath;  // 文件保存路径（表示上传文件的路径）

    public $FileSaveName;  // 文件保存名（不包含后缀名）（可为数组形式，表示不同上传域上传文件保存的不同名称）

    public $NoteFileFalse = '文件错误'; // 文件错误提示

    public $NoteFileType  = '文件类型不符';  // 文件类型不符提示

    public $NoteFileSize  = '文件大小超出';  // 文件大小超出提示

    public function __construct($fromid){
        $this->userId = $fromid;
    }

    /* 上传文件并返回文件名信息（包含后缀名） */
    public function UploadFile()
    {

        $this->CheckFile(); // 检验文件

        $file = $_FILES[$this->InputName];

        $file_number = count($file['name']); // 要上传的文件数

        $file_save_full_name = array(); // 文件保存名（包含后缀名）

        for ($i = 0; $i < $file_number; $i++)
        {
            $name = $file['name'][$i];

            if (!empty($name)) // 文件不为空
            {
                /* 确定文件保存路径 */
                if (is_array($this->FileSavePath))
                {
                    $file_save_path = $this->FileSavePath[$i];
                }
                else
                {
                    $file_save_path = $this->FileSavePath;
                }

                /* 确定文件保存名（不包含后缀名） */

                $file_save_name = $this->userId."".time();

                /* 开始保存 */
                $this->CreatePath($file_save_path); // 如果路径不存在则创建路径


                if (!move_uploaded_file($file["tmp_name"][$i], $file_save_path . $file_save_name . $this->GetSuffix($file['name'][$i])))
                {

                    echo json_encode(array("state"=>"error","message"=>$name . '文件上传失败'));exit;
                }
                else
                {
                    sleep(1);
                    $file_save_full_name[] = $file_save_name . $this->GetSuffix($file['name'][$i]);
                }
            }
            else
            {
                $file_save_full_name[] = null;
            }
        }

        unset($file);
        /* 如果只有一个文件，则返回单个文件名 */
        if (count($file_save_full_name) == 1)
        {
            $file_save_full_name = $file_save_full_name[0];
        }

        return $file_save_full_name;
    }

    /* 检验文件 */
    private function CheckFile(){

        $file = $_FILES[$this->InputName];

        $file_number = count($file['name']); // 要上传的文件数

        for ($i = 0; $i < $file_number; $i++)
        {
            if (!empty($file['name'][$i])) // 文件不为空
            {

                $name  = $file['name'][$i];

                $type  = $file['type'][$i];

                $size  = $file['size'][$i];

                $error = $file['error'][$i];

                /* 确定最大上传文件大小 */
                if (is_array($this->FileMaxSize)){
                    $file_max_size_key = explode('/', $type);

                    $file_max_size_key = $file_max_size_key[0];

                    $file_max_size = $this->FileMaxSize[$file_max_size_key];
                }else {
                    $file_max_size = $this->FileMaxSize;
                }
                /* 文件大小超过最大上传文件大小 */
                if (!is_null($file_max_size) && $size > $file_max_size) {
                    echo json_encode(array("state"=>"error","message"=>$name . $this->NoteFileSize));exit;
                }
                /* 文件错误 */
                if ($error > 0)
                {
                    echo json_encode(array("state"=>"error","message"=>$name . $this->NoteFileFalse));exit;
                }
                /* 文件类型不符 */
                if (!in_array($type, $this->FileType))
                {
                    echo json_encode(array("state"=>"error","message"=>$name . $this->NoteFileType));exit;
                }
            }
        }
    }

    /* 获取文件后缀名 */
    private function GetSuffix($fileName){
        return substr($fileName, strrpos($fileName, "."));
    }

    /* 如果路径不存在则创建路径 */
    private function CreatePath($filePath){
        if (!file_exists($filePath))
        {
            mkdir($filePath,0777,true);
        }
    }
}
