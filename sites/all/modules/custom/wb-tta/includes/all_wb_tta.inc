<?php


//first api callback
function _wb_tta_agency_api_index($agency) {
  //agencies
  $query = db_select('agencies','a');
  $query->addField('a', 'id','agency_id');
  $query->addField('a', 'agency_name','agency_name');
  $query->addField('a', 'agency_id','actual_agency_id');
  $query->condition('a.agency_name', $agency,'=');
  //routes
  $query->leftJoin('routes', 'r','a.id=r.agency_id');
  //routes-field
  $query->addField('r', 'id','route_id');
  $query->addField('r', 'route_name','route_name');
  $query->addField('r', 'route_id','actual_route_id');

  //route_fares
  $query->leftJoin('route_fares', 'rf','rf.route_id=r.id');

  //route-fares-field
  $query->fields('rf', array('origin_id','destination_id','amount'));
  $query->addField('rf', 'id','fare_id');

  $result = $query->execute()
          ->fetchAll(PDO::FETCH_ASSOC) ;

  array_walk($result, "_wb_rate_random");



  if (!empty($result)) {
      // There is data to return!
      $resposne_array = array();
      $resposne_array['status'] = 'success';
      $resposne_array['code'] = '600';
      $resposne_array['message'] = 'GTFS data found.';
      $resposne_array['data'] = $result;
      return $resposne_array;
    } else {
      // There is no data to return...so let's notify this case
      $err_msg = 'No GTFS Data found.';
      return format_error('601', 'error', $err_msg);
    }
}


function _wb_rate_random(&$result, $key) {
   $result['amount'] = is_null($result[$key]['amount']) ? rand(2,7) : $result[$key]['amount'];
}

// all trips
// stop_timings left join stops

//first api callback
function _wb_tta_route_stops_api_index($route_id,$agency) {
  //agencies
  // return $agency;
  $query = db_select('agencies','a');
  // $query->addField('a', 'id','agency_id');
  // $query->addField('a', 'agency_name','agency_name');
  // $query->addField('a', 'agency_id','actual_agency_id');
  if (isset($agency)) {
    $query->condition('a.agency_name', $agency,'=');
  }

  //addding distinct
  $query->distinct();
  //routes
  $query->leftJoin('routes', 'r','a.id=r.agency_id');
  //routes-field
  $query->addField('r', 'id','route_id');
  $query->addField('r', 'route_name','route_name');
  $query->addField('r', 'route_id','actual_route_id');

  if (isset($route_id)) {
    $query->condition('r.id', $route_id,'=');
  }
  //route_fares
  // $query->leftJoin('route_fares', 'rf','rf.route_id=r.id');

  // agency left join
  //
  $query->leftJoin('trips', 't','t.route_id=r.id');
  $query->addField('t', 'id','trip_id');


  // $query->addField('a', 'agency_name','agency_name');
  // $query->addField('a', 'agency_id','actual_agency_id');


  //routes
  $query->leftJoin('stop_times', 'st','t.id=st.trip_id');
  //routes-field
  // $query->addField('r', 'id','route_id');
  // $query->addField('r', 'route_name','route_name');
  // $query->addField('r', 'route_id','actual_route_id');

  //route_fares
  $query->leftJoin('stops', 's','s.stop_id=st.stop_id');

  //route-fares-field
  $query->fields('s', array('stop_id','stop_name','latitude','longitude'));
  // $query->addField('rf', 'id','fare_id');
  $query->groupBy('s.stop_id');

  $result = $query->execute()
          ->fetchAll(PDO::FETCH_ASSOC) ;

  if (!empty($result)) {
      // There is data to return!
      $resposne_array = array();
      $resposne_array['status'] = 'success';
      $resposne_array['code'] = '600';
      $resposne_array['message'] = 'GTFS data found.';
      $resposne_array['data'] = $result;
      return $resposne_array;
    } else {
      // There is no data to return...so let's notify this case
      $err_msg = 'No GTFS Data found.';
      return format_error('601', 'error', $err_msg);
    }
}


// all trips
// stop_timings left join stops

