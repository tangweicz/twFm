<?php

namespace Libs;

use Onoi\HttpRequest\CurlRequest;

use Onoi\HttpRequest\Exception\BadHttpResponseException;

use Onoi\HttpRequest\Exception\HttpConnectionException;

/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 15/10/14
 * Time: 09:17
 */

class HttpRequest {

    private $curlRequest = null;

    public function __construct( CurlRequest $curlRequest ) {
        $this->curlRequest = $curlRequest;
    }

    public function makeHttpRequestTo( $url ) {

        $this->curlRequest->setOption( CURLOPT_URL, $url );

        //if ( !$this->curlRequest->ping() ) {
        //    throw new HttpConnectionException( "Couldn't connect" );
        //}
        $this->curlRequest->setOption( CURLOPT_RETURNTRANSFER, true );

        $this->curlRequest->setOption( CURLOPT_HTTPHEADER, array(
            //'Accept: application/x-turtle'
            //"content-type: application/x-www-form-urlencoded;charset=UTF-8"

        ) );

        $response = $this->curlRequest->execute();

        if ( $this->curlRequest->getLastErrorCode() == 0 ) {
            return $response;
        }

        throw new BadHttpResponseException( $this->curlRequest );
    }


    public function makeHttpPostRequestTo($url, array $arr)
    {
        $this->curlRequest->setOption( CURLOPT_URL, $url );

        $this->curlRequest->setOption( CURLOPT_POST, 1);

        $this->curlRequest->setOption(CURLOPT_POSTFIELDS, $arr);

        $this->curlRequest->setOption( CURLOPT_RETURNTRANSFER, true );

//        $this->curlRequest->setOption( CURLOPT_HTTPHEADER, array(
//            "Content-Type:multipart/form-data"
//        ) );

        $response = $this->curlRequest->execute();

        if ( $this->curlRequest->getLastErrorCode() == 0 ) {
            return $response;
        }

        throw new BadHttpResponseException( $this->curlRequest );
    }
}