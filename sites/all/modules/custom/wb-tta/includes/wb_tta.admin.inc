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

    $form['tta_upload_container']['tta_file_upload'] = array(
        '#name' => 'files[tta_file]',
        '#type' => 'file',
        '#title' => t('GoCary GTFS Data (.zip)'),
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

function wb_tta_admin_form_validate($form, &$form_state) {
  $file = file_save_upload('tta_file', array('file_validate_extensions' => array('zip')));
  if ($file) {
    $form_state['storage']['tta_file_upload'] = $file;
  } else {
    form_set_error('tta_gtfs_file', t('No TTA  file was uploaded.'));
  }

}


function handle_update_ttadata_submit($form, &$form_state) {
  // extracter
  $agency = "godurham";
  $dataRetriever = new TTADataRetriever($agency);
  $tta_zip_filepath = drupal_realpath($form_state['storage']['tta_file_upload']->uri);
  $dataRetriever->extractFiles($tta_zip_filepath);

  //
  $dataImporter = new TTADataImporter();
  $dataImporter->setAgency($agency);
  $dataImporter->setBaseDir($dataRetriever->TTAFileDir(). '/' . $agency);
  $dataImporter->importTTAData();

}

function wb_tta_admin_form_callback_action() {

   // extracter
  $agency = "godurham";
  $dataRetriever = new TTADataRetriever($agency);
  // $tta_zip_filepath = drupal_realpath($form_state['storage']['tta_file_upload']->uri);
  // $dataRetriever->extractFiles($tta_zip_filepath);

  //
  $dataImporter = new TTADataImporter();
  $dataImporter->setAgency($agency);
  $dataImporter->setBaseDir("sites/default/files/tta_scratch/godurham/1483091268/godurham");


 $_SESSION['http_request_count'] = 0;
  $files = $dataImporter->geFilesImportTTAData();
  $params['files'] = $files;
  $params['dataimporter'] = $dataImporter;

  $batch = batch_example_batch($params);
  batch_set($batch);
  batch_process("/ttaimport/routes");
  return "import";
}



function batch_example_batch($input){
  $num_operations = count($input['files']);


  for ($i = 0; $i < $num_operations; $i++) {
    // Each operation is an array consisting of
    // - The function to call.
    // - An array of arguments to that function.
    $operations[] = array(
      'my_csv_import',
      array(
        $input['files'][$i],
         $input['dataimporter'],
        t('(Operation @operation)', array('@operation' => $i + 1)),
      ),
    );
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
