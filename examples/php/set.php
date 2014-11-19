<?php
$address = "tcp://127.0.0.1:5555";
$data = array("node_id" => 8, "value" => false);

$context = new ZMQContext(1);

$publisher = new ZMQSocket($context, ZMQ::SOCKET_PUB);
$publisher->connect("tcp://127.0.0.1:5555");

//Wait 1 second to make sure we are connected. We don't need to do this on every call!
sleep(1);
echo 'sending: ' . json_encode($data) . "\n";

$publisher->send(json_encode($data));

//For demonstation purpose wait another second

sleep(1);
$data["value"] = true;

echo 'sending: ' . json_encode($data) . "\n";
$publisher->send(json_encode($data));
