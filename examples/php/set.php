<?php
$address = "tcp://127.0.0.1:5555";
$data = array("node_id" => 8, "value" => false);

$context = new ZMQContext(1);

$socket = new ZMQSocket($context, ZMQ::SOCKET_REQ);
$socket->connect("tcp://127.0.0.1:5555");

//Wait 1 second to make sure we are connected. We don't need to do this on every call!
sleep(1);
$socket->send(json_encode(array("node_id" => 12, "value" => false)));

$response = json_decode($socket->recv(), true);
if(!isset($response["status"]) || $response["status"] != "okay"){
    echo 'The server did not correctly receive the message';
}

//For demonstation purpose wait another second
sleep(1);

$socket->send(json_encode(array("node_id" => 12, "value" => true)));

$response = json_decode($socket->recv(), true);
if(!isset($response["status"]) || $response["status"] != "okay"){
    echo 'The server did not correctly receive the message';
}

