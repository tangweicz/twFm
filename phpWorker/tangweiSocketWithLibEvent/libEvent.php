<?php
/**
 * Created by PhpStorm.
 * User: tangwei
 * Date: 16/11/30
 * Time: 10:00
 */

class libEvent
{
    public $baseEvent;//存放基础事件

    const EV_READ = 1;

    const EV_WRITE = 2;

    const EV_SIGNAL = 3;

    const EV_TIMER = 4;

    const EV_TIMER_ONCE = 5;

    public $_eventTimer = array();

    public $_eventSignal = array();

    public $_event = array();

    /*
     * @author tangwei
     *
     * @time 2016-11-30 10:06:33
     *
     * @function 构造方法，用于初始化基础事件库
     */
    public function __construct()
    {
        $this->baseEvent = event_base_new();
    }

    /*
     * @author tangwei
     *
     * @time 2016-11-30 10:07:00
     *
     * @function 把不同的事件加入到基础事件库
     *
     * @param $fd流句柄 $flag事件类型 $func回调方法 $args回调方法的参数
     */
    public function add($fd, $flag, $func, $args)
    {
        switch($flag){

            case self::EV_READ://读事件注册入epoll

                $event = event_new();

                if(!event_set($event, $fd, EV_READ | EV_PERSIST, $func)){

                    return false;
                }

                if(!event_base_set($event, $this->baseEvent)){

                    return false;
                }

                if(!event_add($event)){

                    return false;
                }
                $this->_event[$fd][$flag] = $event;

                return true;

                break;

            case self::EV_WRITE://写事件注册入epoll
                $event = event_new();

                if(!event_set($event, $fd, EV_WRITE | EV_PERSIST, $func)){

                    return false;
                }

                if(!event_base_set($event, $this->baseEvent)){

                    return false;
                }

                if(!event_add($event)){

                    return false;
                }

                $this->_event[$fd][$flag] = $event;

                return true;

                break;

            case self::EV_TIMER:
                $event = event_new();

                $timer_id = (int)$event;

                if(!event_set($event, 0, EV_TIMEOUT, array($this, "timeOutCallBack"), $timer_id)){

                    return false;

                }

                if(!event_base_set($event, $this->baseEvent)){

                    return false;

                }

                $time_interval = $fd * 1000000;

                if(!event_add($event, $time_interval)){

                    return false;

                }

                $this->_eventTimer[$timer_id] = array($func, (array)$args, $event, $flag, $time_interval);

                return $timer_id;

                break;

            case self::EV_TIMER_ONCE://超时事件注册入epoll，所谓的超时指socket或者其他流在epoll中未触发任何事件

                $event = event_new();

                $timer_id = (int)$event;

                if(!event_set($event, 0, EV_TIMEOUT, array($this, "timeOutCallBack"), $timer_id)){

                    return false;

                }

                if(!event_base_set($event, $this->baseEvent)){

                    return false;

                }

                $time_interval = $fd * 1000000;

                if(!event_add($event, $time_interval)){

                    return false;

                }

                $this->_eventTimer[$timer_id] = array($func, (array)$args, $event, $flag, $time_interval);

                return $timer_id;

                break;

            case self::EV_SIGNAL://检测信号时间触发机制

                $this->_eventSignal[$fd] = event_new();

                if(!event_set($this->_eventSignal[$fd], $fd, EV_SIGNAL | EV_PERSIST, $func)){

                    return false;

                }

                if(!event_base_set($this->_eventSignal[$fd], $this->baseEvent)){

                    return false;

                }

                if(!event_add($this->_eventSignal[$fd])){

                    return false;

                }

                return true;

                break;

            default:

                $real_flag = $flag === self::EV_READ ? EV_READ | EV_PERSIST : EV_WRITE | EV_PERSIST;

                $event = event_new();

                if(!event_set($event, $fd, $real_flag, $func)){

                    return false;
                }

                if(!event_base_set($event, $this->baseEvent)){

                    return false;
                }

                if(!event_add($event)){

                    return false;
                }

                $this->_event[$fd][$flag] = $event;

                return true;

                break;

        }
    }

    /*
     * @author tangwei
     *
     * @time 2016-11-30 11:14:18
     *
     * @function 超时时间回调
     */
    public function timeOutCallBack($_null1, $_null2, $timer_id)
    {
        if ($this->_eventTimer[$timer_id][3] === self::EV_TIMER) {

            event_add($this->_eventTimer[$timer_id][2], $this->_eventTimer[$timer_id][4]);

        }

        try {

            call_user_func_array($this->_eventTimer[$timer_id][0], $this->_eventTimer[$timer_id][1]);

        } catch (\Exception $e) {

            exit(250);

        } catch (\Error $e) {

            exit(250);
        }
        if(isset($this->_eventTimer[$timer_id]) && $this->_eventTimer[$timer_id][3] === self::EV_TIMER_ONCE){

            $this->del($timer_id, self::EV_TIMER);

        }
    }

    /*
     * @author tangwei
     *
     * @time 2016-11-30 11:21:00
     *
     * @function 从epoll中删除事件
     */
    public function del($event, $flag)
    {
        switch($flag){

            case self::EV_TIMER:

                event_del($this->_eventTimer[$event][2]);

                unset($this->_eventTimer[$event]);

            case self::EV_SIGNAL:

                event_del($this->_eventSignal[$event]);

                unset($this->_eventSignal[$event]);

            case self::EV_READ:

                event_del($this->_event[$event]);

                unset($this->_event[$event]);

            case self::EV_WRITE:

                event_del($this->_event[$event]);

                unset($this->_event[$event]);

        }

        return true;
    }

    /*
     * @author tangwei
     *
     * @time 2016-11-30 12:44:20
     *
     * @function start loop
     */
    public function goLoop()
    {
        event_base_loop($this->baseEvent);
    }


}