<?php

class TTADataImporter {

  private $baseDir;
  protected $agency, $agency_id;

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

    $trip = array('DIRECTION');
    $route = array('RTID', 'RTENAME', 'LINE');
    $fare = array('RT Fare','FARE');
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

    dsm($data);
    return "";

    $service_data = $this->processService($data['service'], $filename);
    $service = $this->service($service_data);
    drupal_set_message("Imported Service with Service ID $service");

    $agency_data = $this->processAgency();
    $agency = $this->agency($agency_data);
    drupal_set_message("Imported Agency with Agency ID $agency");

    $route_data = $this->processRoute($data['route'], $filename);
    $route = $this->route($route_data);
    drupal_set_message("Imported Route with Route ID $route");


    $trip_data = $this->processTrip($data['trip'], $service, $route);
    $trip = $this->trip($trip_data);

    $stop_data = $this->processStop($data['stop']);
    $this->stop($stop_data);


    $stop_timing_data = $this->processStopTimings($data['stop_timing'],$stop_data);
    $stop_timing_data['trip_id'] = $trip;
    $this->stopTimings($stop_timing_data, $outputs);

    $exception_data = isset($data['exception']) ? $this->processException($data['exception']) : NULL;
   // $this->exception($exception_data);

    $fare_data = isset($data['fare']) ? $this->processFare($data['fare']) : NULL;
    $fare_data = array_merge(array('route_id' => $route), $outputs, $fare_data);
    $fare = $this->fare($fare_data);
    //trips
    drupal_set_message("Imported Succesfully $filename");

  }

  public function processTrip($trip_data, $service_id, $route_id) {
    $dname = array('DIRECTION');
    foreach ($trip_data as $key => $value) {
      $flip_array = array_flip($value);
      $needle = key($flip_array);
      if (in_array($needle, $dname)) {
        $trip['direction_name'] = $value[1];
      }
    }

    $trip['service_id'] = $service_id;
    $trip['route_id'] = $route_id;
    return $trip;
  }


  public function processService($service, $filename) {
    $sd = array('Service Days');
    $od = array('OPEN DATE');
    $cd = array('CLOSE DATE');
    $sn = array('SERVICE');

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
      } else if (in_array($needle, $sn)) {
        $data['service_name'] = $value[1];
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

    // weekdays logic
    $weekday = array('is_available_monday', 'is_available_tuesday', 'is_available_wednesday', 'is_available_thursday', 'is_available_friday');
    $weekend = array('is_available_sunday', 'is_available_saturday');
    if (array_key_exists('is_weekday',$data)) {
      $data = array_merge($data, array_combine($weekday, array_fill(0, 5, 1)));
      unset($data['is_weekday']);
    }

    // weekend logic
    if (array_key_exists('is_weekend', $data)) {
      $data = array_merge($data, array_combine($weekend, array_fill(0, 2, 1)));
      unset($data['is_weekend']);
    }

    return $data;
  }



  public function processAgency() {
     $agency['agency_name'] = $this->agency;
     return $agency;
  }

  public function processRoute($route, $filename) {
   // return $route;
    $data = array();
    $rsc = array('LINE');
    $rid = array('RTID');
    $rn = array('RTENAME');
    foreach ($route as $key => $value) {
      $flip_array = array_flip($value);
      $needle = key($flip_array);
      if (in_array($needle, $rsc)) {
       // $data['short_code'] = $value[1];
      }  else if (in_array($needle, $rid)) {
        $data['route_id'] = $value[1];
      } else if (in_array($needle, $rn)) {
        $data['route_name'] = $value[1];
      }
    }

    $file_chuck = explode("-", $filename);
    $data['route_id'] = !isset($data['route_id']) ? trim($file_chuck[0]) : $data['route_id'];
    $data['route_type'] = 1;
    return $data;
  }


  public function processFare($fare){
    $data = array();
    $rsc = array('RT Fare', 'FARE');
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
    $result = array_filter(array_map('array_filter', $result));
    array_shift($result);
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
         $result[$inc]['stop_name'] = $result[$inc]['stop_code'] = $assingment;
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


    $service['start_date'] = format_date(strtotime($service['start_date']), 'custom', 'Y-m-d 00:00:00');
    $service['end_date'] = format_date(strtotime($service['end_date']), 'custom', 'Y-m-d 00:00:00');
    $service['service_id'] = $service['service_name'];


    $exist = db_select('services', 's')
    ->fields('s', array('id'))
    ->condition('service_id', $service['service_id'])
    ->execute()
    ->fetchAssoc();

    if (empty($exist)) {
      $route_id = db_insert('routes')
                  ->fields($route)
                  ->execute();
    }
    $service_id = isset($service_id) ? $service_id :  !empty($exist) ? $exist['id'] : NULL;
    return $service_id;
  }

  public function agency($agency = array()) {
    $agency['created'] =  empty($agency['created']) ? format_date(time(), 'custom', 'Y-m-d 00:00:00') : $agency['created'];
    $agency = array_merge(['agency_name' => $this->agency, 'agency_id' => $this->agency], $agency);


    $exist = db_select('agencies', 'a')
    ->fields('a', array('id'))
    ->condition('agency_id', $agency['agency_id'])
    ->execute()
    ->fetchAssoc();


    if (empty($exist)){
     $agency_id = db_insert('agencies')
                 ->fields($agency)
                  ->execute();
    }
    $this->agency_id = isset($agency_id) ? $agency_id: !empty($exist) ? $exist['id'] : NULL;
    return $this->agency_id;
  }

  public function route($route) {

    $route['agency_id'] = $this->agency_id;

    $exist = db_select('routes', 'r')
    ->fields('r', array('id'))
    ->condition('route_id', $route['route_id'])
    ->execute()
    ->fetchAssoc();
    if (empty($exist)) {
      $route_id = db_insert('routes')
                  ->fields($route)
                  ->execute();
    }
    $route_id = isset($route_id) ? $route_id :  !empty($exist) ? $exist['id'] : NULL;
    return $route_id;
  }


  public function fare($fare){
    $exist = db_select('route_fares', 'rf')
    ->fields('rf', array('id'))
    ->condition('route_id', $fare['route_id'])
    ->execute()
    ->fetchAssoc();
    if (empty($exist)) {
      $fare_id = db_insert('route_fares')
                  ->fields($fare)
                  ->execute();
    }
    $fare_id = isset($fare_id) ? $fare_id :  !empty($exist) ? $exist['id'] : NULL;
    return $fare_id;
  }


  public function stop($stops) {
     foreach ($stops as $key => $stop) {
      $stop_id = db_merge('stops')
                    ->key(array('stop_id' => $stop['stop_id'])) // Table name no longer needs {}
                    ->fields($stop)
                    ->execute();
    }

    return true;
  }

  public function stopTimings($stop_timings, &$options = array()) {
    $trip_id = $stop_timings['trip_id'];
    unset($stop_timings['trip_id']);
     foreach ($stop_timings as $key => $stop_timing) {
      $st_chunk1 = array_slice($stop_timing,0, 2);
      $st_chunk2 = array_slice($stop_timing,2, NULL, TRUE);
      // code for fare source and destination
      end($st_chunk2);
      $options['origin_id'] = key ($st_chunk2);
      // code for fare destionation
      reset($st_chunk2);
      $options['destination_id'] = key($st_chunk2);
       foreach ($st_chunk2 as $stopid => $time) {
        $time =  format_date(strtotime($time), 'custom',  'H:i:s');
        $stop_fields = array_merge($st_chunk1, ['stop_id' =>$stopid, 'arrival_time' => $time, 'trip_id' => $trip_id]);
        unset($stop_fields['direction']);
       $stop_id = db_merge('stop_times')
                      ->key(array('stop_id' => $stopid, 'arrival_time' => $time)) // Table name no longer needs {}
                      ->fields($stop_fields)
                      ->execute();
      }

    }
    return true;
  }

  public function trip($trip) {
    $exist = db_select('trips_back', 't')
    ->fields('t', array('id'))
    ->condition('service_id', $trip['service_id'])
    ->condition('route_id', $trip['route_id'])
    ->execute()
    ->fetchAssoc();

    if (empty($exist)) {
      $trip_id = db_insert('trips_back')
                  ->fields($trip)
                  ->execute();
    }
    $trip_id = isset($trip_id) ? $trip_id :  !empty($exist) ? $exist['id'] : NULL;
    return $trip_id;
  }

  public function exception($exception) {

  }

}


