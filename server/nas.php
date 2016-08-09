<?php
if(isset($_GET['action'])){
	$json = array();
	switch($_GET['action']){
		case 'noshutdown':
			$file = fopen('tmp/noshutdown', 'w');
			$result = fclose($file);
			$json['result'] = $result; 
		break;
		case 'shutdown':
			$result = true;
			if(file_exists('tmp/noshutdown')){
				$result = unlink('tmp/noshutdown');
			}
			$json['result'] = $result;
		break;
		default:
			//By default, «result» is set to «false»
			$json['result'] => false));
		break;
	}
	//Dans tous les cas, on affiche le status
	$json['shutdown'] = file_exists('tmp/noshutdown');
	echo json_encode($json);
}
