<?php
	require_once "db_utils.php";
	
	function submit_bid($user_id, $good_id, $num_to_buy, $max_price, $money) {
	
		mysql_db_connect();
		
		if($max_price <= 1) {
			$max_price = 1;
		}
		
		$captain_id;
		$city_id;
		$captain_money;
		
		$sql = "select id,city_id,money from captains where user_id=$user_id";
		$result = mysql_query($sql);
		if($result && ($row = mysql_fetch_assoc($result))) {
			$captain_id = $row['id'];
			$city_id = $row['city_id'];
			$captain_money = $row['money'];
		} else {
			return array("success"=>"false", "message"=>"Cannot find the captain.");
		}
		
		if($captain_money < $money) {
			return array("success"=>"false", "message"=>"You don't have that much money.");
		}
		
		$sql = "select * from player_bids where user_id=$user_id and good_id=$good_id and city_id=$city_id";
		$result = mysql_query($sql);
		if($result && ($row = mysql_fetch_assoc($result))) {
			return array("success"=>"false", "message"=>"Previous bid must be claimed first.", "bid_id"=>$row['id']);
		}
		
		$sql = "insert into player_bids (user_id,good_id,city_id,to_buy,stock,max_price,balance) " . 
		"values($user_id,$good_id,$city_id,$num_to_buy,0,$max_price,$money)";
		mysql_query($sql);
		
		$sql = "update captains set money=money-$money where id=$captain_id";
		mysql_query($sql);
		
		$sql = "select bids.id as bid_id,goods.id as good_id," . 
		"bids.city_id as city_id,goods.name as good_name,(bids.to_buy - bids.stock) as quantity_unbought," . 
		"bids.stock as quantity_bought,bids.balance as balance,bids.max_price as max_price " . 
		"from goods inner join player_bids as bids on goods.id=bids.good_id " . 
		"where bids.user_id=$user_id and bids.city_id=$city_id and bids.good_id=$good_id";
		$result = mysql_query($sql);
		$bid;
		if($result && ($row = mysql_fetch_assoc($result))) {
			$bid = $row;
		}
		
		return array("success"=>"true","bid"=>$bid);
	}
?>