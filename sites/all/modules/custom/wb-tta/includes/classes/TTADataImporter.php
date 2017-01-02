<?php

class TTADataImporter {

  private $baseDir;
  protected $agency ;

  public function setBaseDir($baseDir) {
    $this->baseDir = $baseDir;
  }

  public function setAgency($agency) {
    $this->agency = $agency;
  }

  public function getBaseDir() {
    return $this->baseDir;
  }

  public function getAgency($agency) {
    return $this->agency;
  }


  public function importTTAData() {

    try {
          $searchPath = $this->getBaseDir()."/*.csv";
          $timePointFiles = glob($searchPath);
          if ($timePointFiles === false || count($timePointFiles) == 0) {
                //$this->generateException("Could not locate TTA Time Points data matching: $searchPath");
            }

            foreach ($timePointFiles as $timePointFile) {
              $this->readCSV($timePointFile);
            }
        } catch (Exception $ex) {
            print_r("Exception added" . $ex->getMessage());
            //$this->generateException("Error reading GIS data file: ".$ex->getMessage());
        }
  }
  public function readCSV($filepath) {

    // Format columns map
    $csv = array_map('str_getcsv', file($filepath));
    $filename = basename($filepath);
    $service = array('SERVICE','Service Days','OPEN DATE', 'CLOSE DATE');
    // have to check what is LINE
   // $agency = array();
    $trip = array('DIRECTION');
    $route = array('RTID', 'RTENAME', 'LINE');
    $fare = array('RT Fare');
    $stop = array('TPNAME', 'STOPID', 'STOPLAT','STOPLNG'); // not sure TPNAME here signifies the As StopName
    $stop_timing = array('RUNS');
    $exception = array('RT Exception Dates');
    $data = array();
    foreach ($csv as $key => $value) {
      $value = array_filter($value);
      if (empty($value)) {
        continue;
      }
      $flip_array = array_flip($value);
      $needle = key($flip_array);
     // print_R($needle);
      if(in_array($needle, $service)) {
        $data['service'][] = $value;
      } else if (in_array($needle, $trip)) {
        $data['trip'][] = $value;
      } else if (in_array($needle, $route)) {
        $data['route'][] = $value;
      } else if (in_array($needle, $fare)) {
        $data['fare'][] = $value;
      } else if (in_array($needle, $stop)) {
        $data['stop'][] = $value;
      }  else if (in_array($needle, $exception)) {
          $data['exception'][] = $value;
      } else {
        $data['stop_timing'][] = $value;
      }
    }

    $service_data = $this->processService($data['service'], $filename);
    //$this->service($service_data);

    $agency_data = $this->processAgency();
//    $this->agency($agency_data);

    $route_data = $this->processRoute($data['route']);
//    $this->route($route_data);

    $fare_data = $this->processFare($data['fare']);
  //  $this->fare($fare_data);

    $stop_data = $this->processStop($data['stop']);
   // $this->stop($stop_data);

    $stop_timing_data = $this->processStopTimings($data['stop_timing'],$stop_data);
   // $this->stop_timings($stop_timing_data);

    $exception_data = $this->processException($data['exception']);
   // $this->exception($exception_data);

//    print_r($service_data);
   // print_r($agency_data);
    // print_r($route_data);
     // print_r($fare_data);
   //print_r($stop_data);
   //  print_r($stop_timing_data);
   // print_r($exception_data);

    dsm($service_data, "service_data");
    dsm($agency_data, "agency_data");
    dsm($route_data, "route_data");
    dsm($fare_data ,"fare_data");
    dsm($stop_data,"stop_data");
    dsm($stop_timing_data, "stop_timing_data");
    dsm($exception_data, "exception_data");
  }

