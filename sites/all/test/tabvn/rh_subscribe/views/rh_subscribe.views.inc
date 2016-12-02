<?php

function rh_subscribe_views_data() {

  $data['rh_subscribe']['table']['group'] = t('Redhat Subscribe');

  $data['rh_subscribe']['table']['base'] = array(
    'field' => 'sid', // This is the identifier field for the view.
    'title' => t('redhat subscribe'),
    'help' => t('rh_subscribe table contains subscription content and can be related to the project.'),
    'weight' => -10,
  );

  // table, the fields are automatically available.
  $data['rh_subscribe']['table']['join'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table.
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );


  $data['rh_subscribe']['sid'] = array(
    'title' => t('Subscribe ID'),
    'help' => t('Just a numeric field.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['rh_subscribe']['nid'] = array(
    'title' => t('Node nid'),
    'help' => t('Subscribe content that references a node.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'relationship' => array(
      'title' => t('Node'),
      'help' => t('Relate subscribe to the project user has subscribed to.'),
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'field' => 'nid',
      'label' => t('Node'),
    ),
    'argument' => array(
      'handler' => 'rh_subscribe_handler_argument_subscribe_nid',
    ),
    'filter' => array(
      'handler' => 'rh_subscribe_handle_filter_node_project', //rh_subscribe_handler_filter_project_select
    ),
  );

  $data['rh_subscribe']['name'] = array(
    'title' => t('Subscriber name'),
    'help' => t('Subscribe name.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['rh_subscribe']['mail'] = array(
    'title' => t('Subscriber mail'),
    'help' => t('Subscribe email address.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['rh_subscribe']['hash'] = array(
    'title' => t('Subscriber hash key'),
    'help' => t('Subscribe hash key.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['rh_subscribe']['created'] = array(
    'title' => t('Time of subscribe'),
    'help' => t('Just a timestamp field.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  return $data;
}
