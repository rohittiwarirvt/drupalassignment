<?php

/**
 * hook_rules_event_info()
 */

function rh_subscribe_rules_event_info() {

  $items = array(
    'rh_subscribe_user_subscribe' => array(
      'label' => t('After User subscribed to the project'),
      'group' => t('Redhat'),
      'variables' => array(
        'subscribe' => array(
          'label' => t('Redhat Subscribe'),
          'type' => 'subscribe',
          'token type' => 'subscribe',
        ),

      ),
    ),

  );

  return $items;
}

/**
 * Implements of hook_rules_data_info()
 */

function rh_subscribe_rules_data_info() {

  return array(
    'subscribe' => array(
      'label' => t('Subscribe'),
      'group' => t('Redhat'),
    ),

  );
}

