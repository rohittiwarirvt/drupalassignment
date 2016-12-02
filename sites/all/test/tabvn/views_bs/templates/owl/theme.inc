<?php

function template_preprocess_views_bs_owl_plugin_style(&$vars) {
  $module_path = drupal_get_path('module', 'views_bs');
  $view = $vars['view'];
  $element_id = $view->name . '-' . $view->style_plugin->display->id;


  static $instances = array();
  if (isset($instances[$element_id])) {
    $instances[$element_id] ++;
    $element_id .= "_" . $instances[$element_id];
  } else {
    $instances[$element_id] = 1;
  }
  $vars['element_id'] = $element_id;


  $slider_settings = !empty($view->style_options['owl']) ? $view->style_options['owl'] : array();
  $slider_settings['items'] = isset($slider_settings['items']) ? $slider_settings['items'] : 4;
  $slider_settings['itemsDesktop'] = isset($slider_settings['itemsDesktop']) ? $slider_settings['itemsDesktop'] : '1000,3';
  $slider_settings['itemsDesktopSmall'] = isset($slider_settings['itemsDesktopSmall']) ? $slider_settings['itemsDesktopSmall'] : '979,3';
  $slider_settings['itemsMobile'] = isset($slider_settings['itemsMobile']) ? $slider_settings['itemsMobile'] : '479,1';
  $slider_settings['itemsTablet'] = isset($slider_settings['itemsTablet']) ? $slider_settings['itemsTablet'] : '768,2';
  $itemsDesktop = _views_bs_owl_slider_format($slider_settings['itemsDesktop']);
  $itemsDesktopSmall = _views_bs_owl_slider_format($slider_settings['itemsDesktopSmall']);
  $itemsMobile = _views_bs_owl_slider_format($slider_settings['itemsMobile']); //itemsMobile
  $itemsTablet = _views_bs_owl_slider_format($slider_settings['itemsTablet']); //itemsTablet
  if ($itemsDesktop) {
    $slider_settings['itemsDesktop'] = $itemsDesktop;
  }
  if ($itemsDesktopSmall) {
    $slider_settings['itemsDesktopSmall'] = $itemsDesktopSmall;
  }
  if ($itemsTablet) {
    $slider_settings['itemsTablet'] = $itemsTablet;
  }
  if ($itemsMobile) {
    $slider_settings['itemsMobile'] = $itemsMobile;
  }
  

  $slider_settings['autoPlay'] = !empty($slider_settings['autoPlay']) ? TRUE : FALSE;
  $slider_settings['mouseDrag'] = !empty($slider_settings['mouseDrag']) ? TRUE : FALSE;
  $slider_settings['navigation'] = !empty($slider_settings['navigation']) ? TRUE : FALSE;
  $slider_settings['pagination'] = !empty($slider_settings['pagination']) ? TRUE : FALSE;
  $slider_settings['touchDrag'] = !empty($slider_settings['touchDrag']) ? TRUE : FALSE;
  $slider_settings['navigationText'] = !empty($slider_settings['navigationText']) ? array('prev', 'next') : FALSE;
  //$slider_settings['autoHeight'] = TRUE;
  $slider_settings['paginationSpeed'] = 1000;
  $slider_settings['goToFirstSpeed'] = 2000;
  $slider_settings['responsive'] = TRUE;
  $slider_settings['singleItem'] = FALSE;
  $slider_settings['transitionStyle'] = 'fade';
  $slider_settings['lazyLoad'] = TRUE;
  $slider_settings['itemsScaleUp'] = TRUE;

  drupal_add_js(array(
      'viewsBs' => array(
          'owl_slider' => array(
              $element_id => array(
                  'id' => $element_id,
                  'name' => $vars['view']->name,
                  'view_name' => $vars['view']->name,
                  'display' => $vars['view']->current_display,
                  'owl' => $slider_settings,
              ),
          ),
      ),
          ), 'setting');

  drupal_add_js($module_path . '/lib/owl_carousel/views_bs_owl_slider.js');
}
