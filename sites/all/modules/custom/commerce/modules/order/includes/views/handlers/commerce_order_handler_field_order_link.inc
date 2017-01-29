<?php

/**
 * Field handler to present a link to an order.
 */
class commerce_order_handler_field_order_link extends views_handler_field {
 function construct() {
    parent::construct();

    $this->additional_fields['order_id'] = 'order_id';
  }


  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }