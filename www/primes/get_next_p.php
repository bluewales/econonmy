<?php
	
	error_reporting(E_ALL);
	ini_set('display_errors', 1);
	
	//echo gmp_strval(gmp_nextprime("1000000"));
	//exit(0);

	// create table mersenne ( id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY, p int, duration int, prime bool, last_updated timestamp, started datetime default null, progress int default 0);
	
	//select id,p,duration,prime, concat(truncate(progress / (p/100),3),"%") as percent from mersenne where started is not null;

	date_default_timezone_set("America/Denver");

	$conn = new mysqli("localhost", "root", "rootpass", "primes");
	
	$conn->query("LOCK TABLES mersenne WRITE");
	
	$sql = "select p from mersenne where started is null and prime is null order by id asc limit 1";
	$result = $conn->query($sql);
	
	if($row = $result->fetch_assoc()) {
		$p = $row['p'];
    } else {
		$sql = "select max(p) as p from mersenne";
		$result = $conn->query($sql);
		
		if($row = $result->fetch_assoc()) {
			$p = $row['p'];
			$p = gmp_strval(gmp_nextprime("" . ($p + 1)));
			
			$sql = "insert into mersenne (p) values($p)";
			$conn->query($sql);
		}
	}
	
	$sql = "update mersenne set started=CURRENT_TIMESTAMP where p=$p";
	$conn->query($sql);
	
	echo $p;
?>