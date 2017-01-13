<?php


/**
 * Export Drupal Commerce products to Views.
 */

/**
 * Implements hook_views_data()
 */
function commerce_product_views_data() {
  $data = array();
  $data['commerce_product']['table']['group']  = t('Commerce Product');

  $data['commerce_product']['table']['base'] = array(
    'field' => 'product_id',
    'title' => t('Commerce Product'),
    'help' => t('Products from the store.'),
    'access query tag' => 'commerce_product_access',
  );

  $data['commerce_product']['table']['entity type'] = 'commerce_product';
  
  $data['commerce_product']['table']['default_relationship'] = array(
    'commerce_product_revision' => array(
      'table' => 'commerce_product_revision',
      'field' => 'revision_id',
    ),
  );
 
  // Expose the product ID.
  $data['commerce_product']['product_id'] = array(
    'title' => t('Product ID'),
    'help' => t('The unique internal identifier of the product.'),
    'field' => array(
      'handler' => 'commerce_product_handler_field_product',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'commerce_product_handler_argument_product_id',
    ),
  ); 
  return $data;	
}