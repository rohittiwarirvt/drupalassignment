<?php

/**
 * Page callback for subscribe form
 */

function rh_subscribe_subscribe_page($js = NULL, $nid) {

  $node = node_load($nid);

  if ($node) {
    drupal_set_breadcrumb(array(
      l('Home', '<front>'),
      l($node->title, 'node/' . $nid),

    ));
  }


  $title = isset($node->title) ? $node->title : 'project';
  $form_state = array(
    'title' => t('Subscribe to !title', array('!title' => $title)),
    'ajax' => TRUE,
    'nid' => $nid,
  );


  if (!$js) {
    $subscribe_form = drupal_build_form('rh_subscribe_form', $form_state);
    return render($subscribe_form);
  }


  ctools_include('modal');
  ctools_include('ajax');


  $output = ctools_modal_form_wrapper('rh_subscribe_form', $form_state);

  if (!empty($form_state['executed'])) {

    $output[] = ctools_modal_command_dismiss();

  }
  print ajax_render($output);
  exit;
}

function rh_subscribe_unsubscribe_page($hash) {
  $s = rh_subscribe_delete($hash);

  if ($s) {
    $node = node_load($s['nid']);
    drupal_set_breadcrumb(array(
      l('Home', '<front>'),
      l($node->title, 'node/' . $s['nid']),
    ));
    drupal_set_message(t('!name has been Un-subscribed to project: <a href="' . url('node/' . $node->nid) . '">!title</a> successful.', array(
      '!title' => $node->title,
      '!name' => $s['name']
    )));
  }

  drupal_goto();
}