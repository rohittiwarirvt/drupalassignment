<?php


// require_once(TTA_CLASSES_PATH . "GoCaryGTFSConstants.php");
require_once(TTA_CLASSES_PATH . "TTADataRetriever.php");
require_once(TTA_CLASSES_PATH . "TTADataImporter.php");
// require_once(TTA_CLASSES_PATH . "GoCaryBusRouteDAO.php");


function wb_tta_admin_form_callback() {
  return drupal_get_form('wb_tta_admin_form');
}



function wb_tta_admin_form($form, &$form_state) {
    // Container for upload-based inputs
    $form['tta_upload_container'] = array(
        '#type' => 'container',
    );

    $form['tta_upload_container']['agency'] =  array(
      '#type' => 'radios',
      '#title' => 'Please select Agency',
      '#required' => TRUE,
      '#description' => t('Select Appropriate Agency you want to Import.'),
      '#options' => tta_options_agencies(),
     );

    $form['tta_upload_container']['tta_file_upload'] = array(
        '#name' => 'files[tta_file]',
        '#type' => 'file',
        '#title' => t('GTFS Data (.zip)'),
    );

    // Form submit actions
    $form['actions']['update_ttadata'] = array(
        '#type' => 'submit',
        '#value' => t('Update TTA Data'),
        '#submit' => array('handle_update_ttadata_submit')
    );

    $form['actions']['cancel'] = array(
        '#type' => 'button',
        '#value' => t('Cancel'),
    );

    return $form;
}


/**
 * Agency options info callback.
 */
function tta_options_agencies() {
  $agencies = array(
    'godurham' => t("GoDurham"),
    'gotriangle' => t("GoTriangle"),
    'gocary' => t("GoCary"),
    'gochapelhill' => t("GoChapelHill"),
    'goraleigh' => t("GoRaleigh"),
  );
  return $agencies;
}
function wb_tta_admin_form_validate($form, &$form_state) {
  $file = file_save_upload('tta_file', array('file_validate_extensions' => array('zip')));
  if ($file) {
    $form_state['storage']['tta_file_upload'] = $file;
  } else {
    form_set_error('tta_gtfs_file', t('TTA  file was not uploaded.'));
  }

}


function handle_update_ttadata_submit($form, &$form_state) {
  // extracter
  $agency = $form_state['values']['agency'];
  $dataRetriever = new TTADataRetriever($agency);
  $tta_zip_filepath = drupal_realpath($form_state['storage']['tta_file_upload']->uri);
  $dataRetriever->extractFiles($tta_zip_filepath);

  //
  $dataImporter = new TTADataImporter();
  $dataImporter->setAgency($agency);
  $dataImporter->setBaseDir($dataRetriever->TTAFileDir());

  $_SESSION['http_request_count'] = 0;
  $files = $dataImporter->geFilesImportTTAData();
  $params['files'] = $files;
  $params['dataimporter'] = $dataImporter;

  $batch = batch_example_batch($params);
  batch_set($batch);
  batch_process("/ttaimport/routes");
  return "import";
}

