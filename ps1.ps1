$ctx = "C:\"
$num = 250;
echo "start" "";
$txt = ls $ctx -r -name "*";

echo "writing" "";
for($i = 0; $i -lt $num; $i++){
	do{
		$rand = random $txt;
		$rand = $ctx + $rand;
	} while((test-path $rand -pathType container));

	echo $rand;
	$file = "G:\workspace\tests\#$i";
	cpi -destination $file -path $rand; 
	ac -path $file -value "";
	ac -path $file -value $rand;
}
echo "" "end";
