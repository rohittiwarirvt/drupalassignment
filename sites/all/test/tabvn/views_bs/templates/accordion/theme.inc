<?php

/**
 * Implementation of template preprocess for the view.
 */
function template_preprocess_views_bs_accordion_plugin_style(&$vars) {
  $view = &$vars['view'];
  $title_field = $vars['options']['title_field'];
  $auto_expand_first_row = isset($vars['options']['auto_expand_first_row']) ? $vars['options']['auto_expand_first_row'] : FALSE;

  $vars['classes_array'][] = 'panel-group';
  $vars['auto_expand_first_row'][0] = FALSE;

  // Get titles.
  if (isset($view->field[$title_field])) {
    foreach ($vars['view']->result as $key => $field) {
      $vars['titles'][$key] = strip_tags($view->style_plugin->get_field($key, $title_field));
      if ($auto_expand_first_row && $key == 0) {
        $vars['auto_expand_first_row'][$key] = TRUE;
      }else{
        $vars['auto_expand_first_row'][$key] = FALSE;
      }
    }
  }
}
