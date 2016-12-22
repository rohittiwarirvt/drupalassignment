<?php

class CommerceProductEntityController extends DrupalCommerceEntityController {

  public function create(array $values = array()) {
    $values += array(
      'product_id' => NULL,
      'is_new' => TRUE,
      'sku' => '',
      'revision_id' => NULL,
      'title' => '',
      'uid' => '',
      'status' => 1,
      'created' => '',
      'changed' => '',
    );

    return parent::create($values);
  }

  public function save($product, DatabaseTransaction $transaction = NULL) {
    global $user;
    // Hardcode the changed time.
    $product->changed = REQUEST_TIME;
    if (empty($product->{$this->idKey}) || !empty($product->is_new)) {
      // Set the creation timestamp if not set, for new entities.
      if (empty($product->created)) {
        $product->created = REQUEST_TIME;
      }
    } else {
       if ($product->created === '') {
        unset($product->created);
      }
      if ($product->uid === '') {
        unset($product->uid);
      }
    }

    $product->revision_timestamp = REQUEST_TIME;
    $product->revision_uid = $user->uid;

    // Determine if we will be inserting a new product.
    $product->is_new = empty($product->product_id);

     if ($product->is_new || !empty($product->revision)) {
      // When inserting either a new product or revision, $entity->log must be set
      // because {commerce_product_revision}.log is a text column and therefore
      // cannot have a default value. However, it might not be set at this
      // point, so we ensure that it is at least an empty string in that case.
      if (!isset($product->log)) {
        $product->log = '';
      }
    }
    elseif (empty($product->log)) {
      // If we are updating an existing product without adding a new revision,
      // we need to make sure $entity->log is unset whenever it is empty. As
      // long as $entity->log is unset, drupal_write_record() will not attempt
      // to update the existing database column when re-saving the revision.
      unset($product->log);
    }

    foreach (field_info_instances('commerce_product', $product->type) as $field_name => $instance) {
            // Load the instance's field data.
      $field = field_info_field($instance['field_name']);
      if ($field['type'] == 'commerce_price' && !empty($product->{$field_name})) {
        foreach ($product->{$field_name} as $langcode => &$items) {
          foreach ($items as $delta => &$item) {
            if (!empty($item['data'])) {
              $item['data']['components'] = array();
            }
          }
        }
      }
    }
    // Reset load by SKU static cache.
    drupal_static_reset('commerce_product_load_by_sku');

    return parent::save($product, $transaction);
  }

  /**
   * Unserializes the data property of loaded products.
   */
  public function attachLoad(&$queried_products, $revision_id = FALSE) {
    foreach ($queried_products as $product_id => &$product) {
      $product->data = unserialize($product->data);
    }

        // Call the default attachLoad() method. This will add fields and call
    // hook_commerce_product_load().
    parent::attachLoad($queried_products, $revision_id);
  }

  public function delete($product_ids, DatabaseTransaction $transaction = NULL) {
    if (!empty($product_ids)) {
      $products = $this->load($product_ids, array());
      foreach ((array) $products as $product_id => $product) {
        if (!commerce_product_can_delete($product)) {
          unset($products[$product_id]);
        }
      }

            // If none of the specified products can be deleted, return FALSE.
      if (empty($products)) {
        return FALSE;
      }


      parent::delete(array_keys($products), $transaction);
      return TRUE;
    } else {
      return FALSE;
    }
  }

  public function buildContent($product, $view_mode = 'full', $langcode = NULL, $content = array()) {
        // Prepare a reusable array representing the CSS file to attach to the view.
    $attached = array(
      'css' => array(drupal_get_path('module', 'commerce_product') . '/theme/commerce_product.theme.css'),
    );
    // Add the default fields inherent to the product entity.
    $content['sku'] = array(
      '#markup' => theme('commerce_product_sku', array('sku' => $product->sku, 'label' => t('SKU:'), 'product' => $product)),
      '#attached' => $attached,
    );
    $content['title'] = array(
      '#markup' => theme('commerce_product_title', array('title' => $product->title, 'label' => t('Title:'), 'product' => $product)),
      '#attached' => $attached,
    );

    $content['status'] = array(
      '#markup' => theme('commerce_product_status', array('status' => $product->status, 'label' => t('Status:'), 'product' => $product)),
      '#attached' => $attached,
    );

    return parent::buildContent($product, $view_mode, $langcode, $content);
  }
}
