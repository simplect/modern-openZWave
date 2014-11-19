<?php

$context = new ZMQContext(1);

//  Socket to talk to clients
$client = new ZMQSocket($context, ZMQ::SOCKET_SUB);
$client->connect("tcp://127.0.0.1:5556");
$client->setSockOpt(ZMQ::SOCKOPT_SUBSCRIBE, "");

while (true) {
    //  Wait for next request from client
    $request = $client->recv();
    echo $request;

}
