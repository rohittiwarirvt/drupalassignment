<?php


/**
 * Form callback wrapper: create or edit an order.
 *
 * @param $order
 *   The order object to edit through the form.
 * @param $account
 *   For new orders, the customer's user account.
 *
 * @see commerce_order_order_form()
 */
function commerce_order_ui_order_form_wrapper($order = NULL, $account = NULL) {
  if (empty($order)) {
    $order = commerce_order_new();
  }
  // Set the page title and a default customer if necessary.
  if (empty($order->order_id)) {
    drupal_set_title(t('Create an order'));

    if (!empty($account)) {
      $order->uid = $account->uid;
    }
  }

  // Include the forms file from the Order module.
  module_load_include('inc', 'commerce_order', 'includes/commerce_order.forms');
  return drupal_get_form('commerce_order_ui_order_form', $order);
}