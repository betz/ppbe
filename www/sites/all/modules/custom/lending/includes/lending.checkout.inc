<?php // -*- drupal -*-

/**
 * @file
 * Everything to deal with the actual checkout form.  This includes
 * the display of the checkout form as well as the validation and
 * submission of it.
 */

function lending_show_checkout_form(&$node) {
  if (! lending_is_checked_out($node->nid)) {
    $node->content['lending_checkoutform'] = array(
      '#value' => drupal_get_form('lending_checkoutform', $node),
      '#weight' => 9
    );
  }
}

function lending_checkoutform($form_state, $node) {
  $form['checkout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Checkout this item for a member')
  );

  $form['checkout']['user_name'] = array(
    '#title' => t('User Name'),
    '#type' => 'textfield',
    '#description' => t('The user id to check out for'),
    '#autocomplete_path' => 'user/autocomplete'
  );
  // For convenience, save the node ID.
  $form['checkout']['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );


  $form['checkout']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );

  return $form;
}

function lending_checkoutform_validate($form, &$form_state) {
  $params = array();
  $params['name'] = $form_state['values']['user_name'];
  $lendee = user_load($params);

  if (! $lendee) {
    form_set_error('user_name', t('User is not valid'));
    return;
  }

  if ( lending_not_user_or_admin($lendee) ) {
    form_set_error('user_name', t('User is not allowed to check out items'));
  }

  $form_state['values']['uid'] = $lendee->uid;

  if ( lending_is_checked_out($form_state['values']['user_name']) ) {
    form_set_error('submit', t('The item is checked out'));
  }
}

function lending_checkoutform_submit($form_id, $form_state) {
  global $user;

  $success = lending_checkout_item(
    $form_state['values']['nid'],
    $form_state['values']['uid'],
    $user->uid
  );

  if ($success) {
    drupal_set_message(t('Checkout completed'));
  } else {
    drupal_set_message(t('Checkout FAILED!'));
  }
}


