<?php
	require_once "db_utils.php";
	
	function claim_bid($user_id, $bid_id) {
	
		$db = mysql_db_connect();
		
		$captain_id;
		$city_id;
		
		$sql = "select id,city_id from captains where user_id=$user_id";
		$result = mysql_query($sql);
		if($result && ($row = mysql_fetch_assoc($result))) {
			$captain_id = $row['id'];
			$city_id = $row['city_id'];
		} else {
			return array("success"=>"false", "message"=>"Cannot find captain.");
		}
		
		
		
		try {
			// First of all, let's begin a transaction
			mysql_query("START TRANSACTION");
		
		
			$sql = "select bids.id as bid_id,goods.id as good_id," . 
			"bids.city_id as city_id,goods.name as good_name,(bids.to_buy - bids.stock) as quantity_unbought," . 
			"bids.stock as quantity_bought,bids.balance as balance,bids.max_price as max_price " . 
			"from goods inner join player_bids as bids on goods.id=bids.good_id " . 
			"where bids.user_id=$user_id and bids.id=$bid_id";
			$result = mysql_query($sql);
			$bid;
			if($result && ($row = mysql_fetch_assoc($result))) {
				$bid = $row;
			} else {
				return array("success"=>"false", "message"=>"Bid does not exist.");
			}
			
			
			$sql = "update captains set money=money+".$bid['balance']." where id=$captain_id";
			$result = mysql_query($sql);
			
			if($bid['quantity_bought'] > 0) {
				$sql = "update inventories set count=count+".$bid['quantity_bought']." where captain_id=$captain_id and good_id=".$bid['good_id'];
				$result = mysql_query($sql);
				if(mysql_affected_rows () == 0) {
					$sql = "insert into inventories (captain_id,good_id,count) values($captain_id,".$bid['good_id'].",".$bid['quantity_bought'].")";
					$result = mysql_query($sql);	
				}
			}
		
			mysql_query("COMMIT");
		} catch (Exception $e) {
			// An exception has been thrown
			// We must rollback the transaction
			mysql_query("ROLLBACK");
			return array("success"=>"false", "message"=>"Transaction failed.");
		}
		
		$sql = "delete from player_bids where user_id=$user_id and id=$bid_id";
		$result = mysql_query($sql);
		
		return array("success"=>"true","bid"=>$bid);	
	}
?>