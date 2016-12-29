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
  $dataRetriever = new TTADataRetriever();
  $dataImporter = new TTADataImporter();

  $tta_zip_filepath = drupal_realpath($form_state['storage']['tta_file_upload']->uri);
  $dataRetriever->extractFiles($tta_zip_filepath);
  $dataImporter->importTTAData($dataRetriever->TTAFileDir());

}