function wb_tta_admin_form_callback_action() {
  $agency = "gotriangle";
  $route_id = 1;
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
    $trips = $query->execute()
          ->fetchAll(PDO::FETCH_ASSOC) ;

    $trips = _wb_tta_group_by($trips,'trip_id');
    $result['routes'][$key] = $route;
    foreach ($trips as  $trip_key=> $stops) {
      foreach ($stops as $stop_key => $stop) {
         if (!isset($result['routes'][$key]['trips'][$trip_key])){
            $result['routes'][$key]['trips'][$trip_key] = _wb_array_get($stop, array('trip_id', 'direction_id','direction_name'));
          }
         if (!isset($result['routes'][$key]['trips'][$trip_key]['services'])) {
            $result['routes'][$key]['trips'][$trip_key]['services'] =_wb_array_get($stop, array('service_name','is_available_monday','is_available_tuesday','is_available_wednesday','is_available_thursday','is_available_friday','is_available_saturday','is_available_sunday','start_date','end_date'));
          }
        $result['routes'][$key]['trips'][$trip_key]['stop_times'][$stop_key] =_wb_array_get($stop, array('stop_id', 'arrival_time','stop_sequence','trip_id'));
        $result['routes'][$key]['trips'][$trip_key]['stop_times'][$stop_key]['stops'] =_wb_array_get($stop, array('stop_id', 'stop_code','stop_name','latitude', 'longitude','created'));
      }


    }
  }

  dsm($result);
  //  // extracter
  //  $agency = "gotriangle";
  // // $dataRetriever = new TTADataRetriever($agency);
  // // // $tta_zip_filepath = drupal_realpath($form_state['storage']['tta_file_upload']->uri);
  // // // $dataRetriever->extractFiles($tta_zip_filepath);

  // // //
  // $dataImporter = new TTADataImporter();
  // $dataImporter->setAgency($agency);
  // $dataImporter->setBaseDir("sites/default/files/tta_scratch/gotriangle/demo/");


  // $_SESSION['http_request_count'] = 0;
  // $files = $dataImporter->geFilesImportTTAData();
  // foreach ($files as $key => $file) {
  //   $dataImporter->readCSV($file->uri);
  // }
  // drupal_goto("/");
  // $params['files'] = $files;
  // $params['dataimporter'] = $dataImporter;


  // if (isset($params) && !empty($params['files'])) {
  //   $batch = batch_example_batch($params);
  //   batch_set($batch);
  //   batch_process("/ttaimport/routes");
  // }

  return "import";
}


function _wb_tta_group_by($arr, $key) {
  $result = array();
  foreach ($arr as $data) {
    $id = $data[$key];
    if (isset($result[$id])) {
       $result[$id][] = $data;
    } else {
       $result[$id] = array($data);
    }
  }

  return $result;
}

function batch_example_batch($input){
  $num_operations = count($input['files']);
  $i = 1;
  foreach ($input['files'] as $key => $file) {
    $operations[] = array(
      'my_csv_import',
      array(
        $file->uri,
         $input['dataimporter'],
        t('(Operation @operation)', array('@operation' => $i)),
      ),
    );
    $i++;
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'batch_example_finished',
    'title' => t('Importing CSVs..'),
    'init_message' => t('Csv Import is starting...'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Importing CSV has encountered an error.')
  );
  return $batch;
}

/**
 * Batch operation for batch 1: load a node.
 *
 * This is the function that is called on each operation in batch 1.
 */
function my_csv_import($file, $dataImporter, $operation_details, &$context) {
   $filename = basename($file);
   $dataImporter->readCSV($file);
  // Store some results for post-processing in the 'finished' callback.
  // The contents of 'results' will be available as $results in the
  // 'finished' function (in this example, batch_example_finished()).
  $context['results'][] =  check_plain($filename);

  // Optional message displayed under the progressbar.
  $context['message'] = t('Importing CSV @title =>', array('@title' => $filename)) . ' ' . $operation_details;

  _batch_example_update_http_requests();
}

/**
 * Utility function to increment HTTP requests in a session variable.
 */
function _batch_example_update_http_requests() {
  $_SESSION['http_request_count']++;
}


/**
 * Batch 'finished' callback used by both batch 1 and batch 2.
 */
function batch_example_finished($success, $results, $operations) {
  if ($success) {
    // Here we could do something meaningful with the results.
    // We just display the number of nodes we processed...
    drupal_set_message(t('@count results processed in @requests HTTP requests.', array('@count' => count($results), '@requests' => _batch_example_get_http_requests())));
    drupal_set_message(t('The final result was "%final"', array('%final' => end($results))));
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    drupal_set_message(
      t('An error occurred while processing @operation with arguments : @args',
        array(
          '@operation' => $error_operation[0],
          '@args' => print_r($error_operation[0], TRUE),
        )
      ),
      'error'
    );
  }
}

/**
 * Utility function to count the HTTP requests in a session variable.
 *
 * @return int
 *   Number of requests.
 */
function _batch_example_get_http_requests() {
  return !empty($_SESSION['http_request_count']) ? $_SESSION['http_request_count'] : 0;
}
