<?php

/**
 * @file
 * Definition of views_bootstrap_plugin_style.
 */

/**
 * Class to define a style plugin handler.
 */
class ViewsBsAccordionPluginStyle extends views_plugin_style {

  /**
   * Definition.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['title_field'] = array('default' => NULL);
    $options['auto_expand_first_row'] = array('default' => TRUE);
    return $options;
  }

  /**
   * Options form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    if (isset($form['grouping'])) {
      $options = array();
      foreach (element_children($form['grouping']) as $key => $value) {
        if (!empty($form['grouping'][$key]['field']['#options']) && is_array($form['grouping'][$key]['field']['#options'])) {
          $options = array_merge($options, $form['grouping'][$key]['field']['#options']);
        }
      }

      // Delete grouping.
      unset($form['grouping']);

      $form['title_field'] = array(
          '#type' => 'select',
          '#title' => t('Title field'),
          '#options' => $options,
          '#required' => TRUE,
          '#default_value' => $this->options['title_field'],
          '#description' => t('Select the field that will be used as the title.'),
      );
      $form['auto_expand_first_row'] = array(
          '#type' => 'checkbox',
          '#title' => t('Auto Expand First Row'),
          '#default_value' => $this->options['auto_expand_first_row'],
          '#description' => t('If checked, first row will auto expand.'),
      );
    }
  }

}
