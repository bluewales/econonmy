<?php

$GIF = new Imagick();
$GIF->setFormat("gif");

	$textures = new Imagick("minecraft_texture.jpg");
	$rock = clone $textures;
	$rock->cropImage ( 16 , 16 , 0 , 16 );
	
	$diamond_ore = clone $textures;
	$diamond_ore->cropImage ( 16 , 16 , 32 , 48 );
	
	$brick = clone $textures;
	$brick->cropImage ( 16 , 16 , 80 , 96 );
	

	$frame = new Imagick();
	$frame->newImage(16, 16, new ImagickPixel('red'));
	$frame->compositeImage($diamond_ore, imagick::COMPOSITE_OVER, 0, 0);
	$frame->resizeImage(128,128,0,1);
	$frame->setImageFormat('gif');
    $frame->setImageDelay(10);
    $GIF->addImage($frame);
	
	for($i = 0; $i < 10; $i++) {
		
		$crack = clone $textures;
		$crack->cropImage ( 16 , 16 , $i * 16 , 240 );
	
		$frame = new Imagick();
		$frame->newImage(16, 16, new ImagickPixel('purple'));
		$frame->compositeImage($diamond_ore, imagick::COMPOSITE_OVER, 0, 0);
		$frame->compositeImage($crack, imagick::COMPOSITE_OVER, 0, 0);
		$frame->resizeImage(128,128,0,1);
		$frame->setImageFormat('gif');
		$frame->setImageDelay(10);
		$GIF->addImage($frame);
	}
	

header("Content-Type: image/gif");
echo $GIF->getImagesBlob();

?>