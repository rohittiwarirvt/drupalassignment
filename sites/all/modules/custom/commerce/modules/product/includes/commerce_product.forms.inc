<?php

function commerce_product_ui_product_form($form, &$form_state, $product) {
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_product') . '/includes/commerce_product.forms.inc';

   $form['sku'] = array(
    '#type' => 'textfield',
    '#title' => t('Product SKU'),
    '#description' => t('Supply a unique identifier for this product using letters, numbers, hyphens, and underscores. Commas may not be used.'),
    '#default_value' => $product->sku,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#weight' => -10,
  );


  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $product->title,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

   // Add the field related form elements.
  $form_state['commerce_product'] = $product;

  $langcode = entity_language('commerce_product', $product);

  if (empty($langcode)) {
    $langcode = LANGUAGE_NONE;
  }

  field_attach_form('commerce_product', $product, $form, $form_state, $langcode);

  $form['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#description' => t('Disabled products cannot be added to shopping carts and may be hidden in administrative product lists.'),
    '#options' => array(
      '1' => t('Active'),
      '0' => t('Disabled'),
    ),
    '#default_value' => $product->status,
    '#required' => TRUE,
    '#weight' => 35,
  );

   $product_type = commerce_product_type_load($product->type);

   $form['change_history'] = array(
    '#type' => 'fieldset',
    '#title' => t('Change history'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($product->product_id) || empty($product_type['revision']),
    '#weight' => 350,
  );

  if (!empty($product->product_id)) {
    $form['change_history']['revision'] = array(
      '#type' => 'checkbox',
      '#title' => t('Create new revision on update'),
      '#description' => t('If an update log message is entered, a revision will be created even if this is unchecked.'),
      '#default_value' => $product_type['revision'],
      '#access' => user_access('administer commerce_product entities'),
    );
  }

   $form['change_history']['log'] = array(
    '#type' => 'textarea',
    '#title' => !empty($product->product_id) ? t('Update log message') : t('Creation log message'),
    '#rows' => 4,
    '#description' => t('Provide an explanation of the changes you are making. This will provide a meaningful history of changes to this product.'),
  );

   $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 400,
  );

     // Simply use default language
  $form['language'] = array(
    '#type' => 'value',
    '#value' => $langcode,
  );

  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save product'),
    '#submit' => array_merge($submit, array('commerce_product_product_form_submit')),
  );

  $form['#validate'][] = 'commerce_product_product_form_validate';

  return $form;
}


function commerce_product_product_form_validate($form, &$form_state) {
  $product = $form_state['commerce_product'];
  $sku = $form_state['values']['sku'];

  if ($sku === FALSE) {
    form_set_error('sku', t('The SKU contains invalid tokens.'));
  }
  else {
    $query = new EntityFieldQuery();

    $query
      ->entityCondition('entity_type', 'commerce_product')
      ->propertyCondition('sku', $sku);
     $result = $query->execute();

         if (!empty($result)) {
         	$product_id = key($result['commerce_product']);
         	if ($product_id != $product->product_id) {
		        form_set_error('sku', t('This SKU is <a href="!url">already in use</a> and must be unique. Please supply another value.', array('!url' => url('admin/commerce/products/' . $product_id))));
		      }
         }

          if (!commerce_product_validate_sku($sku)) {
      		form_set_error('sku', t('The SKU %sku contains invalid characters.', array('%sku' => $sku)));
   		  }
	    // Trim leading and trailing whitespace from the SKU.
	    form_set_value($form['sku'], trim($sku), $form_state);
  }

  field_attach_form_validate('commerce_product', $product, $form, $form_state);
}


function commerce_product_product_form_submit($form, &$form_state) {
  global $user;

  $product = &$form_state['commerce_product'];

    // Save default parameters back into the $product object.
  $product->sku = $form_state['values']['sku'];
  $product->title = $form_state['values']['title'];
  $product->status = $form_state['values']['status'];
  $product->language = $form_state['values']['language'];

    // Set the product's uid if it's being created at this time.
  if (empty($product->product_id)) {
    $product->uid = $user->uid;
  }

  // Trigger a new revision if the checkbox was enabled or a log message supplied.
  if ((user_access('administer commerce_product entities') && !empty($form_state['values']['revision'])) ||
    (!user_access('administer commerce_product entities') && !empty($form['change_history']['revision']['#default_value'])) ||
    !empty($form_state['values']['log'])) {
    $product->revision = TRUE;
    $product->log = $form_state['values']['log'];
  }

  // Notify field widgets.
  field_attach_submit('commerce_product', $product, $form, $form_state);

    // Save the product.
  commerce_product_save($product);

    // Redirect based on the button clicked.
  drupal_set_message(t('Product saved.'));
}
