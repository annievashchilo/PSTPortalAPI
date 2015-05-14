<?php



/**
 * Callback function for retrieve all test cases in specified scenario
 * @param $fn  
 *	REST method to perform. 
 * @param string $scenario 
 *	scenario name, from which you want to retrieve test cases
 * @return test cases
 */
function portal_api_retrieve($fn, $scenario) {
  // Check for mad values
  drupal_set_message('<pre>' . print_r($scenario, TRUE) . '</pre>');
  $scenario = strval($scenario);
  return 
  _portal_api_find_tc_items($scenario);
}


 /**
 * Function that retrieves testcases from database. Made just for learning.
 * @param string $scenario — 
 *	scenario name, from which you want to retrieve test cases
 * @return 
 *	all test cases in scenario
 */
function _portal_api_find_tc_items($scenario) {
	$servername = "localhost:3306";  //tmp
	$username = "root"; 			 //tmp
	$password = "";					 //tmp
	$dbname = "taf";				 //tmp

	$conn = new mysqli($servername, $username, $password, $dbname);
	// Check connection
	if ($conn->connect_error) die("Connection failed: " . $conn->connect_error);

	$query = 'SELECT testcase_id from test_cases WHERE scenario = ?';
	$stmt = mysqli_prepare($conn, $query);
	
	mysqli_stmt_bind_param($stmt, "s", $scenario);
	
	mysqli_stmt_execute($stmt);
	$result = mysqli_stmt_get_result($stmt);
	while ($row = mysqli_fetch_array($result, MYSQLI_NUM)) {
		foreach ($row as $r){
			print "$r";
		}
		print "\n";
	}
	mysqli_stmt_close($stmt);
	mysqli_close($conn);
}

/**
 * Callback for updating test cases resources .
 * @param $fn 
 *	REST method to perform
 * @param somefile.json $data 
 *	data file with 
 * @param string $customer
 * Example
 */
function portal_api_update($fn, $customer, $data) {
	// 'put' and 'create' are doing the same thing for now
	portal_api_create($fn, $data, $customer);
}

/**
 * Callback for creating json file with updated list of testcases
 * @param somefile.json $data 
 *	data file with 
 * @param string $customer
 * Example
 */
function portal_api_create($fn, $customer, $data) {
	global $user;
	
	$uploaddir = 'C:\Users\Hanna_Vashchilo\Desktop'; // tmp
	$filename_template = '\TestSuite';
	$file_type = '.json';

	$path = $uploaddir . $filename_template . $customer . $file_type;
	
	if (!$fp = fopen($path, 'w')) {
		echo 'Cant open the file ($path)';
		exit;
	}
	
	
	if (_fwrite_stream($fp, json_encode($data)) === FALSE) {
		echo 'Cant write to file ($path)';
		exit;
	}

	echo 'The file ' . $path . ' was successfully created';
	fclose($fp);

}

function _fwrite_stream($fp, $string) {
	$string = replace($string);
    for ($written = 0; $written < strlen($string); $written += $fwrite) {
        $fwrite = fwrite($fp, substr($string, $written));
        if ($fwrite === false) {
            return $written;
        }
    }
    return $written;
}

// Remove and replace the following:
// the first '{' -> ''
// the last  '}' -> ''
//	"data": -> ''
// the first "
// the last "
// ' -> "
function replace($contents) {
	$contents = substr($contents, 1, -1); 				  	// remove the first and the last curly brackets from contents
	$contents = str_replace('"data":', "", $contents);   	// remove "data" keyword from contents
	$contents = str_replace("'", '"', $contents);			// replace all single quotes with double quotes
	$contents = substr($contents, 1, -1); 					// then remove the first and the last unnecessary double quotes from contents
	
	return $contents;
}
