<?php
	function hash_password($_password,$_salt) {
		$_password = str_rot13($_password);
		$_password = $_password.$_salt;

		for($i = 0; $i < 100; $i++) {
			$_password = hash("sha512",$_password,false);
			$_password = hash("whirlpool",$_password,false);
		}

		$_password = base64_encode($_password);

		return $_password;
	}

	function randomString($length) {
		$string = openssl_random_pseudo_bytes($length);
		
		$string = base64_encode($string);
		
		return substr($string,0,$length);
	}
?>