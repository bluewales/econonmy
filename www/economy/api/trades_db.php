<?php

require_once "db_utils.php";

function mysql_db_create_trades_tables() {
	$con = mysql_db_connect();

	$sql = "drop table player_offers";
	mysql_query($sql);
	
	$sql = "create table player_offers (" .
	"id int auto_increment," . 
	"user_id int," .
	"good_id int," .
	"city_id int," .
	"to_sell int," .
	"stock int," .
	"min_price int," .
	"balance int default 0," .
	"created TIMESTAMP DEFAULT CURRENT_TIMESTAMP," .
	"primary key(id))";
	mysql_query($sql);
	
	$sql = "drop table player_bids";
	mysql_query($sql);
	
	$sql = "create table player_bids (" .
	"id int auto_increment," . 
	"user_id int," .
	"good_id int," .
	"city_id int," .
	"to_buy int," .
	"stock int," .
	"max_price int," .
	"balance int default 0," .
	"created TIMESTAMP DEFAULT CURRENT_TIMESTAMP," .
	"primary key(id))";
	mysql_query($sql);

	$sql = "drop table players";
	mysql_query($sql);
	
	$sql = "create table players (" .
	"id int auto_increment," . 
	"user_id int," .
	"money int default 10000," .
	"ship_id int," .
	"created TIMESTAMP DEFAULT CURRENT_TIMESTAMP," .
	"primary key(id))";
	mysql_query($sql);	
	
}

?>