  public function processService($service, $filename) {
    $sd = array('Service Days');
    $od = array('OPEN DATE');
    $cd = array('CLOSE DATE');

    foreach ($service as $key => $value) {
      $flip_array = array_flip($value);
      $needle = key($flip_array);
      if (in_array($needle, $sd)) {
        // Depends how the date is given
        $weeks = explode(",", $value[1]);
        foreach ($weeks as $weekday) {
          $weekday = trim(strtolower(trim($weekday,".")));
          switch ($weekday) {
            case 'sunday':
              $data['is_available_sunday'] = 1;
              break;
            case 'saturday':
              $data['is_available_saturday'] = 1;
              break;
            case 'monday':
              $data['is_available_monday'] = 1;
              break;
            case 'tuesday':
              $data['is_available_tuesday'] = 1;
              break;
            case 'wednesday':
              $data['is_available_wednesday'] = 1;
              break;
            case 'thursday':
              $data['is_available_thursday'] = 1;
              break;
            case 'friday':
              $data['is_available_friday'] = 1;
              break;
            default:
              # code...
              break;
          }
        }
      }  else if (in_array($needle, $od)) {
        $data['start_date'] = $value[1];
      } else if (in_array($needle, $cd)) {
        $data['end_date'] = $value[1];
      }
    }

    $days_array = array('is_available_sunday' => 'SUN', 'is_available_saturday' => 'SAT', 'is_weekday' =>'WKDY', 'is_weekend' => 'WKEND');
    // $is_sunday = "SUN";
    // $is_sat = "SAT";
    // $is_weekday = "";\
    foreach ($days_array as $key => $literal) {
          if (strpos($filename, $literal)) {
            $data[$key] = 1;
          }

    }
    return $data;
  }



  public function processAgency($agency) {
    return $agency['agency_name'] = $this->agency;
  }

  public function processRoute($route) {
   // return $route;
    $data = array();
    $rsc = array('LINE');
    $rid = array('RTID');
    $rn = array('RTENAME');
    foreach ($route as $key => $value) {
      $flip_array = array_flip($value);
      $needle = key($flip_array);
      if (in_array($needle, $rsc)) {
        $data['short_code'] = $value[1];
      }  else if (in_array($needle, $rid)) {
        $data['route_id'] = $value[1];
      } else if (in_array($needle, $rn)) {
        $data['route_name'] = $value[1];
      }
    }
    return $data;
  }


  public function processFare($fare){
    $data = array();
    $rsc = array('RT Fare');
    foreach ($fare as $key => $value) {
      $flip_array = array_flip($value);
      $needle = key($flip_array);
      if (in_array($needle, $rsc)) {
        $data['amount'] = $value[1];
      }
    }
    return $data;
  }


  public function processStop($stop) {
    $result = array();

    foreach ($stop as $key => $value) {
      $this->processStopMappings($result, $value, count($value));
    }
    return $result;
  }

  public function processStopMappings(&$result, $data, $col_count) {
    $sn = array('TPNAME');
    $sid = array('STOPID');
    $slat= array('STOPLAT');
    $slong = array('STOPLNG');
    $needle = $data[0];
    for ($inc = 0; $inc <= $col_count; $inc++){
        $assingment = !empty($data[$inc]) ? $data[$inc]: NULL;
      if (in_array($needle, $sn)) {
         $result[$inc]['stop_name'] = $assingment;
        }  else if (in_array($needle, $sid)) {
          $result[$inc]['stop_id'] = $assingment;
        } else if (in_array($needle, $slat)) {
          $result[$inc]['latitude'] = $assingment;
        } else if (in_array($needle, $slong)) {
          $result[$inc]['longitude'] = $assingment;
        }
    }
  }

  public function processStopTimings($stop_timings, $stop_data) {
    $data = array();
    $this->trimStopTimming($stop_timings);
    $keys = array('stop_sequence', 'direction');
    foreach ($stop_data as $key => $value) {
      if (isset($value['stop_id']) && $value['stop_id'] != 'STOPID') {
       $keys =  array_merge($keys, array($value['stop_id']));
      }
    }

    foreach ($stop_timings as $key => $value) {
      $data[] = array_combine($keys, $value);
    }
    return $data;
  }




  public function trimStopTimming(&$stop_timings) {
    $sruns = 'RUNS';
    $start_index = NULL;
    foreach ($stop_timings as $key => $value) {
      if(array_search($sruns, $value) !== false) {
        $start_index = $key;
      }
    }
    $stop_timings = array_slice($stop_timings, $start_index + 1);

  }

  public function processException($exception) {
    $data = array();
    $rexp = array('RT Exception Dates');
    foreach ($exception as $key => $value) {
      $flip_array = array_flip($value);
      $needle = key($flip_array);
      if (in_array($needle, $rexp)) {
        $data['exception_date'] = $value[1];
      }
    }
    return $data;
  }

  public function service($service) {

  }

  public function agency($agency) {

  }

  public function route($route) {

  }


  public function fare($fare){

  }


  public function stop($stop) {

  }

  public function stopTimings($stop_timing) {

  }

  public function exception($exception) {

  }

}


