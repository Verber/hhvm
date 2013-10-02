<?php

require_once('config.inc');

$dbh = pg_connect($conn_str);
$tbl_name = 'test_47199';
@pg_query("DROP TABLE $tbl_name");
pg_query("CREATE TABLE $tbl_name (null_field INT, not_null_field INT NOT NULL)");

pg_insert($dbh, $tbl_name, array('null_field' => null, 'not_null_field' => 1));
pg_insert($dbh, $tbl_name, array('null_field' => null, 'not_null_field' => 2));

var_dump(pg_fetch_all(pg_query('SELECT * FROM '. $tbl_name)));

$query = pg_delete($dbh, $tbl_name, array('null_field' => NULL,'not_null_field' => 2), PGSQL_DML_STRING|PGSQL_DML_EXEC);

echo $query, "\n";

$query = pg_update($dbh, $tbl_name, array('null_field' => NULL, 'not_null_field' => 0), array('not_null_field' => 1, 'null_field' => ''), PGSQL_DML_STRING|PGSQL_DML_EXEC);

echo $query, "\n";

var_dump(pg_fetch_all(pg_query('SELECT * FROM '. $tbl_name)));

@pg_query("DROP TABLE $tbl_name");
pg_close($dbh);

echo PHP_EOL."Done".PHP_EOL;

?>