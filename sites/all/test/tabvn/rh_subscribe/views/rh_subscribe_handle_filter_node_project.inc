<?php


class rh_subscribe_handle_filter_node_project extends views_handler_filter_in_operator {

  public $projects;

  function init(&$view, &$options) {

    parent::init($view, $options);

    $this->get_projects();
  }

  function get_projects() {

    $options = array();
    $result = db_select('node', 'n')
      ->fields('n', array('nid', 'title'))
      ->condition('n.type', 'project')
      ->condition('n.status', 1)
      ->execute()
      ->fetchAll();

    if (!empty($result)) {
      foreach ($result as $k => $v) {
        $options[$v->nid] = $v->title;
      }
    }

    $this->projects = $options;

    return $this->projects;
  }


  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_options = $this->projects;
    }
  }
}
