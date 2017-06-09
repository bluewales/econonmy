<?php
$servername = "localhost";
$username = "root";
$password = "rootpass";
$dbname = "test";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
if(isset($_GET['side']) && isset($_GET['die'])) {
	$sql = "update dice_rolls set n=n+1 where side=" . $_GET['side'] . " and die='" . $_GET['die'] . "'";

	$result = $conn->query($sql);
}

$sql = "select side,n from dice_rolls";

$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "side: " . $row["side"] . " : " . $row["n"] . "<br>";
    }
} else {
    echo "0 results";
}

$sql = "select sum(x) as chi, sum(n) as rolls from (select side, (n-exp)*(n-exp)/exp as x, n from dice_rolls cross join (select sum(n)/count(*) as exp from dice_rolls) as avg) as xs";

$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "<br>chi: " . $row["chi"] . ", rolls: " . $row["rolls"] . "<br>";
    }
} else {
    echo "0 results";
}

$conn->close();
?>
<style>
table, th, td {
    border: 1px solid black;

}
td{
	padding:10px;
}
</style>
<br>
<table>
<tr><td><td>0.90<td>0.95<td>0.975<td>0.99<td>0.999
<tr><td>19<td>27.204<td>30.144<td>32.852<td>36.191<td>43.820
</table>