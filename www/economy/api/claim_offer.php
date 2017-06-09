<?php
	require_once "db_utils.php";
	
	function claim_offer($user_id, $offer_id) {
	
		mysql_db_connect();
		
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
		
			$sql = "select offers.id as offer_id,goods.id as good_id," . 
			"offers.city_id as city_id,goods.name as good_name,(offers.to_sell - offers.stock) as quantity_sold," . 
			"offers.stock as quantity_unsold,offers.balance as balance,offers.min_price as min_price " . 
			"from goods inner join player_offers as offers on goods.id=offers.good_id " . 
			"where offers.user_id=$user_id and offers.id=$offer_id";
			$result = mysql_query($sql);
			$offer;
			if($result && ($row = mysql_fetch_assoc($result))) {
				$offer = $row;
			} else {
				return array("success"=>"false", "message"=>"Offer does not exist.");
			}
			
			
			$sql = "update captains set money=money+".$offer['balance']." where id=$captain_id";
			$result = mysql_query($sql);
			
			if($offer['quantity_unsold'] > 0) {
				$sql = "update inventories set count=count+".$offer['quantity_unsold']." where captain_id=$captain_id and good_id=".$offer['good_id'];
				$result = mysql_query($sql);
				if(mysql_affected_rows () == 0) {
					$sql = "insert into inventories (captain_id,good_id,count) values($captain_id,".$offer['good_id'].",".$offer['quantity_unsold'].")";
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
		
		
		$sql = "delete from player_offers where user_id=$user_id and id=$offer_id";
		$result = mysql_query($sql);
		
		
		return array("success"=>"true","offer"=>$offer);
	}
?>