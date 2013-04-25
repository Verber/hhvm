<?php

/* Prototype  : string nl_langinfo  ( int $item  )
 * Description: Query language and locale information
 * Source code: ext/standard/string.c
*/

echo "*** Testing nl_langinfo() : basic functionality ***\n";

$original = setlocale(LC_ALL, 'C');

var_dump(nl_langinfo(ABDAY_2));
var_dump(nl_langinfo(DAY_4));
var_dump(nl_langinfo(ABMON_7));
var_dump(nl_langinfo(MON_4));
var_dump(nl_langinfo(RADIXCHAR));

setlocale(LC_ALL, $original); 
?>
===DONE===