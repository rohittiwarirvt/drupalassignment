<?php

function _all_wb_tta_api_index($agency) {
  //agencies
  $query = db_select('agencies','a');
  $query->addField('a', 'id','agency_id');
  $query->addField('a', 'agency_name','agency_name');
  $query->addField('a', 'agency_id','actual_agency_id');

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



