<?php

function average($a, $b) {
	return ($a + $b)/2;
}

function distance($a,$b) {
	return sqrt ($a*$a + $b*$b);
}


function color_string_from_rgb($r, $g, $b) {
	if($r > 255) $r = 255;
	if($g > 255) $g = 255;
	if($b > 255) $b = 255;
	if($r < 0) $r = 0;
	if($g < 0) $g = 0;
	if($b < 0) $b = 0;
	$r = (int)$r;
	$g = (int)$g;
	$b = (int)$b;
	

	$result = "#";
	if ($r < 16) $result .= '0';
	$result .= dechex($r);
	if ($g < 16) $result .= '0';
	$result .= dechex($g);
	if ($b < 16) $result .= '0';
	$result .= dechex($b);
	
	return $result;
}

function mix($secondary, $primary, $ratio) {
	/*
	$r = rand(0,1000);
	$ratio *= 1000;
	
	if($r < $ratio) {
		return $secondary;
	} else {
		return $primary;
	}
	*/
	if($ratio > .95)
		return 255;
	$ratio = 1 - $ratio;
	$a = $primary*$ratio;
	$b = $secondary*(1-$ratio);
	return $a + $b;
}

$width = 128;
$height = 128;

function circle($image_array,$code) {

	$width = count($image_array);
	$height = count($image_array[0]);

	$point_i = rand(0,$width);
	$point_j = rand(0,$height);
	$radius = rand(min($width,$height)/4,min($width,$height)/2);
	$r = rand(128,255);
	$g = rand(128,255);
	$b = rand(128,255);
	if($code %3 == 0) {
		$r = 255;
		$g = 128;
	}
	if($code %3 == 1) {
		$g = 255;
		$b = 128;
	}
	if($code %3 == 2) {
		$b = 255;
		$r = 128;
	}

	for($i = -$width; $i < $width*2; $i++) {
		for($j = -$height; $j < $height*2; $j++) {
			$vi = $i;
			$vj = $j;
			
			while($vi < 0) $vi += $width;
			while($vj < 0) $vj += $height;
			while($vi >= $width) $vi -= $width;
			while($vj >= $height) $vj -= $height;
			
			$d = distance($i-$point_i,$j-$point_j);
			
			if($d < $radius) {
				$image_array[$vi][$vj]['r'] = mix($r,$image_array[$vi][$vj]['r'],$d/$radius);
				$image_array[$vi][$vj]['g'] = mix($g,$image_array[$vi][$vj]['g'],$d/$radius);
				$image_array[$vi][$vj]['b'] = mix($b,$image_array[$vi][$vj]['b'],$d/$radius);
			}
		}
	}
	return $image_array;
}


$image_array = array();
for($i = 0; $i < $width; $i++) {
	$image_array[$i]  = array();
	for($j = 0; $j < $height; $j++) {
		$image_array[$i][$j] = array('r' => distance(abs($i-$width/2),abs($j-$height/2)), 'g' => distance(abs($i-$width/2),abs($j-$height/2)), 'b' => distance(abs($i-$width/2),abs($j-$height/2)));
	}
}
$image_array = circle($image_array,1);
$image_array = circle($image_array,2);
$image_array = circle($image_array,3);
//$image_array = circle($image_array,4);
//$image_array = circle($image_array,5);


// print the image
$img = new Imagick();
$img->newImage($width, $height, new ImagickPixel('transparent'));

$draw = new ImagickDraw(); 
$pixel = new ImagickPixel('red');


for($i = 0; $i < $width; $i++) {
	for($j = 0; $j < $height; $j++) {
		
		$pixel->setColor(color_string_from_rgb($image_array[$i][$j]['r'], $image_array[$i][$j]['g'], $image_array[$i][$j]['b']));
		$draw->setFillColor($pixel); 
		
		$draw->point($i,$j);
	}
}

$img->drawImage($draw);
$img->setImageFormat('png');

/* Set appropriate header for PNG and output the image */
header('Content-Type: image/png');
echo $img;

?>