<?php
/* Read the image */
$im = new Imagick("dice.png");

$textures = new Imagick("minecraft_texture.jpg");

$rock = clone $textures;

$rock->cropImage ( 16 , 16 , 80 , 96 );

/* Create an empty canvas */
$canvas = new Imagick();

/* Canvas needs to be large enough to hold the both images */
$width = 128;
$height = 64;
$canvas->newImage($width, $height, new ImagickPixel("black"));
$canvas->setImageFormat("png");

/* Composite the original image and the reflection on the canvas */

for($i = 0; $i < $width; $i += 16) {
	for($j = 0; $j < $height; $j += 16) {
		$canvas->compositeImage($rock, imagick::COMPOSITE_OVER, $i, $j);
	}
}

$canvas->resizeImage(1024,512,0,1);

$width = 1024;
$height = 512;


$shifts = array();
for($i = 0; $i < $width; $i += 1) {
	$shifts[$i] = array();
	for($j = 0; $j < $height; $j += 1) {
		$offset = 128;
		$x = $width/2 + $offset/2;
		$y = $height/2;
		$distance = sqrt (($x-$i)*($x-$i) + ($y-$j)*($y-$j));
		
		if($i > $offset) {
			$depth = 0;
			if($distance <= 250) { 
				$depth = sqrt(((250-$distance)*2));
			} else {
				$depth = 0;
			}
			$shifts[$i][$j] = $shifts[$i-$offset][$j]-$depth;
		} else {
			$shifts[$i][$j] = 0;
		}
	}
}


$draw = new ImagickDraw(); 
$pixel = new ImagickPixel('red');


for($i = 0; $i < $width; $i += 1) {
	for($j = 0; $j < $height; $j += 1) {
		$x = 512;
		$y = 256;
		$distance = sqrt (($x-$i)*($x-$i) + ($y-$j)*($y-$j));
		$offset = 128;
		if($i > 128) {
			$pixel = $canvas->getImagePixelColor(($i-$shifts[$i][$j])%$offset, $j);
			$draw->setFillColor($pixel);
			$draw->point($i,$j);
		}
	}
}

$canvas->drawImage($draw);


/* Output the image*/
header("Content-Type: image/png");
echo $canvas;
?>