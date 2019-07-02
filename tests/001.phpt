--TEST--
Check if blurhash is loaded
--SKIPIF--
<?php
if (!extension_loaded('blurhash')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "blurhash" is available';
?>
--EXPECT--
The extension "blurhash" is available
