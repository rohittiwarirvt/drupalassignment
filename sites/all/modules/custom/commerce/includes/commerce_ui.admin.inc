<?php


function commerce_currency_settings_form($from, &$form_state) {
  $options = array();

  foreach (commerce_currencies(FALSE, TRUE) as $currency_code => $currency) {
      $options[$currency_code] = t("@code - !name", array('@code' => $currency['code'], '!name' => $currency['symbol']));
  }


  $form['default_currency'] = array(
    '#type' => 'select',
    '#title' => t('Default store currency'),
    '#description' => t('The default store currency will be used as the default for all price fields.'),
    '#options' => $options,
    '#default_value' => commerce_default_currency(),
    );

  $form['enabled_currencies'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enabled currencies'),
    '#description' => t('Only enabled currencies will be visible to users when entering prices. The default currency will always be enabled.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['enabled_currencies']['commerce_enabled_currencies'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => variable_get('commerce_enabled_currencies', array('USD' => 'USD')),
  );

  $form['#validate'][] = 'commerce_currency_settings_form_validate';

  return system_settings_form($form);
}


function commerce_currency_settings_form_validate($form, &$form_state) {
  $default = $form_state['values']['commerce_default_currency'];
  $form_state['values']['commerce_enabled_currencies'][$default] = $default;
}
