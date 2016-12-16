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


function commerce_product_ui_product_type_form($form, &$form_state, $product_type) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_product_ui') . '/includes/commerce_product_ui.forms.inc';
    // Store the initial product type in the form state.
  $form_state['product_type'] = $product_type;
    $form['product_type'] = array(
    '#tree' => TRUE,
  );

  $form['product_type']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $product_type['name'],
    '#description' => t('The human-readable name of this product type. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#required' => TRUE,
    '#size' => 32,
  );

    if (empty($product_type['type'])) {
    $form['product_type']['type'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => $product_type['type'],
      '#maxlength' => 32,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'commerce_product_type_load',
        'source' => array('product_type', 'name'),
      ),
      '#description' => t('The machine-readable name of this product type. This name must contain only lowercase letters, numbers, and underscores, it must be unique.'),
    );
  }


  $form['product_type']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Describe this product type. The text will be displayed on the <em>Add new content</em> page.'),
    '#default_value' => $product_type['description'],
    '#rows' => 3,
  );

  $form['product_type']['help'] = array(
    '#type' => 'textarea',
    '#title' => t('Explanation or submission guidelines'),
    '#description' => t('This text will be displayed at the top of the page when creating or editing products of this type.'),
    '#default_value' => $product_type['help'],
    '#rows' => 3,
  );

    $form['product_type']['revision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Default products of this type to be saved as new revisions when edited.'),
    '#default_value' => $product_type['revision'],
  );

  if (module_exists('entity_translation')) {
    $form['product_type']['multilingual'] = array(
      '#type' => 'radios',
      '#title' => t('Multilingual support'),
      '#description' => t('If <em>Entity translation</em> is enabled it will be possible to provide a different version of the same product for each available language.') . '<br />' . t('You can find more options in the <a href="!url">entity translation settings</a>.', array('!url' => url('admin/config/regional/entity_translation'))) . '<br />' . t('Existing products will not be affected by changing this option.'),
      '#options' => array(
        0 => t('Disabled'),
        ENTITY_TRANSLATION_ENABLED => t('Enabled via <em>Entity translation</em>'),
      ),
      '#default_value' => variable_get('language_product_type_' . $product_type['type'], 0),
    );
  }


  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 40,
  );

    // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save product type'),
    '#submit' => array_merge($submit, array('commerce_product_ui_product_type_form_submit')),
  );

  if (!empty($form_state['product_type']['type'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete product type'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/products/types'),
      '#submit' => array_merge($submit, array('commerce_product_ui_product_type_form_delete_submit')),
      '#weight' => 45,
    );
  }
  else {
    $form['actions']['save_continue'] = array(
      '#type' => 'submit',
      '#value' => t('Save and add fields'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/products/types'),
      '#submit' => array_merge($submit, array('commerce_product_ui_product_type_form_submit')),
      '#weight' => 45,
    );
  }

  $form['#validate'][] = 'commerce_product_ui_product_type_form_validate';

  return $form;
}




function commerce_product_ui_product_type_form_validate($form, &$form_state) {
  $product_type = $form_state['product_type'];

  // If saving a new product type, ensure it has a unique machine name.
  if (empty($product_type['type'])) {
    if (!commerce_product_ui_validate_product_type_unique($form_state['values']['product_type']['type'])) {
      form_set_error('product_type][type', t('The machine name specified is already in use.'));
    }
  }
}

/**
 * Form submit handler: save a product type.
 */
function commerce_product_ui_product_type_form_submit($form, &$form_state) {
    $product_type = $form_state['product_type'];
    $updated = !empty($product_type['type']);

      // If a type is set, we should still check to see if a row for the type exists
  // in the database; this is done to accomodate types defined by Features.
  if ($updated) {
    $updated = db_query('SELECT 1 FROM {commerce_product_type} WHERE type = :type', array(':type' => $product_type['type']))->fetchField();
  }

  foreach ($form_state['values']['product_type'] as $key => $value) {
    $product_type[$key] = $value;
  }

    // Write the product type to the database.
  $product_type['is_new'] = !$updated;

    commerce_product_ui_product_type_save($product_type);

      // Set the multingual value for the product type if entity translation is enabled.
  if (module_exists('entity_translation')) {
    variable_set('language_product_type_' . $product_type['type'], $product_type['multilingual']);
  }

    // Redirect based on the button clicked.
  drupal_set_message(t('Product type saved.'));

  if ($form_state['triggering_element']['#parents'][0] == 'save_continue') {
    $form_state['redirect'] = 'admin/commerce/products/types/' . strtr($product_type['type'], '_', '-') . '/fields';
  }
  else {
    $form_state['redirect'] = 'admin/commerce/products/types';
  }
}


/**
 * Submit callback for delete button on commerce_product_ui_product_type_form().
 *
 * @see commerce_product_ui_product_type_form()
 */
function commerce_product_ui_product_type_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/products/types/' . strtr($form_state['product_type']['type'], '_', '-') . '/delete';
}

/**
 * Form callback: confirmation form for deleting a product type.
 *
 * @param $product_type
 *   The product type array to be deleted.
 *
 * @see confirm_form()
 */
function commerce_product_ui_product_type_delete_form($form, &$form_state, $product_type) {
  $form_state['product_type'] = $product_type;
      // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_product_ui') . '/includes/commerce_product_ui.forms.inc';

  $form['#submit'][] = 'commerce_product_ui_product_type_delete_form_submit';

    $form = confirm_form($form,
    t('Are you sure you want to delete the %name product type?', array('%name' => $product_type['name'])),
    'admin/commerce/products/types',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}


/**
 * Submit callback for commerce_product_product_type_delete_form().
 */
function commerce_product_ui_product_type_delete_form_submit($form, &$form_state) {
  $product_type = $form_state['product_type'];
  commerce_product_ui_product_type_delete($product_type['type']);
  drupal_set_message(t('The product type %name has been deleted.', array('%name' => $product_type['name'])));
  watchdog('commerce_product', 'Deleted product type %name.', array('%name' => $product_type['name']), WATCHDOG_NOTICE);
  $form_state['redirect'] = 'admin/commerce/products/types';
}


// function commerce_product_ui_product_delete_form($form, &$form_state, $product) {
//    $form_state['product_type'] = $product_type;

//   // Ensure this include file is loaded when the form is rebuilt from the cache.
//   $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_product_ui') . '/includes/commerce_product_ui.forms.inc';
// }
