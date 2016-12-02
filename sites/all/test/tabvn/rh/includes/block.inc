<?php


/**
 * Implements of hook_block_info()
 */

function rh_block_info() {

  $blocks['recent_redhat_block'] = array(
    'info' => t('[Rehat] Recent block posts, docs'),
  );

  $blocks['home_execdeck_block'] = array(
    'info' => t('[Redhat] Home Execdeck block'),
  );

  return $blocks;

}

/**
 * Implements of hook_block_view()
 */

function rh_block_view($delta = '') {
  $block = array();

  switch ($delta) {
    case 'recent_redhat_block':
      $block['content'] = theme('redhat_recent_block');

      break;

    case 'home_execdeck_block':

      $nid = db_query("SELECT n.nid FROM {node} n INNER JOIN {field_data_field_date_of_deck} d ON d.entity_id = n.nid WHERE n.status = 1 AND n.type = :type ORDER BY d.field_date_of_deck_value DESC LIMIT 1 OFFSET 0", array(":type" => 'executive_deck'))
        ->fetchField();
      if ($nid) {
        $node = node_load($nid);
        $block['content'] = theme('redhat_home_execdeck_block', array('node' => $node));
      }

      break;
  }

  return $block;

}