//first api callback
function _wb_tta_route_trips_stops_api_index($route_id, $agency) {

  $agency_route_table = "{$agency}_gtfs_routes";
  $agency_stops_table = "{$agency}_gtfs_stops";
  $query = db_select('agencies','a');
  if (isset($agency)) {
    $query->condition('a.agency_name', $agency,'=');
  }
  //$query->distinct();
  $query->leftJoin('routes', 'r','a.id=r.agency_id');
  //routes-field
  $query->addField('r', 'id','route_id');
  $query->addField('r', 'route_name','route_name');
  $query->addField('r', 'route_id','actual_route_id');

  if (isset($route_id)) {
    $query->condition('r.id', $route_id,'=');
  }

  $result1 = $query->execute()
          ->fetchAll(PDO::FETCH_ASSOC) ;

  $result= array();

  foreach ($result1 as  $key => $route) {
    $query = db_select('trips','t');
    $query->addField('t', 'id','trip_id');
    $query->fields('t', array('direction_id', 'direction_name'));
    $query->condition('t.route_id', $route['route_id'],'=');
    $query->leftJoin('services', 'serv','t.service_id=serv.id');
    $query->fields('serv');
  //routes
    $query->leftJoin('stop_times', 'st','t.id=st.trip_id');
    $query->fields('st');
  //routes-field

  //route_fares
    $query->leftJoin('stops', 's','s.stop_id=st.stop_id');
    $query->fields('s', array('stop_id','stop_name','latitude','longitude'));
    if (db_table_exists($agency_stops_table)) {
      $query->leftJoin($agency_stops_table, 'ls','ls.stop_code=s.stop_id');
      $query->addField('ls', 'longitude','l_longitude');
      $query->addField('ls', 'latitude','l_latitude');
    }
    $trips = $query->execute()
          ->fetchAll(PDO::FETCH_ASSOC) ;

    $trips = _wb_tta_group_by($trips,'trip_id');

    //get routes
    if (db_table_exists($agency_route_table)) {
     $rtquery = db_select($agency_route_table, 'lr');
     $rtquery->fields('lr', array('route_url', 'pdf_file_id','short_name'));
     $short_name = str_ireplace('rt ', '', $route['actual_route_id']);
     $rtquery->condition('lr.short_name', '%' . db_like($short_name) . "%", 'LIKE');
     $lg_routes = $rtquery->execute()->fetchAssoc();
    // $lg_routes);
     if( $lg_routes['pdf_file_id']) {
       $file = file_load($lg_routes['pdf_file_id']);
     }
     $route['file_url'] = isset($file) ? file_create_url($file->uri) : NULL;
     $route['route_url'] = $lg_routes['route_url'];
    }
    $result['routes'][$key] = $route;


    $i = 0;
    foreach ($trips as  $trip_key=> $stops) {
            $result['routes'][$key]['trips'][$i] = _wb_array_get(current($stops), array('trip_id', 'direction_id','direction_name'));
            $result['routes'][$key]['trips'][$i]['services'] =_wb_array_get(current($stops), array('service_name','is_available_monday','is_available_tuesday','is_available_wednesday','is_available_thursday','is_available_friday','is_available_saturday','is_available_sunday','start_date','end_date'));

      foreach ($stops as $stop_key => $stop) {

        $result['routes'][$key]['trips'][$i]['stop_times'][$stop_key] =_wb_array_get($stop, array('stop_id', 'arrival_time','stop_sequence','trip_id'));
        $wb_stop = _wb_array_get($stop, array('stop_id', 'stop_code','stop_name','latitude', 'longitude','created'));
        $wb_stop['latitude'] =  intval($wb_stop['latitude']) ? $wb_stop['latitude'] :  $stop['l_latitude'];
        $wb_stop['longitude'] =  intval($wb_stop['longitude']) ? $wb_stop['longitude'] :  $stop['l_longitude'];
        $result['routes'][$key]['trips'][$i]['stop_times'][$stop_key]['stops'] = $wb_stop;
      }
      $i++;
    }
  }
  if (!empty($result)) {
      // There is data to return!
      $resposne_array = array();
      $resposne_array['status'] = 'success';
      $resposne_array['code'] = '600';
      $resposne_array['message'] = 'GTFS data found.';
      $resposne_array['data'] = $result;
      return $resposne_array;
    } else {
      // There is no data to return...so let's notify this case
      $err_msg = 'No GTFS Data found.';
      return format_error('601', 'error', $err_msg);
    }
}

