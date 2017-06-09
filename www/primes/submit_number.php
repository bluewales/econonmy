<?php

	error_reporting(E_ALL);
	ini_set('display_errors', 1);

	// create table mersenne ( id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY, p int, duration int, prime bool, last_updated timestamp, started datetime default null, progress int default 0);

	date_default_timezone_set("America/Denver");

	$conn = new mysqli("localhost", "root", "rootpass", "primes");

	//GET /primes/submit_number.php?p=%d&progress=%d&time=%d
	//GET /primes/submit_number.php?p=%d&result=%s&time=%d
	if(isset($_GET['progress'])) {
		$sql = "update mersenne set progress=" . $_GET['progress'] . ", duration=" . $_GET['time'] . " where p=" . $_GET['p'];
	}

	if(isset($_GET['result'])) {
		$sql = "update mersenne set prime=" . $_GET['result'] . ", duration=" . $_GET['time'] . ", progress=" . $_GET['p'] . " where p=" . $_GET['p'];
	}

	$result = $conn->query($sql);

?>
