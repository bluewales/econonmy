<?php



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


$width = 512;
$height = 512;




// print the image
$img = new Imagick();
$img->newImage($width, $height, new ImagickPixel('transparent'));

$draw = new ImagickDraw(); 
$pixel = new ImagickPixel('red');


for($i = 0; $i < $width; $i++) {
	for($j = 0; $j < $height; $j++) {
		
		$pixel->setColor(color_string_from_rgb(100, 150, 200));
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