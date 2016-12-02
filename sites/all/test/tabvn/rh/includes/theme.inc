<?php

/**
 * Implements of hook_theme()
 */
function rh_theme($existing, $type, $theme, $path) {
  return array(
    'redhat_recent_block' => array(
      'variables' => array(),
      'template' => 'redhat-recent-block',
      'path' => drupal_get_path('module', 'rh') . '/templates',
    ),
    'redhat_home_execdeck_block' => array(
      'variables' => array('node' => NULL),
      'template' => 'redhat-home-execdeck-block',
      'path' => drupal_get_path('module', 'rh') . '/templates',
    ),
    'redhat_executive_deck_pdf' => array(
      'variables' => array('node' => NULL),
      'template' => 'redhat-executive-deck-pdf',
      'path' => drupal_get_path('module', 'rh') . '/templates',
    ),

  );
}