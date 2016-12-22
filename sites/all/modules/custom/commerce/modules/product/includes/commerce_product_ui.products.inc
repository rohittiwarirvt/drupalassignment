<?php

function commerce_product_ui_add_page() {
	$item = menu_get_item();

	$content = system_admin_menu_block($item);
	if (count($content) == 1) {
		$item = array_shift($content);
		drupal_goto($item['href']);
	}

	return theme('product_add_list', ['content' => $content]);
}


function theme_product_add_list($variables) {
	$content = $variables['content'];
	$output = "";
	if ($content) {
		 $output = '<dl class="commerce-product-type-list">';
		 foreach ($content as  $item) {
   			$output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
   			$output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
		 }
		 	$output .= '</dl>';
	} else {
	  if (user_access('administer product types')) {
	      $output = '<p>' . t('You have not created any product types yet. Go to the <a href="@create-product-type">product type creation page</a> to add a new product type.', array('@create-product-type' => url('admin/commerce/products/types/add'))) . '</p>';
	    }
	    else {
	      $output = '<p>' . t('No product types have been created yet for you to use.') . '</p>';
	    }
	}
  return $output;
}


function commerce_product_ui_product_form_wrapper($product) {
  module_load_include('inc', 'commerce_product', 'includes/commerce_product.forms');
  return drupal_get_form('commerce_product_ui_product_form', $product);
}

/**
 * Form callback wrapper: confirmation form for deleting a product.
 *
 * @param $product
 *   The product object being deleted by this form.
 *
 * @see commerce_product_product_delete_form()
 */
function commerce_product_ui_product_delete_form_wrapper($product) {
  // Include the forms file from the Product module.
  module_load_include('inc', 'commerce_product', 'includes/commerce_product.forms');
  return drupal_get_form('commerce_product_ui_product_delete_form', $product);
}
